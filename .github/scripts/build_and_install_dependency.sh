#!/bin/bash

set -ex

NAME=$1

mkdir -p /opt/$NAME/build
cd /opt/$NAME/build

cmake -GNinja ..
ninja
ninja install

cd -
