cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(GazeTracking LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Path to vcpkg
# Needs to be set manually
set(CMAKE_TOOLCHAIN_FILE "C:/Software/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/Software/vcpkg/installed/x64-windows")


find_package(OpenCV REQUIRED)
find_package(OpenBLAS REQUIRED)
find_package(dlib REQUIRED)

# Include files
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib/OpenFace/include
)

include_directories(${OpenCV_INCLUDE_DIRS} ${dlib_INCLUDE_DIRS} ${OpenBLAS_INCLUDE_DIRS} ${INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE OPEN_FACE_SRC "${CMAKE_SOURCE_DIR}/lib/OpenFace/src/*.cpp")
file(GLOB_RECURSE API_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(GazeTracking ${OPEN_FACE_SRC} ${API_SRC} main.cpp)

target_link_libraries(GazeTracking ${OpenCV_LIBS} dlib::dlib ${OpenBLAS_LIBRARIES})