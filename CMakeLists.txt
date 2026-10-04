cmake_minimum_required(VERSION 3.20)

# Allow overriding the toolchain via command line argument
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
endif()

project(ipc VERSION 1.0.0 LANGUAGES C)

# Set C11 standard requirement
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDE support (e.g., clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define a static library target
add_library(ipc STATIC)

# Expose public header files to consumers of this library
target_include_directories(ipc PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

# Set private compiler options and warning flags
target_compile_options(ipc PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
    -ffunction-sections
    -fdata-sections
)

# Include module-specific source files
include(${CMAKE_SOURCE_DIR}/cmake/ipc.cmake)
