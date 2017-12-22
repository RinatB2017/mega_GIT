
#DEFINES += STM32_MISC_SOURCE
#DEFINES += STM32_ADC_SOURCE
#DEFINES += STM32_BKP_SOURCE
#DEFINES += STM32_CAN_SOURCE
#DEFINES += STM32_CEC_SOURCE
#DEFINES += STM32_CRC_SOURCE
#DEFINES += STM32_DAC_SOURCE
#DEFINES += STM32_DBGMCU_SOURCE
#DEFINES += STM32_DMA_SOURCE
#DEFINES += STM32_EXTI_SOURCE
#DEFINES += STM32_FLASH_SOURCE
#DEFINES += STM32_FSMC_SOURCE
DEFINES += STM32_GPIO_SOURCE
#DEFINES += STM32_I2C_SOURCE
#DEFINES += STM32_IWDG_SOURCE
#DEFINES += STM32_PWR_SOURCE
DEFINES += STM32_RCC_SOURCE
#DEFINES += STM32_RTC_SOURCE
#DEFINES += STM32_SDIO_SOURCE
#DEFINES += STM32_SPI_SOURCE
#DEFINES += STM32_TIM_SOURCE
DEFINES += STM32_USART_SOURCE
#DEFINES += STM32_WWDG_SOURCE

contains(DEFINES, STM32_MISC_SOURCE) {
    HEADERS += misc.h
    SOURCES += misc.c
}
contains(DEFINES, STM32_ADC_SOURCE) {
    HEADERS += stm32f10x_adc.h
    SOURCES += stm32f10x_adc.c
}
contains(DEFINES, STM32_BKP_SOURCE) {
    HEADERS += stm32f10x_bkp.h
    SOURCES += stm32f10x_bkp.c
}
contains(DEFINES, STM32_CAN_SOURCE) {
    HEADERS += stm32f10x_can.h
    SOURCES += stm32f10x_can.c
}
contains(DEFINES, STM32_CEC_SOURCE) {
    HEADERS += stm32f10x_cec.h
    SOURCES += stm32f10x_cec.c
}
contains(DEFINES, STM32_CRC_SOURCE) {
    HEADERS += stm32f10x_crc.h
    SOURCES += stm32f10x_crc.c
}
contains(DEFINES, STM32_DAC_SOURCE) {
    HEADERS += stm32f10x_dac.h
    SOURCES += stm32f10x_dac.c
}
contains(DEFINES, STM32_DBGMCU_SOURCE) {
    HEADERS += stm32f10x_dbgmcu.h
    SOURCES += stm32f10x_dbgmcu.c
}
contains(DEFINES, STM32_DMA_SOURCE) {
    HEADERS += stm32f10x_dma.h
    SOURCES += stm32f10x_dma.c
}
contains(DEFINES, STM32_EXTI_SOURCE) {
    HEADERS += stm32f10x_exti.h
    SOURCES += stm32f10x_exti.c
}
contains(DEFINES, STM32_FLASH_SOURCE) {
    HEADERS += stm32f10x_flash.h
    SOURCES += stm32f10x_flash.c
}
contains(DEFINES, STM32_FSMC_SOURCE) {
    HEADERS += stm32f10x_fsmc.h
    SOURCES += stm32f10x_fsmc.c
}
contains(DEFINES, STM32_GPIO_SOURCE) {
    HEADERS += stm32f10x_gpio.h
    SOURCES += stm32f10x_gpio.c
}
contains(DEFINES, STM32_I2C_SOURCE) {
    HEADERS += stm32f10x_i2c.h
    SOURCES += stm32f10x_i2c.c
}
contains(DEFINES, STM32_IWDG_SOURCE) {
    HEADERS += stm32f10x_iwdg.h
    SOURCES += stm32f10x_iwdg.c
}
contains(DEFINES, STM32_PWR_SOURCE) {
    HEADERS += stm32f10x_pwr.h
    SOURCES += stm32f10x_pwr.c
}
contains(DEFINES, STM32_RCC_SOURCE) {
    HEADERS += stm32f10x_rcc.h
    SOURCES += stm32f10x_rcc.c
}
contains(DEFINES, STM32_RTC_SOURCE) {
    HEADERS += stm32f10x_rtc.h
    SOURCES += stm32f10x_rtc.c
}
contains(DEFINES, STM32_SDIO_SOURCE) {
    HEADERS += stm32f10x_sdio.h
    SOURCES += stm32f10x_sdio.c
}
contains(DEFINES, STM32_SPI_SOURCE) {
    HEADERS += stm32f10x_spi.h
    SOURCES += stm32f10x_spi.c
}
contains(DEFINES, STM32_TIM_SOURCE) {
    HEADERS += stm32f10x_tim.h
    SOURCES += stm32f10x_tim.c
}
contains(DEFINES, STM32_USART_SOURCE) {
    HEADERS += stm32f10x_usart.h
    SOURCES += stm32f10x_usart.c
}
contains(DEFINES, STM32_WWDG_SOURCE) {
    HEADERS += stm32f10x_wwdg.h
    SOURCES += stm32f10x_wwdg.c
}
