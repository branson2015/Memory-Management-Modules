cmake_minimum_required(VERSION 3.2)

project(test)

include_directories(${SOURCE_DIR})

set(SOURCE_FILES main.cpp ../src/mmm.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} example)

set (EXECUTABLE_OUTPUT_PATH ${BIN_DIR})
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Debug)

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_DIR})