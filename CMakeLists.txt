# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(jogo_labirinto C CXX ASM)

pico_sdk_init()

add_executable(jogo_labirinto
    src/main.c
    inc/display_oled/ssd1306_i2c.c
    inc/display_oled/display_oled.c
    inc/logica_labirinto/logica_labirinto.c
    inc/matriz_LED/matriz_LED.c
    inc/mpu6050/mpu6050.c
)

pico_set_program_name(jogo_labirinto "jogo_labirinto")
pico_set_program_version(jogo_labirinto "0.1")

pico_generate_pio_header(jogo_labirinto ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

pico_enable_stdio_uart(jogo_labirinto 0)
pico_enable_stdio_usb(jogo_labirinto 1)

target_link_libraries(jogo_labirinto
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_clocks
)

target_include_directories(jogo_labirinto PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(jogo_labirinto)