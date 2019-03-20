cmake_minimum_required(VERSION 3.13)
include(ExternalProject)

project(fractal_explorer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

add_executable (fractal_explorer main.cpp)
ExternalProject_Add (
        NanoGUI
        GIT_REPOSITORY "https://github.com/wjakob/nanogui.git"
        GIT_TAG "master"
        UPDATE_COMMAND ""
        INSTALL_COMMAND "" )

target_include_directories (fractal_explorer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (fractal_explorer OpenCL::OpenCL)