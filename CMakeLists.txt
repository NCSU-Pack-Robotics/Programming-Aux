cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)

project(Push_Back_Aux)

add_executable(Push_Back_Aux main.cpp
        common/packet/Packet.cpp
        common/Buffer.cpp
        common/utils.cpp
        common/
        common/SerialHandler.cpp)