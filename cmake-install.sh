#!/bin/bash

SCRIPT_CMAKE_PROGRAM="${CMAKE_PROGRAM}"
SCRIPT_CONAN_PROGRAM="${CONAN_PROGRAM}"

SCRIPT_MSVC_RUNTIME_LIBRARY='MultiThreadedDLL'
SCRIPT_BUILD_TYPE='Release'
SCRIPT_GENERATOR='Visual Studio 16 2019'

"${SCRIPT_CMAKE_PROGRAM}" -DCMAKE_MSVC_RUNTIME_LIBRARY:STRING="${SCRIPT_MSVC_RUNTIME_LIBRARY}" -DCMAKE_BUILD_TYPE:STRING="${SCRIPT_BUILD_TYPE}" -DCMAKE_CONAN_PROGRAM:FILEPATH="${SCRIPT_CONAN_PROGRAM}" -G "${SCRIPT_GENERATOR}" -S "." -B "build" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "library-conan-install" &&\
"${SCRIPT_CMAKE_PROGRAM}" "build" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "install" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "library-conan-export-testing" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "test-conan-install" &&\
"${SCRIPT_CMAKE_PROGRAM}" "build" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "integration-tests-interface-executable" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "integration-tests" &&\
"${SCRIPT_CMAKE_PROGRAM}" --build "build" --target "library-conan-export" &&\
(tput setaf 2 && echo 'BUILD_SUCCESSFUL') ||\
(tput setaf 1 && echo 'BUILD_FAILED')
