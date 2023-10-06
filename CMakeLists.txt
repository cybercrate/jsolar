cmake_minimum_required(VERSION 3.20)

if(WIN32)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOMEPATH}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(jsolar)

if (MSVC)
    set(CMAKE_CXX_FLAGS "/std:c++20")
else()
    set(CMAKE_CXX_FLAGS "-std=c++20")
endif()

set(JSOLAR_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")

set(PROJECT_INCLUDE_DIRS ${JSOLAR_INCLUDE_PATH})

find_package(GTest CONFIG REQUIRED)

add_subdirectory(test)
