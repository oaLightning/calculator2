cmake_minimum_required(VERSION 3.3)
project(calculator2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    common.h
    main.c
    SP_Aux.c
    SP_Aux.h
    SP_Stack.c
    SP_Stack.h
    SP_Stack_UnitTest.c)

add_executable(calculator2 ${SOURCE_FILES})