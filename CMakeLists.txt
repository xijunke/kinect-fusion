cmake_minimum_required(VERSION 3.13)

project(kinect-fusion)

set(EIGEN_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/eigen/Eigen" CACHE PATH "Path to the Eigen headers")
set(OPENCL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/opencl-headers" CACHE PATH "Path to the OpenCL headers")
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/submodules/glm/glm" CACHE PATH "Path to the GLM headers for OpenGL")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    find_library(OPENCL_LIBRARIES opencl64)
    if(OPENCL_LIBRARIES)
        find_library(OPENCL_LIBRARIES opencl32)
    endif(OPENCL_LIBRARIES)
else()
    set(OPENCL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libOpenCL.so.1")
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories("${EIGEN_INCLUDE_DIR}" "${GLM_INCLUDE_DIR}") 
include_directories("${OPENCL_INCLUDE_DIR}" "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}")
link_libraries(${OPENCL_LIBRARIES} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} glfw freeimage)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_executable(kinect-fusion ${SOURCES})

# Visual Studio properties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT kinect-fusion)
set_property(TARGET kinect-fusion PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)