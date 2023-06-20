cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pcf8575testProject)
add_subdirectory(PCF8575)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# rest of your project
add_executable(pcf8575test
    test.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(pcf8575test pico_stdlib hardware_i2c PCF8575)
target_link_libraries(PCF8575 pico_stdlib hardware_i2c)

# enable usb output, disable uart output
pico_enable_stdio_usb(pcf8575test 1)
pico_enable_stdio_uart(pcf8575test 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pcf8575test)