cmake_minimum_required( VERSION 3.11 )

ADD_LIBRARY( mmm STATIC mmm.cpp )

SET( APP_EXE StaticTest )

ADD_EXECUTABLE( ${APP_EXE} Main.cpp ) 
TARGET_LINK_LIBRARIES( ${APP_EXE} -static-libgcc -static-libstdc++ )
# TARGET_COMPILE_OPTIONS( ${APP_EXE} PUBLIC -static-libgcc -static-libstdc++ )