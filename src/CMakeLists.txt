cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "./lib/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")


project(AudioAnalyzer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(FFTW3 REQUIRED)

add_executable(audioanalyzer main.cpp)

target_link_libraries(audioanalyzer PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System FFTW3::fftw3)
