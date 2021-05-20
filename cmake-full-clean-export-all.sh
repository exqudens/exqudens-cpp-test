#!/bin/bash

rm -rfv "${USERPROFILE}/.conan/data/exqudens-cpp-test-lib/" &&\
rm -rfv "./build/" &&\
"./cmake-export.sh" "msvc" "shared" &&\
sleep 3s &&\
"./cmake-export.sh" "msvc" "static" &&\
sleep 3s &&\
"./cmake-export.sh" "gcc" "shared" &&\
sleep 3s &&\
"./cmake-export.sh" "gcc" "static"
