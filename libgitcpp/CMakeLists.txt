cmake_minimum_required (VERSION 2.8)
project (gitcpp)


set(CMAKE_C_FLAGS_DEBUG            "-g")
set(CMAKE_CXX_FLAGS_DEBUG          "-g -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -pedantic")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
set(GTEST_ROOT "googletest/googletest")

option(TEST "Build all tests." ON)

find_package(Libgit2 REQUIRED)


# Build Release by default
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
        MinSizeRel." FORCE)
  message(STATUS "Build type not specified: cmake build type Release.")
endif()


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-DDEBUG)
  message("Generating DEBUG build")
else()
  add_definitions(-DRELEASE=1)
  add_definitions(-DNDEBUG)
  message("Generating Release build")
endif()

# Find required dependencies
include_directories(src include ${LIBGIT2_INCLUDE_DIR})
message("Libgit Include Dir" ${LIBGIT2_INCLUDE_DIR})

# Collect sourcefiles
file(GLOB SRC_H include/*.h)
file(GLOB SRC_C src/*.cpp)

add_library(gitcpp STATIC ${SRC_H} ${SRC_C})
target_link_libraries(gitcpp ${LIBGIT2_LIBRARIES})


# Tests
if (TEST)
  add_subdirectory(${GTEST_ROOT})
  #enable_testing()

  # Include the gtest library.
  include_directories(${GTEST_ROOT}/include ${GTEST_ROOT})

  # Unit Tests
  file(GLOB SRC_TESTS test/*.cpp)
  add_executable(runUnitTests ${SRC_TESTS})

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(runUnitTests gitcpp)
  add_custom_target(test COMMAND ./runUnitTests)
endif()
