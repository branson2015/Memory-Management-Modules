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

#target_link_libraries(${PROJECT_NAME} ${subdirectories})

#install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_DIR})
