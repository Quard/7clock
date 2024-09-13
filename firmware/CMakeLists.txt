cmake_minimum_required(VERSION 3.20)

set(MCU   attiny414)
set(F_CPU 2000000)

add_definitions(-DF_CPU=${F_CPU})

# program names
set(AVRCPP     avr-g++)
set(AVRC       avr-gcc)
set(AVRSTRIP   avr-strip)
set(OBJCOPY    avr-objcopy)
set(OBJDUMP    avr-objdump)
set(AVRSIZE    avr-size)
set(AVRDUDE    avrdude)
set(PROG_TYPE  serialupdi)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

project("7clock" C CXX ASM)

#add_compile_options(-mmcu=attiny414 -B /opt/microchip/ATtiny_DFP/3.1.260/gcc/dev/attiny414 -isystem /opt/microchip/ATtiny_DFP/3.1.260/include/)
# Compiler flags
set(APACK_PATH "/opt/microchip/ATtiny_DFP/3.1.260")

set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(APACK_BIN "-B ${APACK_PATH}/gcc/dev/${MCU}")
set(APACK_INC "-isystem ${APACK_PATH}/include")

set(CFLAGS   "${CMCU} ${CDEBUG} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING} ${APACK_BIN} ${APACK_INC}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${COPT} ${CTUNING} ${APACK_BIN} ${APACK_INC}")
set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")
#add_link_options("${CMCU}")

include_directories("src/")
set(SRC_FILES
    "src/display.c"
    "src/rtc.c"
    "src/system.c"
    "src/ambient.c"
    "src/state.c"
    "src/peripherals/twi.c"
    "src/main.c")
add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)

add_custom_target(flash ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
