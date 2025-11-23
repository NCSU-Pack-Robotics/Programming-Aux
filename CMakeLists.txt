cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)

project(Push_Back_Aux)

add_definitions(-DPI)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(Push_Back_Aux
        src/main.cpp)

add_subdirectory(dependencies/common)

target_link_libraries(Push_Back_Aux PRIVATE Programming_Push_Back_Common_lib)

target_link_libraries(Push_Back_Aux PRIVATE wiringPi)