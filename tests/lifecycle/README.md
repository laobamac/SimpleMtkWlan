# Driver lifecycle and API tests

Run from the repository root with Python 3 and clang++:

```sh
python3 tests/lifecycle/test-queue.py
python3 tests/lifecycle/test-detach.py
python3 tests/lifecycle/test-basics.py
sh tests/scan/run.sh
```

The tests extract the actual implementation and use host stubs with ASan/UBSan. They cover queue draining, cancellation and membership; partial-attach teardown ordering; polling delays; selector bounds; and poisoned scan-request buffers. They do not prove hardware unload/reload or Darwin kernel scheduling.
