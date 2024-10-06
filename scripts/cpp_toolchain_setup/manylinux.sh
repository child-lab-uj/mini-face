#!/bin/bash

# Only for usage inside manylinux container created by cibuildwheel in CI

set -e

# install tools
yum install -y zip cmake pkgconfig

echo "Available compilers:"
whereis g++
whereis c++
whereis gcc
whereis clang
echo ""

# explicitly set C++ compiler for the fucking CMake
export CMAKE_CXX_COMPILER=$(type -aP g++)

echo "C++ Compiler: $CMAKE_CXX_COMPILER"

# install vcpkg :skull:
git clone https://github.com/microsoft/vcpkg.git
sh ./vcpkg/bootstrap-vcpkg.sh

# expose vcpkg
export VCPKG_ROOT=$(pwd)/vcpkg
export PATH=$PATH:$VCPKG_ROOT

# check if it works properly
echo "[Debug info]:"
whereis vcpkg
vcpkg --version

echo "Successfully installed C++ toolchain!"
