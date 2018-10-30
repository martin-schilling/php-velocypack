
mkdir tmp-vpack
pushd tmp-vpack
    git clone https://github.com/arangodb/velocypack.git velocypack
    cd velocypack
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC -std=c++11"
    sudo make install
popd

phpize
./configure
make all -j4
sudo make install

echo "extension=velocypack.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
