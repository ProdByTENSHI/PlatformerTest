@echo off
cls
cd build/debug
cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ ../../
cmake --build .
Platformer.exe
cd ../../
