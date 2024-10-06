#!/bin/bash

# Only for usage inside manylinux container created by cibuildwheel in CI

set -e

# install tools
yum install -y zip cmake pkgconfig

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
