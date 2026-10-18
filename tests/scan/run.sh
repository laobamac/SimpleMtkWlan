#!/bin/sh
set -eu
cd "$(dirname "$0")"
ruby generate.rb ../.. > scan.cpp
clang++ -std=c++14 -fsanitize=address,undefined scan.cpp -o scan-test
./scan-test
