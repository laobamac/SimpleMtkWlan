# Output work-loop contract

`python3 tests/tx-output/test-output-config.py` compiles the real configureInterface method with a contract-recording interface stub. It verifies kIONetworkWorkLoopSynchronous, propagation of rejected pull configuration, and existing early failure paths.

The pull-model default runs outputStart outside the work loop. TX touches the same unprotected descriptor indices and TXWI freelist as the interrupt event source on the controller main work loop. Selecting the documented synchronous option serializes those paths. Source: Apple IONetworkingFamily IONetworkInterface.cpp, if_start/if_start_gated; SDK IONetworkInterface.h configureOutputPullModel. This test checks API configuration, not Darwin scheduling or radio stability. Hardware validation must record the exact built revision.
