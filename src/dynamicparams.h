// Copyright (c) 2016-2017 Plaxton/Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_DYNAMIC_H
#define DYNAMIC_DYNAMIC_H

#include "arith_uint256.h"

#include "hash.h"
#include "util.h"
#include "uint256.h"

#include "chain.h"
#include "chainparams.h"
#include "uint256.h"
#include "util.h"

#include "consensus/merkle.h"

#include "primitives/block.h"
#include "primitives/transaction.h"

#include <algorithm>

class CBlockIndex;

// Dynamic Constants
static const CAmount MIN_TXOUT_AMOUNT = MIN_TX_FEE;
static const CAmount BLOCKCHAIN_INIT_REWARD = COIN * 0;
static const CAmount PHASE_1_POW_REWARD = COIN * 1;
static const CAmount PHASE_1_DYNODE_PAYMENT = COIN * 0.382;
static const CAmount PHASE_2_DYNODE_PAYMENT = COIN * 0.618;

enum ForkID {
	DELTA_RETARGET = 1,
	PRE_DELTA_RETARGET = 2,
	START_DYNODE_PAYMENTS = 3,
	FORK_SLOT_3 = 4,
	FORK_SLOT_4 = 5
};

bool CheckForkIsTrue(ForkID identifier, const CBlockIndex* pindexLast, bool fTableFlip=false);
unsigned int LegacyRetargetBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);

void MineGenesis(CBlockHeader& genesisBlock, const uint256& powLimit, bool noProduction);
CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, const uint32_t nTime, const uint32_t nNonce, const uint32_t nBits, const int32_t nVersion, const CAmount& genesisReward);
CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward);

#endif // DYNAMIC_DYNAMIC_H