mkdir install
mkdir build
cd build

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=$CONFIGURATION -DTARGET_CPU=$PLATFORM -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_PREFIX_PATH=C:/projects/timle/SFML/install

cmake --build . --config Release
cd ../
