// Copyright (c) 2016-2017 Plaxton/Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// Contains Dynamic Exclusive Parameters

#include "dynamicparams.h"
#include "chain.h"
#include "main.h"
#include "pow.h"

const CBlockIndex* GetLastBlockIndexer(const CBlockIndex* pindex)
{
    while (pindex && pindex->pprev)
        pindex = pindex->pprev;
    return pindex;
}

//////////////////////////////////////////////////////////////////////////////////////
/////// Forking Logic																//
//////////////////////////////////////////////////////////////////////////////////////

bool CheckForkIsTrue(ForkID identifier, const CBlockIndex* pindexLast, bool fTableFlip) {
	bool booleanParam;
	const Consensus::Params& consensusParams = Params().GetConsensus();

	int placeholderIntX = 2;
	int placeholderIntY = 3;

	// Check if we are handling a valid fork
	if (identifier == DELTA_RETARGET || identifier == PRE_DELTA_RETARGET || identifier == START_DYNODE_PAYMENTS || identifier == FORK_SLOT_3 || identifier == FORK_SLOT_4) {  
		// Have we forked to the DELTA Retargeting Algorithm? (We're using pindexLast here because of logical reason)
		if((pindexLast->nHeight + 1) > consensusParams.nUpdateDiffAlgoHeight && identifier == DELTA_RETARGET) { booleanParam = true; }
		// Are we using the reward system before DELTA Retargeting's Fork?
		else if ((pindexLast->nHeight + 1) < consensusParams.nUpdateDiffAlgoHeight && identifier == PRE_DELTA_RETARGET) { booleanParam = true; }
		// Have we now formally enabled Dynode Payments?
		else if ((pindexLast->nHeight + 1) > consensusParams.nDynodePaymentsStartBlock && identifier == START_DYNODE_PAYMENTS) { booleanParam = true; }
		// Empty Forking Slot III
		else if (placeholderIntX == placeholderIntY && identifier == FORK_SLOT_3) { booleanParam = true; } 
		// Empty Forking Slot IV
		else if (placeholderIntX == placeholderIntY && identifier == FORK_SLOT_4) { booleanParam = true; } 
		// All parameters do not lead to forks!
		else { booleanParam = false; }
	// There seems to be an invalid entry!
	} else { throw std::runtime_error(strprintf("%s: Unknown Fork Verification Cause! %s.", __func__, identifier)); }

	return booleanParam;
}

//////////////////////////////////////////////////////////////////////////////////////
/////// Legacy Code that has been replaced by its current counterpart				//
//////////////////////////////////////////////////////////////////////////////////////

unsigned int LegacyRetargetBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
	unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

	if (pindexLast == NULL)
		return nProofOfWorkLimit; // genesis block

	const CBlockIndex* pindexPrev = GetLastBlockIndexer(pindexLast);
	if (pindexPrev->pprev == NULL)
		return nProofOfWorkLimit; // first block
		
	const CBlockIndex* pindexPrevPrev = GetLastBlockIndexer(pindexPrev->pprev);
	if (pindexPrevPrev->pprev == NULL)
		return nProofOfWorkLimit; // second block
	
	int64_t nTargetSpacing = params.nPowTargetTimespan;
	int64_t nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();

	if (nActualSpacing < 0) {
		nActualSpacing = nTargetSpacing;
	}
	else if (nActualSpacing > nTargetSpacing * 10) {
		nActualSpacing = nTargetSpacing * 10;
	}

	// target change every block
	// retarget with exponential moving toward target spacing
	// Includes fix for wrong retargeting difficulty by Mammix2
	arith_uint256 bnNew;
	bnNew.SetCompact(pindexPrev->nBits);

	int64_t nInterval = 10;
	bnNew *= ((nInterval - 1) * nTargetSpacing + nActualSpacing + nActualSpacing);
	bnNew /= ((nInterval + 1) * nTargetSpacing);

    if (bnNew <= 0 || bnNew > nProofOfWorkLimit)
    bnNew = nProofOfWorkLimit;

	return bnNew.GetCompact();

}

//////////////////////////////////////////////////////////////////////////////////////
/////// Genesis Block Information													//
//////////////////////////////////////////////////////////////////////////////////////

// Genesis Mining Code

void MineGenesis(CBlockHeader& genesisBlock, const uint256& powLimit, bool noProduction)
{
    if(noProduction)
        genesisBlock.nTime = std::time(0);
    genesisBlock.nNonce = 0;

    printf("NOTE: Genesis nTime = %u \n", genesisBlock.nTime);
    printf("WARN: Genesis nNonce (BLANK!) = %u \n", genesisBlock.nNonce);

    arith_uint256 besthash;
    memset(&besthash,0xFF,32);
    arith_uint256 hashTarget = UintToArith256(powLimit);
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    arith_uint256 newhash = UintToArith256(genesisBlock.GetHash());
    while (newhash > hashTarget) {
        genesisBlock.nNonce++;
        if (genesisBlock.nNonce == 0) {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesisBlock.nTime;
        }
        // If nothing found after trying for a while, print status
        if ((genesisBlock.nNonce & 0xfff) == 0)
            printf("nonce %08X: hash = %s (target = %s)\n",
                   genesisBlock.nNonce, newhash.ToString().c_str(),
                   hashTarget.ToString().c_str());

        if(newhash < besthash) {
            besthash = newhash;
            printf("New best: %s\n", newhash.GetHex().c_str());
        }
        newhash = UintToArith256(genesisBlock.GetHash());
    }
    printf("Genesis nTime = %u \n", genesisBlock.nTime);
    printf("Genesis nNonce = %u \n", genesisBlock.nNonce);
    printf("Genesis nBits: %08x\n", genesisBlock.nBits);
    printf("Genesis Hash = %s\n", newhash.ToString().c_str());
    printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

// Block Information

CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, const uint32_t nTime, const uint32_t nNonce, const uint32_t nBits, const int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 1489862748 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */

CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times 03/18/2017: Britain Livid on Spying Claim, but Trump Isn’t Apologizing";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}
