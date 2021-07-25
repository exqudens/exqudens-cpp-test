#!/bin/bash

set -e

rm -rfv "${BASH_HOME}/.conan/data/exqudens-cpp-test-lib/"
rm -rfv "./build/"

PROCESSOR_ARCHITECTURE="x86" \
INCLUDE="${MSVC_INCLUDE}" \
LIBPATH="${MSVC_X86_LIBPATH}" \
LIB="${MSVC_X86_LIB}" \
PATH="${MSVC_X86_BASH_CLPATH}:${MSVC_X86_BASH_RCPATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-msvc-x86-release-shared"

sleep 3s

PROCESSOR_ARCHITECTURE="x86" \
INCLUDE="${MSVC_INCLUDE}" \
LIBPATH="${MSVC_X86_LIBPATH}" \
LIB="${MSVC_X86_LIB}" \
PATH="${MSVC_X86_BASH_CLPATH}:${MSVC_X86_BASH_RCPATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-msvc-x86-release-static"

sleep 3s

PROCESSOR_ARCHITECTURE="AMD64" \
INCLUDE="${MSVC_INCLUDE}" \
LIBPATH="${MSVC_X86_X64_LIBPATH}" \
LIB="${MSVC_X86_X64_LIB}" \
PATH="${MSVC_X86_X64_BASH_CLPATH}:${MSVC_X86_X64_BASH_RCPATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-msvc-x86_x64-release-shared"

sleep 3s

PROCESSOR_ARCHITECTURE="AMD64" \
INCLUDE="${MSVC_INCLUDE}" \
LIBPATH="${MSVC_X86_X64_LIBPATH}" \
LIB="${MSVC_X86_X64_LIB}" \
PATH="${MSVC_X86_X64_BASH_CLPATH}:${MSVC_X86_X64_BASH_RCPATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-msvc-x86_x64-release-static"

sleep 3s

PATH="${GCC_BASH_PATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-gcc-x86_x64-release-shared"

sleep 3s

PATH="${GCC_BASH_PATH}:${PATH}" \
"./cmake-export.sh" "cmd-windows-ninja-gcc-x86_x64-release-static"
