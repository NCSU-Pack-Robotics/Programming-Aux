cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)  # C++23 features *are* used
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix SDK versions
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)

# Developing in Pico2 so setting this—change if using a different Pico
set(PICO_DEFAULT_PLATFORM rp2350)
set(PICO_PLATFORM rp2350)
set(PICO_BOARD pico2)

# Pico build system
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICO_SDK_FETCH_FROM_GIT_TAG "2.2.0")


include(pico_sdk_import.cmake)

project(Push_Back_Aux)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(src)
add_subdirectory(dependencies/common)

target_include_directories(Push_Back_Aux_exe PUBLIC
        dependencies/common
)

option(PI "Code will run on a Raspberry Pi" ON)

#target_link_libraries(Push_Back_Aux PRIVATE wiringPi)
