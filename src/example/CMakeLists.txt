cmake_minimum_required(VERSION 3.2)

set(proj "example")

project(${proj} C CXX)

#   set(SOURCE_FILES example.h example.cpp)
set(SOURCE_FILES "")

add_library(${proj} SHARED STATIC ${SOURCE_FILES})

install(TARGETS ${proj} DESTINATION ${LIB_DIR})
install(FILES ${proj}.h DESTINATION ${INCLUDE_DIR})