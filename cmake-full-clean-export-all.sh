#!/bin/bash

# CMD_MSVC_INCLUDE='C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\ATLMFC\include;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\ucrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\shared;C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\winrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\cppwinrt'
# CMD_MSVC_X86_LIBPATH='C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\ATLMFC\lib\x86;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x86;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x86\store\references;C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.19041.0;C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0;C:\Windows\Microsoft.NET\Framework\v4.0.30319'
# CMD_MSVC_X64_LIBPATH='C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\ATLMFC\lib\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x86\store\references;C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.19041.0;C:\Program Files (x86)\Windows Kits\10\References\10.0.19041.0;C:\Windows\Microsoft.NET\Framework64\v4.0.30319'
# CMD_MSVC_X86_LIB='C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\ATLMFC\lib\x86;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x86;C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\ucrt\x86;C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x86'
# CMD_MSVC_X64_LIB='C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\ATLMFC\lib\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30037\lib\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x64'
# BASH_HOME="/c/Users/username"
# BASH_MSVC_X86_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30037/bin/Hostx86/x86:/c/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/x86"
# BASH_MSVC_X64_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30037/bin/Hostx64/x64:/c/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/x64"
# BASH_GCC_PATH="/c/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin"

set -e

rm -rfv "${BASH_HOME}/.conan/data/exqudens-cpp-test-lib/"
rm -rfv "./build/"

# PROCESSOR_ARCHITECTURE="x86" INCLUDE="${CMD_MSVC_INCLUDE}" LIBPATH="${CMD_MSVC_X86_LIBPATH}" LIB="${CMD_MSVC_X86_LIB}" PATH="${BASH_MSVC_X86_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-msvc-release-shared"
# sleep 3s
# PROCESSOR_ARCHITECTURE="x86" INCLUDE="${CMD_MSVC_INCLUDE}" LIBPATH="${CMD_MSVC_X86_LIBPATH}" LIB="${CMD_MSVC_X86_LIB}" PATH="${BASH_MSVC_X86_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-msvc-release-static"
# sleep 3s

# rm -rfv "./build/0"
# rm -rfv "./build/1"

PROCESSOR_ARCHITECTURE="AMD64" INCLUDE="${CMD_MSVC_INCLUDE}" LIBPATH="${CMD_MSVC_X64_LIBPATH}" LIB="${CMD_MSVC_X64_LIB}" PATH="${BASH_MSVC_X64_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-msvc-release-shared"
sleep 3s
PROCESSOR_ARCHITECTURE="AMD64" INCLUDE="${CMD_MSVC_INCLUDE}" LIBPATH="${CMD_MSVC_X64_LIBPATH}" LIB="${CMD_MSVC_X64_LIB}" PATH="${BASH_MSVC_X64_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-msvc-release-static"
sleep 3s

PATH="${BASH_GCC_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-gcc-release-shared"
sleep 3s
PATH="${BASH_GCC_PATH}:${PATH}" "./cmake-export.sh" "cmd-windows-ninja-gcc-release-static"
