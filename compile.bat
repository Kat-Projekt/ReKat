echo off

cmake . -B ../Build -G "MinGW Makefiles"

cd ../Build

mingw32-make

cd ReKat

echo ended compilation (msg from compile)
echo running (msg from compile)

.\ReKat carlo a

cd ../../