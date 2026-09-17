# CMake 4.x dropped compatibility with cmake_minimum_required below 3.5,
# and googletest 1.17 requires 3.16.
cmake_minimum_required(VERSION 3.16)
project(basic_test CXX)

################################
# Language standard
#
# Set before googletest is configured so the whole build agrees.
# C++20 requires GCC 10+ or Clang 10+.
################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################
# GTest
#
# If a googletest/ directory is sitting next to this file, use it.
# Otherwise download the pinned release. That way the build works both
# on a machine with network access and on one without, as long as the
# directory was shipped or fetched once.
#
# The download is cached in build/_deps, so only the first `cmake ..`
# in a fresh build directory needs the network.
################################
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
    message(STATUS "Using bundled googletest in ./googletest")
    add_subdirectory(googletest)
else()
    message(STATUS "No bundled googletest found; downloading v1.17.0")
    include(FetchContent)

    # DOWNLOAD_EXTRACT_TIMESTAMP was added in CMake 3.24. On older CMake it is
    # not a recognized keyword, so it gets absorbed into the value of the
    # preceding URL_HASH argument and the sub-build dies with a confusing
    # "CMake step for googletest failed: 1". Only pass it where it exists.
    set(gtest_fetch_extra_args "")
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
        list(APPEND gtest_fetch_extra_args DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    endif()

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
        ${gtest_fetch_extra_args}
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

################################
# Bounds-checked STL (recommended)
#
# Turns out-of-range vector/string indexing into an immediate, readable
# abort instead of silent memory corruption. Costs a little speed; these
# tests are far too small for that to matter.
#
# Disable with:  cmake .. -DENABLE_STL_ASSERTIONS=OFF
################################
option(ENABLE_STL_ASSERTIONS "Enable libstdc++ bounds checking" ON)

################################
# Unit Tests
################################
add_executable(runUnitTests src/targetgtest.cpp src/algs.h src/tree.h)
target_link_libraries(runUnitTests PRIVATE GTest::gtest GTest::gtest_main)

if(ENABLE_STL_ASSERTIONS AND NOT MSVC)
    target_compile_definitions(runUnitTests PRIVATE _GLIBCXX_ASSERTIONS)
endif()

add_test(NAME runUnitTests COMMAND runUnitTests)
