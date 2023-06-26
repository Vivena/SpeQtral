#!/bin/bash

# Install Boost
sudo apt-get update
sudo apt-get install -y libboost-all-dev

# Install yaml-cpp
sudo apt-get install libyaml-cpp-dev

git clone https://github.com/oatpp/oatpp.git oatpp
cd oatpp/
mkdir build && cd build
cmake ..
make install

git clone --depth=1 https://github.com/oatpp/oatpp-starter oatpp-swagger
cd my-oatpp-swagger
mkdir build && cd build/$ cmake ..
make