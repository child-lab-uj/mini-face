#!/bin/bash

set -ex

NAME=$1

mkdir -p /opt/$NAME/build
cd /opt/$NAME/build

cmake -GNinja -DBUILD_SHARED_LIBS=TRUE -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DCXXFLAGS="-fPIC" ..
ninja
ninja install

cd -
