#!/usr/bin/env bash

SOURCE="$PWD"
VERSION="$1"
[[ -z "$VERSION" ]] && echo "No version given" && exit -1

echo "Building release $VERSION"
echo -n "$VERSION" > ./version.txt
git commit -m "Prepearing release of $VERSION" version.txt
git push
git tag "v$VERSION"
git push --tags
export TOM_VERSION="$VERSION"

TARGET="$PWD/release-$VERSION"
mkdir -p "$TARGET"

function finish {
    true docker stop tom-ubuntu
}

trap finish EXIT

set -e

function buildUbuntu() {
    local CONTAINER="tom-ubuntu"

    # ubuntu packages
    for name in "$SOURCE"/deployment/ubuntu/*.Dockerfile ; do
        (
            cd "$(dirname "$name")"
            UBUNTU_VERSION="$(basename -s ".Dockerfile" "$name")"
            echo "Building deb for Ubuntu ${UBUNTU_VERSION}"

            DEB_TARGET="$TARGET/$UBUNTU_VERSION"
            mkdir -p "$DEB_TARGET"

            true docker stop "$CONTAINER"

            docker build --rm -t "$CONTAINER" -f "$name" .
            docker run -d -t -i --rm --name "$CONTAINER" -v "$DEB_TARGET:/dist" "$CONTAINER"
            docker exec -e TOM_VERSION "$CONTAINER" bash /root/build-ubuntu-deb.sh

            docker stop "$CONTAINER"

            mv "$DEB_TARGET"/*.deb "$TARGET/tom-$VERSION-$UBUNTU_VERSION.deb"
        )
    done
}

function buildMacOS() {
    echo "Building DMG for macOS Mojave..."
    ssh mojave "source /etc/profile; cd dev; rm -rf tom-ui; git clone https://github.com/jansorg/tom-ui; cd tom-ui; bash TOM_VERSION=$VERSION ./deployment/build-mac-dmg.sh" && scp mojave:dev/tom-ui/build/Tom.dmg "$TARGET"
}

buildUbuntu

buildMacOS
