// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"
#include "checkpoints.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xd2;
        pchMessageStart[1] = 0x53;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0xe4;
        vAlertPubKey = ParseHex("042f222989769417eb053cd9f16a9a2894ad5ebbd551be1a4bd2d10cdb679228c91e26e26900eb9fd13c016ed91528241bcf3bd55023679be17f0bd3a16e6fbeba");
        nDefaultPort = 34575;
        nRPCPort = 34574;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
		
        const char* pszTimestamp = "http://www.theguardian.com/us-news/live/2016/apr/13/us-election-campaign-live-sanders-clinton-trump-cruz-kasich";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1460576704, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1460576704;
        genesis.nBits    = 0x1f00ffff; 
        genesis.nNonce   = 275475;

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0xd4487c6a177d2c4562828fd650047f17878e19dd4dfb93635162ec4e2bf4b7f5"));
        assert(genesis.hashMerkleRoot == uint256("0x381fcf23c50c4f60d34f351eb4a3adc3ec980333b9e458b7a730494f7d97fecd"));

        
        base58Prefixes[PUBKEY_ADDRESS] = list_of(12);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(24);
        base58Prefixes[SECRET_KEY] =     list_of(140);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0xd5)(0x92)(0x6c);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0xd5)(0x91)(0xfa);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nPOSStartBlock = 1;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x1d;
        pchMessageStart[1] = 0xd7;
        pchMessageStart[2] = 0x2f;
        pchMessageStart[3] = 0x0e;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("042df222989769437eb53cd9f46a9a2894ad5ebbd551beda4bd210cdb679228c9ae26e26900eb9fd013c076ed97528247bcf3bd55023679be77f0bd3a16e6fbeba");
        nDefaultPort = 44575;
        nRPCPort = 44574;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = 520649337; 
        genesis.nNonce = 47342;

        //assert(hashGenesisBlock == uint256(""));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(30);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(60);
        base58Prefixes[SECRET_KEY]     = list_of(154);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0xb3)(0x77)(0x9a)(0xBC);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0xb3)(0x77)(0x5d)(0xDA);


        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nPOSStartBlock = 1;

    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    
    bool fTestNet = GetBoolArg("-testnet", false);
    
    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
