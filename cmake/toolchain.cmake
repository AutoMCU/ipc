# Specify the target system and processor architecture
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Set the C cross-compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)

# Set base compiler flags for ARM ARMV7-m (modify if using a different core)
set(CMAKE_C_FLAGS "-march=armv7-m" CACHE STRING "C Compiler flags")

# Prevent CMake from trying to compile test executables
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Find required cross-compilation utilities
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
find_program(CMAKE_OBJDUMP arm-none-eabi-objdump)
find_program(CMAKE_SIZE arm-none-eabi-size)
