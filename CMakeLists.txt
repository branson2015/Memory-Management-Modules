cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(BIN_DIR     ${PROJECT_SOURCE_DIR}/bin)
set(LIB_DIR     ${PROJECT_SOURCE_DIR}/lib)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

add_subdirectory(${SOURCE_DIR})
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
