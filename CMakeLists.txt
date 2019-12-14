cmake_minimum_required(VERSION 3.13)

project(kinect-fusion)

set(EIGEN_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/eigen/Eigen" CACHE PATH "Path to the Eigen headers")
set(OPENCL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/opencl-headers" CACHE PATH "Path to the OpenCL headers")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    find_library(OPENCL_LIBRARIES opencl64)
    if(OPENCL_LIBRARIES)
        find_library(OPENCL_LIBRARIES opencl32)
    endif(OPENCL_LIBRARIES)
else()
    set(OPENCL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libOpenCL.so.1")
endif()

include_directories(${EIGEN_INCLUDE_DIR} ${OPENCL_INCLUDE_DIR})

add_executable(kinect-fusion src/main.cpp)
target_link_libraries(kinect-fusion ${OPENCL_LIBRARIES})