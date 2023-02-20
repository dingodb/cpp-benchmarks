#!/bin/bash

set -x 

# BUILD_TYPE=Debug
BUILD_TYPE="${1:-Release}"

cmake --preset ninja-multiconfiguration-vcpkg && \
  cmake --build ./builds/ninja-multiconfiguration-vcpkg/ --config $BUILD_TYPE

