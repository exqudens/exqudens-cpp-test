#!/bin/bash

set -e

cmake -S "." --preset "${1}"
cmake --build --preset "${1}" --target "exqudens-cpp-test-lib"
cmake --build --preset "${1}" --target "exqudens-cpp-test-test"
cmake --build --preset "${1}" --target "exqudens-cpp-test-lib-cmake-install"
cmake --build --preset "${1}" --target "exqudens-cpp-test-lib-conan-export-testing"
cmake --build --preset "${1}" --target "exqudens-cpp-test-package-test-conan-install"
cmake --build --preset "${1}" --target "rebuild_cache"
cmake --build --preset "${1}" --target "exqudens-cpp-test-package-test"
cmake --build --preset "${1}" --target "exqudens-cpp-test-lib-conan-export-testing-clean"
cmake --build --preset "${1}" --target "exqudens-cpp-test-lib-conan-export"
echo 'BUILD_SUCCESSFUL' || (echo 'BUILD_FAILED' && false)
