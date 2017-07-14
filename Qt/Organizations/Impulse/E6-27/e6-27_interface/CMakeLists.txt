# Project name
PROJECT(e6-27_interface)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENABLE_LANGUAGE(ASM)

# Select your memory sizes here
# Flash size
SET(STM32_FLASH_SIZE "512K")
# RAM size
SET(STM32_RAM_SIZE "64K")
# Stack address - bottom of RAM => RAM origin + RAM size
SET(STM32_STACK_ADDRESS "0x20010000")

# Origins
SET(STM32_FLASH_ORIGIN  "0x08000000")
SET(STM32_RAM_ORIGIN    "0x20000000")

# Compiler definitions for std. periph. library
# Select your chip type here
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER")
SET(CMAKE_ASM_FLAGS  "${CMAKE_ASM_FLAGS}")

# Startup sources
# Select your chip type here
SET(STARTUP_SOURCE ${STM32_STARTUP_HD})

# Uncomment needed modules
SET(MOD_SOURCES
#  ${STM32_MISC_SOURCE}
    ${STM32_ADC_SOURCE}
#  ${STM32_BKP_SOURCE}
#  ${STM32_CAN_SOURCE}
#  ${STM32_CEC_SOURCE}
#  ${STM32_CRC_SOURCE}
#  ${STM32_DAC_SOURCE}
#  ${STM32_DBGMCU_SOURCE}
    ${STM32_DMA_SOURCE}
#  ${STM32_EXTI_SOURCE}
    ${STM32_FLASH_SOURCE}
    ${STM32_FSMC_SOURCE}
    ${STM32_GPIO_SOURCE}
    ${STM32_I2C_SOURCE}
#  ${STM32_IWDG_SOURCE}
#  ${STM32_PWR_SOURCE}
    ${STM32_RCC_SOURCE}
#  ${STM32_RTC_SOURCE}
    ${STM32_SDIO_SOURCE}
#  ${STM32_SPI_SOURCE}
#  ${STM32_TIM_SOURCE}
#  ${STM32_USART_SOURCE}
#  ${STM32_WWDG_SOURCE}
)

# Project sources
SET(PROJECT_SOURCES
  #src/Fat32/iDisk.c
  #src/Fat32/iFAT.c
  #src/Fat32/iFAT32.c
  #src/Fat32/unicodetogb2312.c
  #
  src/sdcard/sdcard.c
  src/touchscreen/touchscreen-hw.c
  src/touchscreen/touchscreen.c
  src/font/big_font.c
  src/font/digits_72.c
  src/font/ldigits_48.c
  src/font/idigits_48.c
  src/font/digits_48.c
  src/font/digits_36.c
  src/font/digits_24.c
  src/font/digits_14.c
  src/i2c/i2c.c
  src/printf.c
  src/lcd/lcd.c
  src/main.c
)

# Default interrupts and system init
SET(SERVICE_SOURCES
  ${STM32_SYSTEM_SOURCE}
)

# Configuring linker script for our target
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/stm32_flash.ld.in ${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld)
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld ${CMAKE_EXE_LINKER_FLAGS}")

INCLUDE_DIRECTORIES(
  #src/Fat32
  ${CMAKE_CURRENT_SOURCE_DIR}/.
  ${STM32_StdPeriphLib_INCLUDE_DIRS}
)

# Compile executable (*.elf)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES} ${MOD_SOURCES} ${STARTUP_SOURCE} ${SERVICE_SOURCES})

# Convert elf to bin and hex
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
