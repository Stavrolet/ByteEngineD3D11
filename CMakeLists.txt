# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(ByteEngineD3D11 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/GlobalBuildConfigs.cmake)

add_subdirectory("ByteEngineD3D11")