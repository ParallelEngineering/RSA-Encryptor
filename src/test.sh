#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
FETCH_DIR="$BUILD_DIR/_deps"

echo "Cleaning only FetchContent deps to avoid stale dependencies"
# Remove only the FetchContent directory, not the entire build
rm -rf "$FETCH_DIR"

echo "Configuring (Debug, tests ON, Clang, Unix Makefiles)"
cmake -S . -B "$BUILD_DIR" -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_EXTENSIONS=OFF \
  -DFETCHCONTENT_UPDATES_DISCONNECTED=ON

echo "Building the executable"
cmake --build "$BUILD_DIR" --target RSA-Encryptor-Tests -- -j"$(nproc)"

echo "Running tests"
ctest --test-dir "$BUILD_DIR" --output-on-failure
