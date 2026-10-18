# Associated firmware context tests

Run from the repository root with Python 3 and clang++ (Apple Command Line Tools):

```sh
python3 tests/association/peer-test.py
python3 tests/association/bss-context-test.py
```

The tests extract the actual constructors and wire structures from the driver. Kernel allocation and MCU transport are mocked. ASan and UBSan check the host harness; this does not simulate the device or validate firmware behavior. Generated sources, executables and result files stay in this directory and are ignored by Git.
