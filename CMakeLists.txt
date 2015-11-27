cmake_minimum_required(VERSION 2.8.11)

project(GitRelations)

set(CMAKE_C_FLAGS_DEBUG            "-g")
set(CMAKE_CXX_FLAGS_DEBUG          "-g -fsanitize=address")
set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -pedantic")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Libgit2 REQUIRED)

# TODO: Do a find_package
include_directories(/home/tim/Projects/libgitcpp/include ${LIBGIT2_INCLUDE_DIR})
link_directories(${CMAKE_SOURCE_DIR})


file(GLOB SRC_C *.cpp */*.cpp)
file(GLOB SRC_H *.h */*.h)
file(GLOB SRC_UI  *.ui)
set (SRC_BIN main.cpp)

qt5_wrap_cpp(MOC_H ${SRC_H})
qt5_wrap_ui (MOC_UI  ${SRC_UI})

add_library (GitRelationsLib SHARED 
    ${SRC_C}
    ${MOC_H}
    ${MOC_UI}
)

target_link_libraries (GitRelationsLib Qt5::Widgets Qt5::PrintSupport)

add_executable(git_relations ${SRC_BIN})
target_link_libraries (git_relations GitRelationsLib libgitcpp.a ${LIBGIT2_LIBRARIES})
