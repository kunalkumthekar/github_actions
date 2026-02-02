set -e

#######################
# Build
#######################
echo -e "Building BIZ....."
cd ./
if [ ! -d build ]; then
    mkdir build
fi
cd ./build/
cmake ..
make
cd ..
echo -e "Build Successful....."

#######################
# Launch
#######################
echo -e "Launching Test Cases........"
./build/BIZ_test