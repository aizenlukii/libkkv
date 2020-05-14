#!/usr/bin/env bash

mkdir -p build/
cd build/

cmake -DKKV_BUILD_EXAMPLE_TEST=1 ..
make
