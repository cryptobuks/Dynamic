**Dynamic v1.4.0.0**

* [Qt] Fix for issues with startup and multiple monitors on Windows.
* Update error message for old wallet version issues
* Lock Icon can now be clicked to unlock wallet
* Fix Segfault/Init Load Issue
* Implement Full HD Wallet Functionality
* [QT] Fix issue with enableWallet
* [QT] Fix several signal/slot issues
* [Qt] Use nPowTargetSpacing constant
* [Qt] Show ModalOverlay by pressing the progress bar, disabled show() in sync mode
* [Qt] use NotifyHeaderTip's height and date for the progress update
* [Qt] sync-overlay: Don't show progress twice
* [Qt] sync-overlay: Don't show estimated number of headers left
* [Qt] sync-overlay: Don't block during reindex
* [Qt] modalinfolayer: removed unused comments, renamed signal, code style overhaul
* [Qt] only update "amount of blocks left" when the header chain is in-sync
* [Qt] add out-of-sync modal info layer
* Remove namespace std; usages from entire codebase
* Remove unused local variable shadowing upper local in sighash_tests.cpp
* UpdateTip: log only one line at most per block
* Qt: Sort transactions by date
* VerifyDB: don't check blocks that have been pruned
* Disable the mempool P2P command when bloom filters disabled
* Consensus: Decouple from chainparams.o and timedata.o
* qt: askpassphrasedialog: Clear pass fields on accept
* Repair rpcconsole.cpp signal slot
* CAddrman::Deserialize handle corrupt serializations better
* net: Avoid duplicate getheaders requests
* Move GetAccountBalance from rpcwallet.cpp to CWallet::GetAccountBalance
* Optimise reindex
* Removal of unused function in random.cpp
* Bug fixes and improvements to traffic graph widget
* Shift fork logic to its own file
* Fix issue with headers first sync
* Update RPC error type in dnsync
* Ensure Sporks are synced before allowing mining with GBT
* Threads increased to 4 in hash.h for Argon2d
* Protocol Bumped to 70400
* Min Proto Bump for PrivateSend and Dynode POSE_BAN to 70300
* Inline crypter.cpp with Bitcoin 0.13
* Reject payment vote if Dynode rank cannot be calculated
* Fix ProcessVerifyBroadcast
* Fix potential race condition in vote processing
* Multiple vote fix
* Update const usages in crypter.cpp/h
* Refactor the fill_segment code in the Argon2d core
* Fix bad pointer copy when using custom in Argon2d core
* Eliminate redundant function incompatible with Win in black2-impl.h
* Fix memory leak in fill_memory_blocks in Argon2d core
* Inline sync.cpp/h with Bitcoin 0.13
* Inline GBT with Bitcoin 0.13
* Add scam warning to RPC console
* Multiple watchdog rate check
* Modify DynodeRateCheck to support updating buffers only on failure
* Update rate check buffer only when fAddToSeen is true
* Amend Venerability with 'dnsync next'


**Dynamic v1.3.0.2**

* [Sync] Fix issue with headers first sync
* [Sync] [Consensus] Shift Fork Logic to its own file
* [Qt] Add CheckForks in the Qt Project File
* [Fork] Silence usage of pindex compeletely
* [Sync]Timeouts/DB/Headers/Limits
* Increase nDefaultDbCache from 300MiB to 512MiB
* Bump Proto and ONLY connect to 1.3.0.1 (Proto 70200)
* Bump Governance/Core Proto/Versions


**Dynamic v1.3.0.1**

* Bump Protocols to lock out nodes at or below v1.2 to prevent any forks


**Dynamic v1.3.0.0**

* c++11:Backport from bitcoin-core: don't throw from the reverselock destructor
* InitError instead of throw on failure
* Hard Fork at block 300,000 for Delta difficulty retarget algorithm


**Dynamic v1.2.0.0**

* Make RelayWalletTransaction attempt to AcceptToMemoryPool
* Update tests for Byteswap
* Ensure is in valid range
* Make strWalletFile const
* Avoid ugly exception in log on unknown inv type
* libconsensus: Add input validation of flags/missing flags & make catch() args const
* Add LockedPool
* Add getmemoryinfo
* Add benchmark for lockedpool allocation/deallocation
* trivial: fix bloom filter init to isEmpty = true
* Lockedpool fixes
* Improve DecodeBase58 performance.
* Add include utility to miner.cpp
* Don't return the address of a P2SH of a P2SH
* Implement (begin|end)_ptr in C++11 and add deprecation comment
* add include stdlib.h to random.cpp
* Generate auth cookie in hex instead of base64
* Do not shadow variable (multiple files)
* dynamic-cli: More detailed error reporting
* Add High TX Fee Warning
* C++11: s/boost::scoped_ptr/std::unique_ptr/
* Do not shadow variables in networking code
* Remove shadowed variables in Qt files
* Do not shadow LOCK's criticalblock variable for LOCK inside LOCK
* Do not shadow variables in src/wallet
* Berkeley DB v6 compatibility fix
* Reduce default number of blocks to check at startup
* Fix random segfault when closing Choose data directory dialog
* Fix several node initialization issues
* Add FEATURE_HD
* Improve handling of unconnecting headers
* Fix DoS vulnerability in mempool acceptance
* Bump default db cache to 300MiB
* Fix a bug where the SplashScreen will not be hidden during startup
* Stop trimming when mapTx is empty
* Evict orphans which are included or precluded by accepted blocks
* Reduce unnecessary hashing in signrawtransaction
* Watchdog check removed until Sentinel is updated/compatible fully
* Bump protocol versions to 70000
* Added IPv4 seed nodes to chainparamsseeds.h


**Dynamic v1.1.0.0**

* Inline with BTC 0.12		
* HD Wallet Code Improvements		
* Remove/Replace Boost usage for c++11		
* Do not shadow member variables in httpserver		
* Update dbwrapper_tests.cpp		
* Access WorkQueue::running only within the cs lock		
* Use BIP32_HARDENED_KEY_LIMIT		
* Update NULL to use nullptr in GetWork & GetBlockTemplate		
* Few changes to governance rpc		
* Safety check in CInstantSend::SyncTransaction		
* Full path in 'failed to load cache' warnings		
* Refactor privateSendSigner		
* Net Fixes/DNS Seed Fix		
* Don't add non-current watchdogs to seen map		
* [RPC] remove the option of having multiple timer interfaces		
* Fix memory leak in httprpc.cpp		
* Make KEY_SIZE a compile-time constant
