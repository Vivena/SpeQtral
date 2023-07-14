#!/bin/sh

build_service () {

    echo "building project $1"
    rm -rf ./$1/build
    mkdir -p "$1/build"

    cd "$1/build/"

    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

    cd ../../

}

build_service util
build_service keyStore
build_service qkdModuleInterface
build_service keyStoreManager
build_service keySupplyInterface


rm -rf monolith/all-services/build/
mkdir -p monolith/all-services/build/
cd monolith/all-services/build/

cmake -DCMAKE_BUILD_TYPE=Release ..
make

cd ../../../