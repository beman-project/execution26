# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.23)
project(execution
    VERSION 1.0.0
    DESCRIPTION "std::execution for the Beman Project"
    LANGUAGES CXX
)
include(CTest)

set(CMAKE_CXX_STANDARD 23)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src/examples)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()
