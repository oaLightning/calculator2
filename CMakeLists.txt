cmake_minimum_required(VERSION 3.3)
project(calculator2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic-errors")

set(SOURCE_FILES
    common.h
    main.c
    SP_Aux.c
    SP_Aux.h
    SP_Stack.c
    SP_Stack.h
    SP_Stack_UnitTest.c)

add_executable(calculator2 ${SOURCE_FILES})