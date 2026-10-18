# Firmware TX-free parsing

`python3 tests/tx-free/test-free.py` extracts the production parser and exercises short headers, PAIR/MSDU sequences, and truncated PAIR lists under ASan/UBSan. The packet stub allocates exactly the received bytes, so a loop reading past the firmware payload is detected. To reproduce the original defect, pass the original HAL source path followed by `baseline`.

A header declaring one MSDU followed by only one PAIR word passes the old initial count check; PAIR increments the loop bound and the next iteration reads beyond the allocation. Check every word independently of the advertised MSDU count. Host malformed-event validation does not establish whether this event occurred on hardware.

`test-unwind.py` injects no-token, DMA-map and ring-full errors plus successful completion into the actual mwx_tx function (400 paths); packet/token/map ownership must return exactly once. `test-node.py` extracts mwx_start and checks management/data node references on success/failure. Completion retains only the mbuf/token and copied descriptor, not the ieee80211_node pointer.
