cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)

project(Push_Back_Aux)

if (PI)
    add_definitions(-DPI)
endif ()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(Push_Back_Aux main.cpp
        common/Buffer.cpp
        common/utils.cpp
        common/packet/Packet.cpp
        common/SerialHandler.cpp)

target_include_directories(Push_Back_Aux PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(Push_Back_Aux PRIVATE ${LIBUSB_LIBRARIES})