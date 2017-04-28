// Copyright (c) 2014-2017 The Dash Core Developers
// Copyright (c) 2016-2017 Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"
#include "bip39.h"
#include "chainparams.h"
#include "hdchain.h"
#include "util.h"
#include "utilstrencodings.h"

bool CHDChain::SetNull()
{
    nVersion = CHDChain::CURRENT_VERSION;
    nExternalChainCounter = 0;
    nInternalChainCounter = 0;
    vchSeed.clear();
    id = uint256();
    return IsNull();
}

bool CHDChain::IsNull() const
{
    return vchSeed.empty() || id == uint256();
}

void CHDChain::Debug()
{
    DBG(
        printf("mnemonic: '%s'\n", std::string(vchMnemonic.begin(), vchMnemonic.end()).c_str());
        printf("mnemonicpassphrase: '%s'\n", std::string(vchMnemonicPassphrase.begin(), vchMnemonicPassphrase.end()).c_str());

        printf("seed: '%s'\n", HexStr(vchSeed).c_str());

        CExtKey extkey;
        extkey.SetMaster(&vchSeed[0], vchSeed.size());

        CBitcoinExtKey b58extkey;
        b58extkey.SetKey(extkey);
        printf("extended private masterkey: '%s'\n", b58extkey.ToString().c_str());

        CExtPubKey extpubkey;
        extpubkey = extkey.Neuter();

        CBitcoinExtPubKey b58extpubkey;
        b58extpubkey.SetKey(extpubkey);
        printf("extended public masterkey: '%s'\n", b58extpubkey.ToString().c_str());
    );
}

bool CHDChain::SetMnemonic(const std::vector<unsigned char>& vchMnemonicIn, const std::vector<unsigned char>& vchMnemonicPassphraseIn, bool fUpdateID)
{
    std::vector<unsigned char> vchMnemonicTmp = vchMnemonicIn;

    if (fUpdateID) {
        // can't (re)set mnemonic if seed was already set
        if (!IsNull())
            return false;

        std::string strMnemonic(vchMnemonicIn.begin(), vchMnemonicIn.end());
        std::string strMnemonicPassphrase(vchMnemonicPassphraseIn.begin(), vchMnemonicPassphraseIn.end());

        // empty mnemonic i.e. "generate a new one"
        if (vchMnemonicIn.empty()) {
            strMnemonic = mnemonic_generate(128);
            vchMnemonicTmp = std::vector<unsigned char>(strMnemonic.begin(), strMnemonic.end());
        }
        // NOTE: default mnemonic passphrase is an empty string

        if (!mnemonic_check(strMnemonic.c_str())) {
            throw std::runtime_error(std::string(__func__) + ": invalid mnemonic: `" + strMnemonic + "`");
        }

        uint8_t seed[64];
        mnemonic_to_seed(strMnemonic.c_str(), strMnemonicPassphrase.c_str(), seed, 0);
        vchSeed = std::vector<unsigned char>(seed, seed + 64);
        id = GetSeedHash();
    }

    vchMnemonic = vchMnemonicTmp;
    vchMnemonicPassphrase = vchMnemonicPassphraseIn;

    Debug();

    return !IsNull();
}

bool CHDChain::GetMnemonic(std::vector<unsigned char>& vchMnemonicRet, std::vector<unsigned char>& vchMnemonicPassphraseRet) const
{
    // mnemonic was not set, fail
    if (vchMnemonic.empty())
        return false;

    vchMnemonicRet = vchMnemonic;
    vchMnemonicPassphraseRet = vchMnemonicPassphrase;
    return true;
}

bool CHDChain::GetMnemonic(std::string& strMnemonicRet, std::string& strMnemonicPassphraseRet) const
{
    // mnemonic was not set, fail
    if (vchMnemonic.empty())
        return false;

    strMnemonicRet = std::string(vchMnemonic.begin(), vchMnemonic.end());
    strMnemonicPassphraseRet = std::string(vchMnemonicPassphrase.begin(), vchMnemonicPassphrase.end());

    return true;
}

bool CHDChain::SetSeed(const std::vector<unsigned char>& vchSeedIn, bool fUpdateID)
{
    vchSeed = vchSeedIn;

    if (fUpdateID) {
        id = GetSeedHash();
    }

    Debug();

    return !IsNull();
}

std::vector<unsigned char> CHDChain::GetSeed() const
{
    return vchSeed;
}

uint256 CHDChain::GetSeedHash()
{
    return Hash(vchSeed.begin(), vchSeed.end());
}

void CHDChain::DeriveChildExtKey(uint32_t childIndex, CExtKey& extKeyRet, bool fInternal)
{
    // Use BIP44 keypath scheme i.e. m / purpose' / coin_type' / account' / change / address_index
    CExtKey masterKey;              //hd master key
    CExtKey purposeKey;             //key at m/purpose'
    CExtKey cointypeKey;            //key at m/purpose'/coin_type'
    CExtKey accountKey;             //key at m/purpose'/coin_type'/account'
    CExtKey changeKey;              //key at m/purpose'/coin_type'/account'/change
    CExtKey childKey;               //key at m/purpose'/coin_type'/account'/change/address_index

    masterKey.SetMaster(&vchSeed[0], vchSeed.size());

    // Use hardened derivation for purpose, coin_type and account
    // (keys >= 0x80000000 are hardened after bip32)
    // TODO: support multiple accounts, external/internal addresses, and multiple index per each

    // derive m/purpose'
    masterKey.Derive(purposeKey, 44 | 0x80000000);
    // derive m/purpose'/coin_type'
    purposeKey.Derive(cointypeKey, Params().ExtCoinType() | 0x80000000);
    // derive m/purpose'/coin_type'/account'
    cointypeKey.Derive(accountKey, 0x80000000);
    // derive m/purpose'/coin_type'/account/change
    accountKey.Derive(changeKey, fInternal ? 1 : 0);
    // derive m/purpose'/coin_type'/account/change/address_index
    changeKey.Derive(extKeyRet, childIndex);
}

std::string CHDPubKey::GetKeyPath() const
{
    return strprintf("m/44'/%d'/%d'/%d/%d", Params().ExtCoinType(), nAccount, nChange, extPubKey.nChild);
}