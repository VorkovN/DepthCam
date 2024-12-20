cmake_minimum_required(VERSION 3.10)
project(3DVision)

set(CMAKE_CXX_STANDARD 20)
set(OrbbecSDK_DIR "/home/nvorkov/Workspace/OrbbecSDK")
find_package(OrbbecSDK REQUIRED)

add_subdirectory(RecordDepth)
add_subdirectory(RecordPointCloud)
add_subdirectory(Show)
