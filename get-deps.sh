#!/bin/bash

set -e

if command -v dnf &> /dev/null; then
    dnf -y install gcc g++ libstdc++-static.i686 glibc-static.i686 libstdc++-static.x86_64 mingw64-gcc mingw32-gcc  mingw32-gcc-c++ mingw64-gcc-c++
elif command -v apt &> /dev/null; then
    sudo apt install --yes build-essential gcc-multilib mingw-w64-tools
else
    echo "Package manager is not apt or dnf, please add support for it!"
    exit 1
fi