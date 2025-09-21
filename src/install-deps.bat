@echo off

mkdir lib
cd lib

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

call bootstrap-vcpkg.bat

.\vcpkg.exe install fftw3 sfml --head

cd ..\..
