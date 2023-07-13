# Install Boost
brew install boost

# Install yaml-cpp
brew install yaml-cpp

git clone https://github.com/oatpp/oatpp.git oatpp
cd oatpp/
mkdir build && cd build
cmake ..
make install

cd ../..


git clone --depth=1 https://github.com/oatpp/oatpp-starter oatpp-swagger
cd oatpp-swagger
mkdir build && cd build
cmake ..
make