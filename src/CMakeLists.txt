cmake_minimum_required(VERSION 3.2)

project(MMM)

#set(subdirectories 
#    "example"
#)

set(SOURCE_FILES 
    mmm.cpp
    mmm.hpp
)

#add_subdirectory(${PROJECT_NAME} ${subdirectories})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(LIBRARY_OUTPUT_PATH ${LIB_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)