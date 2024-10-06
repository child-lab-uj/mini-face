# ------------------------- Project & vcpkg setup -------------------------

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_COMPILER $ENV{CMAKE_CXX_COMPILER})
# set(CXX $ENV{CXX})

# vcpkg dependency manager
option(CMAKE_FETCH_VCPKG "Whether to fetch vcpkg if VCPKG_ROOT is not set." ON)

message("[Debug] vcpkg root:  $ENV{VCPKG_ROOT}")

if(DEFINED CMAKE_TOOLCHAIN_FILE) # Print toolchain information
    message(STATUS "Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")
elseif(NOT DEFINED CMAKE_PROJECT_NAME)
    if(DEFINED $ENV{VCPKG_ROOT})
        message(STATUS "Using vcpkg installed in $ENV{VCPKG_ROOT}")
        set(CMAKE_TOOLCHAIN_FILE
            "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH ""
        )
    else()
        message("That fucking shit doesn't work")
    endif()
    # elseif(CMAKE_FETCH_VCPKG)
    #     message(STATUS "Fetching vcpkg")
    #     include(FetchContent)
    #     FetchContent_Declare(vcpkg
    #         GIT_REPOSITORY https://github.com/microsoft/vcpkg/
    #         GIT_TAG 2022.09.27
    #     )
    #     FetchContent_MakeAvailable(vcpkg)
    #     set(CMAKE_TOOLCHAIN_FILE
    #         "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake"
    #         CACHE FILEPATH ""
    #     )
    #     message(STATUS "Fetching vcpkg -- done")
    # else()
    #     message(STATUS
    #         "Not using `vcpkg`. Consider setting CMAKE_FETCH_VCPKG=ON"
    #     )
    # endif()
endif()

# Avoid specifying package name and version in multiple files by extracting
# them from `vcpkg.json` in the repository root.
file(READ ${CMAKE_SOURCE_DIR}/vcpkg.json vcpkg_json)
string(JSON PROJECT_NAME GET ${vcpkg_json} "name")
string(JSON VERSION_STRING GET ${vcpkg_json} "version-semver")
project(${PROJECT_NAME} VERSION ${VERSION_STRING} LANGUAGES CXX)


# ------------------------- Build the project library -------------------------

# Project dependencies
find_package(OpenCV REQUIRED)
find_package(OpenBLAS REQUIRED)
find_package(dlib REQUIRED)

file(GLOB_RECURSE OPEN_FACE_SRC "${CMAKE_SOURCE_DIR}/src/mini-face/OpenFace/src/*.cpp")
file(GLOB_RECURSE API_SRC "${CMAKE_SOURCE_DIR}/src/mini-face/local/*.cpp")
add_library(${PROJECT_NAME}
    ${OPEN_FACE_SRC}
    ${API_SRC}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/mini-face/local
    src/mini-face/OpenFace/include
    ${OpenCV_INCLUDE_DIRS}
    ${dlib_INCLUDE_DIRS}
    ${OpenBLAS_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${OpenCV_LIBS}
    dlib::dlib
    OpenBLAS::OpenBLAS
)


# ----------------------------- Build python api ------------------------------

if(DEFINED SKBUILD)
    message(STATUS "Build Python API")

    message(STATUS "Python executable: ${PYTHON_EXECUTABLE}")

    # prevent an unused variable warning
    set(ignoreMe "${SKBUILD}")

    # call pybind11-config to obtain the root of the cmake package
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pybind11 --cmakedir
        OUTPUT_VARIABLE pybind11_ROOT_RAW)
    string(STRIP "${pybind11_ROOT_RAW}" pybind11_ROOT)
    find_package(pybind11 CONFIG REQUIRED)

    # Take into account that this name should be consistent with the contents
    # of the `__init__.py` file and when calling PYBIND11_MODULE() in the
    # python API code. This could be avoided with some code generation but it
    # would be too complicated for this simple example.
    set(PROJECT_PYTHON_API_NAME api)

    # This function behaves very much like CMake’s builtin `add_library`
    pybind11_add_module(${PROJECT_PYTHON_API_NAME}
        MODULE
        src/python/api.cpp
    )

    target_link_libraries(${PROJECT_PYTHON_API_NAME}
        PRIVATE
        ${PROJECT_NAME}
    )

    target_compile_definitions(${PROJECT_PYTHON_API_NAME}
        PRIVATE
        VERSION_INFO=${PROJECT_VERSION}
    )

    # List `vcpkg` dependencies from VCPKG_INSTALLED_DIR so we can install them
    # together with the python API.
    file(GLOB VCPKG_DEPENDENCIES
        RELATIVE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/"
        "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*"
    )

    # Install the `vcpkg` dependencies that are used in our ${PROJECT_NAME}
    # library in the same DESTINATION as the python API. Note that it
    # only supports collecting the runtime dependencies for Windows, Linux and
    # macOS platforms.
    install(TARGETS ${PROJECT_PYTHON_API_NAME}
        RUNTIME_DEPENDENCIES
        PRE_INCLUDE_REGEXES ${VCPKG_DEPENDENCIES}
        PRE_EXCLUDE_REGEXES ".*"
        DESTINATION .
    )
endif()
