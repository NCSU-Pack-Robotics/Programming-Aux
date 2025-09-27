cmake_minimum_required(VERSION 3.22.1)

project(Push_Back_Aux)

add_executable(Push_Back_Aux main.cpp
        common/COBS.cpp
        common/SerialHandler.cpp)