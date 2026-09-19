# Receive handoff regression

Run `python3 tests/rx-input/test-input.py`. The harness extracts the actual `_if_input` and `ml_dequeue` functions. ASan/UBSan check six batch sizes (0,1,2,16,64,1024), ordered exact-once delivery, cleared source list, one flush per nonempty batch and ownership transfer. The stub requires `nextpkt == NULL` and consumes packets immediately; this is an ownership-contract test, not a Darwin performance simulation. Original traversal fails the test.

Apple reference: https://github.com/apple-oss-distributions/IONetworkingFamily/blob/main/IONetworkInterface.cpp (`inputPacket` asserts `mbuf_nextpkt(packet) == 0`).
