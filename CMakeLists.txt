cmake_minimum_required(VERSION 3.13)

project(kinect-fusion)

set(EIGEN_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/eigen/Eigen" CACHE PATH "Path to the Eigen headers")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${EIGEN_INCLUDE_DIR})
add_executable(kinect-fusion src/main.cpp)