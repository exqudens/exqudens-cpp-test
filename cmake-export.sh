#!/bin/bash

SCRIPT_CMAKE_PROGRAM="${CMAKE_PROGRAM}"
SCRIPT_CONAN_PROGRAM="${CONAN_PROGRAM}"

SCRIPT_MSVC_RUNTIME_LIBRARY='MultiThreadedDLL'
SCRIPT_BUILD_TYPE='Release'
if [ "msvc" == "${1}" ]
then
  SCRIPT_GENERATOR='Visual Studio 16 2019'
elif [ "gcc" == "${1}" ]
then
  SCRIPT_GENERATOR='CodeBlocks - MinGW Makefiles'
else
  echo "Unsupported GENERATOR: '${1}'" && exit 1
fi
if [ "shared" == "${2}" ]
then
  SCRIPT_BUILD_SHARED_LIBS='true'
elif [ "static" == "${2}" ]
then
  SCRIPT_BUILD_SHARED_LIBS='false'
else
  echo "Unsupported BUILD_SHARED_LIBS: '${2}'" && exit 1
fi

"${SCRIPT_CMAKE_PROGRAM}" -DBUILD_SHARED_LIBS:BOOL="${SCRIPT_BUILD_SHARED_LIBS}" -DCMAKE_MSVC_RUNTIME_LIBRARY:STRING="${SCRIPT_MSVC_RUNTIME_LIBRARY}" -DCMAKE_BUILD_TYPE:STRING="${SCRIPT_BUILD_TYPE}" -DCONAN_PROGRAM:FILEPATH="${SCRIPT_CONAN_PROGRAM}" -G "${SCRIPT_GENERATOR}" -S "exqudens-cpp-test-lib" -B "build/${1}-${2}/exqudens-cpp-test-lib" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build/${1}-${2}/exqudens-cpp-test-lib" --target "exqudens-cpp-test-lib-cmake-install" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build/${1}-${2}/exqudens-cpp-test-lib" --target "exqudens-cpp-test-lib-conan-export-testing" &&\
"${SCRIPT_CMAKE_PROGRAM}" -DBUILD_SHARED_LIBS:BOOL="${SCRIPT_BUILD_SHARED_LIBS}" -DCMAKE_MSVC_RUNTIME_LIBRARY:STRING="${SCRIPT_MSVC_RUNTIME_LIBRARY}" -DCMAKE_BUILD_TYPE:STRING="${SCRIPT_BUILD_TYPE}" -DCONAN_PROGRAM:FILEPATH="${SCRIPT_CONAN_PROGRAM}" -G "${SCRIPT_GENERATOR}" -S "exqudens-cpp-test-test" -B "build/${1}-${2}/exqudens-cpp-test-test" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build/${1}-${2}/exqudens-cpp-test-test" --target "exqudens-cpp-test-test" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build/${1}-${2}/exqudens-cpp-test-lib" --target "exqudens-cpp-test-lib-conan-export" &&\
echo 'BUILD_SUCCESSFUL' ||\
(echo 'BUILD_FAILED' && false)
