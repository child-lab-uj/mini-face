cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(GazeTracking LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Set project root path
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
add_definitions(-DPROJECT_ROOT="${PROJECT_ROOT}")

# Path to vcpkg
# By default, needs to be set manually by the user (as a parameter to 'cmake -G' command) - as in the example below
# set(CMAKE_TOOLCHAIN_FILE "C:/Software/vcpkg/scripts/buildsystems/vcpkg.cmake")
# set(CMAKE_PREFIX_PATH "C:/Software/vcpkg/installed/x64-windows")

find_package(OpenCV REQUIRED)
find_package(OpenBLAS REQUIRED)
find_package(dlib REQUIRED)

# Python bindings
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

# Include files
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/OpenFace/include
)

include_directories(${OpenCV_INCLUDE_DIRS} ${dlib_INCLUDE_DIRS} ${OpenBLAS_INCLUDE_DIRS} ${INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE OPEN_FACE_SRC "${CMAKE_SOURCE_DIR}/lib/OpenFace/src/*.cpp")
file(GLOB_RECURSE API_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

# This one is for creating an .exe
# add_executable(GazeTracking ${OPEN_FACE_SRC} ${API_SRC} main.cpp)

# And this one if for creating python module
pybind11_add_module(GazeTracking ${OPEN_FACE_SRC} ${API_SRC})

target_link_libraries(GazeTracking PRIVATE ${OpenCV_LIBS} dlib::dlib ${OpenBLAS_LIBRARIES})
