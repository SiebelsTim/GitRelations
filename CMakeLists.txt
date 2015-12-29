cmake_minimum_required(VERSION 2.8.11)

project(GitRelations)

set(CMAKE_C_FLAGS_DEBUG            "-g")
set(CMAKE_CXX_FLAGS_DEBUG          "-g -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra -pedantic")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

add_definitions(-DHAVE_STRING_H)

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Libgit2 REQUIRED)
find_package(Graphviz REQUIRED)
find_package(Libgitcpp REQUIRED)

link_directories(${CMAKE_SOURCE_DIR} ${lib_graphviz})


file(GLOB SRC_C *.cpp */*.cpp)
file(GLOB SRC_H *.h */*.h)
file(GLOB SRC_UI  *.ui)
set (SRC_BIN main.cpp)

qt5_wrap_cpp(MOC_H ${SRC_H})
qt5_wrap_ui (MOC_UI  ${SRC_UI})

add_library (GitRelationsLib STATIC 
    ${SRC_C}
    ${MOC_H}
    ${MOC_UI}
)

target_link_libraries (GitRelationsLib Qt5::Widgets Qt5::PrintSupport)

add_executable(git_relations ${SRC_BIN})
target_link_libraries (git_relations GitRelationsLib libgitcpp.a ${LIBGIT2_LIBRARIES} cgraph gvc)
