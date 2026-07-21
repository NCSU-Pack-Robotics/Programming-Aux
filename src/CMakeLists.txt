add_executable(Push_Back_Aux_exe
        main.cpp
        comms/I2C.hpp
        comms/I2C.cpp
        comms/PiPicoComm.hpp
        comms/PiPicoComm.cpp
        devices/PAA5160E1.hpp
        devices/PAA5160E1.hpp
        devices/PAA5160E1.cpp
        devices/PAA5160E1.cpp
        config.hpp
)

target_link_libraries(Push_Back_Aux_exe
        pico_stdlib
        hardware_i2c
        Programming_Push_Back_Common_lib
)

pico_enable_stdio_usb(Push_Back_Aux_exe 1)
pico_enable_stdio_uart(Push_Back_Aux_exe 0)

target_compile_options(Push_Back_Aux_exe PRIVATE
        -Wno-psabi
)

target_compile_definitions(Push_Back_Aux_exe PRIVATE
        PICO_STDIO_DEFAULT_CRLF=0
        PICO_FLASH_SIZE_BYTES=4194304
)

# Create pico specific output files (e.g. .uf2)
pico_add_extra_outputs(Push_Back_Aux_exe)

# Copy extra outputs to src/build_artifacts
list(APPEND OUTPUT_EXTS bin dis elf elf.map hex uf2)
set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_artifacts)
add_custom_command(TARGET Push_Back_Aux_exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_PATH})
foreach (EXT IN LISTS OUTPUT_EXTS)
    add_custom_command(TARGET Push_Back_Aux_exe POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_BASE_NAME:Push_Back_Aux_exe>.${EXT} ${OUTPUT_PATH}
    )
endforeach ()
