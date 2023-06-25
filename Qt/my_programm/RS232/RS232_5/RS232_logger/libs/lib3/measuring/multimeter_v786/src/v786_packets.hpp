/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef V786_PACKETS_HPP
#define V786_PACKETS_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
enum {
    V764_STATUS_REQUEST             = 0x01,
    V764_REMOTE_CONTROL_ENABLE      = 0x02,
    V764_REMOTE_CONTROL_DISABLE     = 0x03,
    V764_SERVICE_ENABLE             = 0x04,
    V764_POWER_OFF                  = 0x05,
    V764_DC_VOLTAGE_MEASUREMENT     = 0x06,
    V764_AC_VOLTAGE_MEASUREMENT     = 0x07,
    V764_DC_CURRENT_MEASUREMENT     = 0x08,
    V764_AC_CURRENT_MEASUREMENT     = 0x09,
    V764_RESISTANCE_MEASUREMENT     = 0x0A,
    V764_CAPACITANCE_MEASUREMENT    = 0x0B,
    V764_INDUCTANCE_MEASUREMENT     = 0x0C,
    V764_BUZZER_TEST                = 0x0D,
    V764_FREQUENCY_MEASUREMENT      = 0x0E,
    V764_ACTIVE_POWER_MEASUREMENT   = 0x0F,
    V764_REACTIVE_POWER_MEASUREMENT = 0x10,
    V764_FULL_POWER_MEASUREMENT     = 0x11,
    V764_COS_FI_MEASUREMENT         = 0x12,
    V764_UDC_CAL                    = 0x13,
    V764_UAC_CAL                    = 0x14,
    V764_IDC_CAL                    = 0x15,
    V764_IAC_CAL                    = 0x16,
    V764_RES_CAL                    = 0x17,
    V764_FLASH_WRITE                = 0x1B,
    V764_SERVICE_DISABLE            = 0x1C,
    V764_DIODE_TEST                 = 0x1D,
    V764_UDC_CAL_ZEROES             = 0x1E,
    V764_IDC_CAL_ZEROES             = 0x1F,
    V764_RES_CAL_ZEROES             = 0x20,
    V764_CAP_CAL_ZEROES             = 0x21,
    V764_IND_CAL_ZEROES             = 0x22,
    V764_FLASH_WRITE_ZEROES         = 0x23,

    //TODO added 28.07.22
    V764_READ_POINT_FROM_MEMORY     = 0x50,
    V764_WRITE_POINT_TO_MEMORY      = 0x51,
    V764_READ_MICROPROGRAMM_VERSION = 0x52,
    V764_SET_NEW_ADDRESS            = 0x53
    //---
};

#define V764_STATUS_REQUEST_TEXT               "Статус мультиметра"
#define V764_REMOTE_CONTROL_ENABLE_TEXT        "Включение удаленного управления"
#define V764_REMOTE_CONTROL_DISABLE_TEXT       "Выключение удаленного управления"
#define V764_SERVICE_ENABLE_TEXT               "Включение сервисного режима"
#define V764_POWER_OFF_TEXT                    "Выключение питания"
#define V764_DC_VOLTAGE_MEASUREMENT_TEXT       "Измерение постоянного напряжения"
#define V764_AC_VOLTAGE_MEASUREMENT_TEXT       "Измерение переменного напряжения"
#define V764_DC_CURRENT_MEASUREMENT_TEXT       "Измерение постоянного тока"
#define V764_AC_CURRENT_MEASUREMENT_TEXT       "Измерение переменного тока"
#define V764_RESISTANCE_MEASUREMENT_TEXT       "Измерение сопротивления"
#define V764_CAPACITANCE_MEASUREMENT_TEXT      "Измерение ёмкости"
#define V764_INDUCTANCE_MEASUREMENT_TEXT       "Измерение индуктивности"
#define V764_BUZZER_TEST_TEXT                  "Тест прозвонки цепи"
#define V764_FREQUENCY_MEASUREMENT_TEXT        "Измерение частоты"
#define V764_ACTIVE_POWER_MEASUREMENT_TEXT     "Измерение активной мощности"
#define V764_REACTIVE_POWER_MEASUREMENT_TEXT   "Измерение реактивной мощности"
#define V764_FULL_POWER_MEASUREMENT_TEXT       "Измерение полной мошности"
#define V764_COS_FI_MEASUREMENT_TEXT           "Измерение коэффициента мощности"
#define V764_UDC_CAL_TEXT                      "Калибровка постоянного напряжения"
#define V764_UAC_CAL_TEXT                      "Калибровка переменного напряжения"
#define V764_IDC_CAL_TEXT                      "Калибровка постоянного тока"
#define V764_IAC_CAL_TEXT                      "Калибровка переменного тока"
#define V764_RES_CAL_TEXT                      "Калибровка сопротивления"
#define V764_FLASH_WRITE_TEXT                  "Запись флеша"
#define V764_SERVICE_DISABLE_TEXT              "Отключение сервисного режима"
#define V764_DIODE_TEST_TEXT                   "Диодный тест"
#define V764_UDC_CAL_ZEROES_TEXT               "Калибровка нулей постоянного напряжения"
#define V764_IDC_CAL_ZEROES_TEXT               "Калибровка нулей постоянного тока"
#define V764_RES_CAL_ZEROES_TEXT               "Калибровка нулей сопротивления"
#define V764_CAP_CAL_ZEROES_TEXT               "Калибровка нулей ёмкости"
#define V764_IND_CAL_ZEROES_TEXT               "Калибровка нулей индуктивности"
#define V764_FLASH_WRITE_ZEROES_TEXT           "Запись нулей во флеш"

//TODO added 28.07.22
#define V764_READ_POINT_FROM_MEMORY_TEXT       "RF_READ_POINT_FROM_MEMORY"
#define V764_WRITE_POINT_TO_MEMORY_TEXT        "RF_WRITE_POINT_TO_MEMORY"
#define V764_READ_MICROPROGRAMM_VERSION_TEXT   "RF_READ_MICROPROGRAMM_VERSION"
#define V764_SET_NEW_ADDRESS_TEXT              "RF_SET_NEW_ADDRESS"
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct
{
    uint16_t SerialNo;
    uint16_t SerialYear;
} SERIAL;

typedef struct
{
    uint8_t address;
    uint8_t cmd;
    uint8_t count_data;
} B7_86_HEADER;
//--------------------------------------------------------------------------------
struct V764_HEADER_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
};
//--------------------------------------------------------------------------------
struct V764_ERROR_A
{
    B7_86_HEADER header;
    unsigned int xxx;
    uint8_t error;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_MEASURING_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_MEASURING_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
struct V764_STATUS_REQUEST_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_STATUS_REQUEST_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t Range; // предел
    uint8_t Mode;  // режим
    uint8_t Math_Hold;
    uint16_t BattVoltage;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_REMOTE_CONTROL_ENABLE_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_REMOTE_CONTROL_ENABLE_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_REMOTE_CONTROL_DISABLE_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_REMOTE_CONTROL_DISABLE_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_SERVICE_ENABLE_Q
{
    B7_86_HEADER header;
    char password[6];
    uint16_t crc16;
    uint8_t channel;
};
struct V764_SERVICE_ENABLE_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_POWER_OFF_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_POWER_OFF_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_DC_VOLTAGE_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_DC_VOLTAGE_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_AC_VOLTAGE_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_AC_VOLTAGE_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_DC_CURRENT_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_DC_CURRENT_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_AC_CURRENT_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_AC_CURRENT_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_RESISTANCE_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_RESISTANCE_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_CAPACITANCE_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_CAPACITANCE_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_INDUCTANCE_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_INDUCTANCE_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_BUZZER_TEST_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_BUZZER_TEST_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_FREQUENCY_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_FREQUENCY_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_ACTIVE_POWER_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_ACTIVE_POWER_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_REACTIVE_POWER_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_REACTIVE_POWER_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_FULL_POWER_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_FULL_POWER_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_COS_FI_MEASUREMENT_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_COS_FI_MEASUREMENT_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    char DisplaySymbols[7];
    uint8_t limits;
    uint8_t deviation_retention;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_UDC_CAL_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_UDC_CAL_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_UAC_CAL_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint8_t data_1;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_UAC_CAL_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_IDC_CAL_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_IDC_CAL_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_IAC_CAL_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint8_t data_1;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_IAC_CAL_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_RES_CAL_Q
{
    B7_86_HEADER header;
    uint8_t data_0;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_RES_CAL_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_FLASH_WRITE_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_FLASH_WRITE_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_SERVICE_DISABLE_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_SERVICE_DISABLE_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint8_t byte_C1;
    uint8_t byte_C2;
    uint8_t byte_C3;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct V764_DIODE_TEST_Q
{
    B7_86_HEADER header;
    uint16_t crc16;
    uint8_t channel;
};
struct V764_DIODE_TEST_A
{
    B7_86_HEADER header;
    SERIAL serial_year;
    uint16_t crc16;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
