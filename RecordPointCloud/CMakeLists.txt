cmake_minimum_required(VERSION 3.10)
project(RecordPointCloud)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        OrbbecSDK::OrbbecSDK
        ${OpenCV_LIBS}
)