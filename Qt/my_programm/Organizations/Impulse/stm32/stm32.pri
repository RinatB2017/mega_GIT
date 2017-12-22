
TOOLCHAIN_DIR   = /usr
TOOLCHAIN_BIN_DIR   = $$TOOLCHAIN_DIR/bin
TOOLCHAIN_LIBC_DIR  = $$TOOLCHAIN_DIR/arm-none-linux-gnueabi/lib
TOOLCHAIN_INC_DIR   = $$TOOLCHAIN_DIR/arm-none-linux-gnueabi/usr/include

STM32_StdPeriphLib_DIR = /opt/STM32F10x_StdPeriph_Lib_V3.5.0

INCLUDEPATH += \
    $$STM32_StdPeriphLib_DIR/Libraries/CMSIS/CM3/CoreSupport/ \
    $$STM32_StdPeriphLib_DIR/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
    $$STM32_StdPeriphLib_DIR/Libraries/STM32F10x_StdPeriph_Driver/inc/ \
    $$STM32_StdPeriphLib_DIR/Libraries/STM32F10x_StdPeriph_Driver/src/
DEPENDPATH = $$INCLUDEPATH

include(conf.pri)

QMAKE_CC        = arm-none-linux-gnueabi-gcc
QMAKE_CXX       = arm-none-linux-gnueabi-g++
QMAKE_LINK      = arm-none-linux-gnueabi-g++
QMAKE_AR        = arm-none-linux-gnueabi-ar
#cqs
QMAKE_AS        = arm-none-linux-gnueabi-as

QMAKE_OBJCOPY   = arm-none-linux-gnueabi-objcopy
QMAKE_OBJDUMP   = arm-none-linux-gnueabi-objdump
QMAKE_STRIP     = arm-none-linux-gnueabi-strip

QMAKE_CFLAGS    += -isystem $$TOOLCHAIN_INC_DIR -mthumb -mcpu=cortex-m3 -fno-builtin -Wall -std=gnu99
QMAKE_CXXFLAGS  += -isystem $$TOOLCHAIN_INC_DIR -mthumb -mcpu=cortex-m3 -fno-builtin -Wall
QMAKE_CFLAGS    += -Os

# Flash size
STM32_FLASH_SIZE    = "128K"
# RAM size
STM32_RAM_SIZE      = "8K"
# Stack address - bottom of RAM => RAM origin + RAM size
STM32_STACK_ADDRESS = "0x20002000"
# не забыть сконфигурировать stm32_flash.ld //???

QMAKE_LFLAGS    += -Tsrc/stm32_flash.ld
QMAKE_LFLAGS    += -nostartfiles -Wl,-Map -Wl,main.map -mthumb -mcpu=cortex-m3

QMAKE_CFLAGS    += -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER
QMAKE_CXXFLAGS  += -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER

QMAKE_ASM_FLAGS += -mthumb -mcpu=cortex-m3

#SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}/arm-none-linux-gnueabi-as CACHE INTERNAL "")
#SET(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3" CACHE INTERNAL "asm compiler flags")

#/usr/bin/arm-none-linux-gnueabi-objcopy -Oihex stm32template.elf stm32template.hex
#/usr/bin/arm-none-linux-gnueabi-objcopy -Obinary stm32template.elf stm32template.bin

#hex.input   = $$TARGET.elf
#hex.output  = $$TARGET.hex
#hex.commands = -Oihex $$TARGET.elf $$TARGET.hex
#QMAKE_EXTRA_COMPILERS += hex

#hex.input   = $$TARGET.elf
#hex.output  = $$TARGET.bin
#hex.commands = -Obinary ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
#QMAKE_EXTRA_COMPILERS += hex
