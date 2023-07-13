#!/bin/sh

build_service () {

    echo "building project $1"
    mkdir -p "$1/build"

    cd "$1/build/"

    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

    cd ../../

}

cd ../../

build_service keySupplyInterface
build_service keyRelayInterface
build_service keyStoreManager
