cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)


# specify cross compilers and tools
#set(CMAKE_C_COMPILER /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++)
#set(CMAKE_ASM_COMPILER /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc)
#set(CMAKE_AR /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-ar)
#set(CMAKE_OBJCOPY /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-objcopy)
#set(CMAKE_OBJDUMP /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-objdump)
#set(SIZE /opt/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-size)

set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_AR /usr/bin/arm-none-eabi-ar)
set(CMAKE_OBJCOPY /usr/bin/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP /usr/bin/arm-none-eabi-objdump)
set(SIZE /usr/bin/arm-none-eabi-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)



# project settings
project(LPC2294 C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#########################
# MCU settings
set(MCPU arm7tdmi-s)
set(MCU_DEVICE "LPC2148")
#########################

#########################
# Flashing settings
#set(LINK_SERVER_EXE "C:/NXP/LinkServer_1.3.15/LinkServer.exe")
set(LINK_SERVER_EXE "/usr/bin/JLinkExe")
set(DEVICE ${MCU_DEVICE})
set(BOARD "LPC845BREAKOUT")
set(SERIAL_PROBE "1701400E")
#########################



#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
add_compile_options(-mfloat-abi=soft)
add_compile_options(-mcpu=${MCPU} -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -g -g3 -ggdb -Og -O0 )

# uncomment to mitigate c++17 absolute addresses warnings
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

#if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#    message(STATUS "Maximum optimization for speed")
#    add_compile_options(-Ofast)
#elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
#    message(STATUS "Maximum optimization for speed, debug info included")
#    add_compile_options(-Ofast -g)
#elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
#    message(STATUS "Maximum optimization for size")
#    add_compile_options(-Os)
#else ()
#    message(STATUS "Minimal optimization, debug info included")
#    add_compile_options(-Og -g)
#endif ()

include_directories(
        ${CMAKE_SOURCE_DIR}/LinkerScript
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/log
        ${CMAKE_SOURCE_DIR}/serial
)

#add_definitions(#[[TODO insert definitions, ex. -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx]])
add_definitions("-DDEBUG -DCODE_RED -D__NEWLIB__ -DCORE_M0PLUS -DMTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE__ -DCPP_USE_HEAP -DLPC84X__ ")
add_definitions("-DROM_RUN")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "./startup/*.S"
        "./src/*.cpp"
        "./src/*.c"
        "./log/*.c"
        "./serial/*.c"
)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/LinkerScript/${MCU_DEVICE}_flash.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb-interwork --specs=nosys.specs)
add_link_options(-g -ggdb -g3)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

#target_compile_definitions(${PROJECT_NAME}.elf PRIVATE #IDK which CPU it has, will set it to all
#        CPU_LPC845M301JBD48
#        CPU_LPC845M301JBD64
#        CPU_LPC845M301JHI33
#        CPU_LPC845M301JHI48
#)

set(HEX_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin)



add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

#add_custom_target(FLASH DEPENDS ${PROJECT_NAME}.elf
#        COMMAND ${LINK_SERVER_EXE} -CommandFile ${CMAKE_SOURCE_DIR}/JLink/cmd.jlink)