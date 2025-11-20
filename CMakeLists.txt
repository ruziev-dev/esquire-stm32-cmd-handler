cmake_minimum_required(VERSION 3.22)

project(esquire_stm32_cmd_handler)
add_library(esquire_stm32_cmd_handler INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)


target_include_directories(esquire_stm32_cmd_handler INTERFACE
    ./
)

target_sources(esquire_stm32_cmd_handler INTERFACE
    ./esquire_cmd_handler.c
    ./esquire_message_handler.c
)

if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


