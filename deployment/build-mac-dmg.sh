#!/usr/bin/env bash
set -e

FLAGS="$1"

git pull
[[ -n "$TOM_VERSION" ]] && git checkout "v${TOM_VERSION}"

(
    rm -rf "$HOME/tom/"
    git clone "https://github.com/jansorg/tom.git" "$HOME/tom"
    cd "$HOME/tom/"
    git pull
    [[ -n "$TOM_VERSION" ]] && git checkout "v${TOM_VERSION}"
    go build -o tom -ldflags "-s -w" .
)

QTDIR="$HOME/Qt/5.15.2/clang_64"

rm -rf build
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="$QTDIR" -DCMAKE_BUILD_TYPE="Release" "$FLAGS" ..
make -j4

mv tom-ui.app Tom.app
cp "$HOME/tom/tom" Tom.app/Contents/MacOS/tom
install_name_tool -add_rpath "@executable_path/../Frameworks" Tom.app/Contents/MacOS/tom-ui
"$QTDIR/bin/macdeployqt" Tom.app -dmg -timestamp -sign-for-notarization=