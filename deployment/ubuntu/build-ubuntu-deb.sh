#!/usr/bin/env bash
set -e

cd $HOME
go get github.com/jansorg/tom
git clone https://github.com/jansorg/tom-ui
cd tom-ui
[[ -n "$TOM_VERSION" ]] && git checkout "tag/v${TOM_VERSION}"
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" ..
make -j2
cpack -G DEB
ls -la
cp *.deb /dist
