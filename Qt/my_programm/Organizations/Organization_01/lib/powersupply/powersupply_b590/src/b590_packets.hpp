/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef PACKETS_B590_HPP
#define PACKETS_B590_HPP
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "b590_defines.hpp"
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
#define B590_CMD_0x11       0x11
#define B590_CMD_0x11_TEXT tr("Б5-90 функция 0x11: Перейти на дистанционное управление через USB порт")

#define B590_CMD_0x12       0x12
#define B590_CMD_0x12_TEXT tr("Б5-90 функция 0x12: Перейти на местное управление с ДУ через USB порт")

#define B590_CMD_0x15       0x15
#define B590_CMD_0x15_TEXT tr("Б5-90 функция 0x15: Запись данных точки в профиль для исполнения")

#define B590_CMD_0x16       0x16
#define B590_CMD_0x16_TEXT tr("Б5-90 функция 0x16: Запуск профиля на исполнение")

#define B590_CMD_0x17       0x17
#define B590_CMD_0x17_TEXT tr("Б5-90 функция 0x17: Переход к режим SLP (выключение) и выход ЦАП на землю через 1кОм")

//--------------------------------------------------------------------------------
#define B590_CMD_0x41       0x41
#define B590_CMD_0x41_TEXT tr("Б5-90 функция 0x41: Сброс в исходное состояние.")

#define B590_CMD_0x42       0x42
#define B590_CMD_0x42_TEXT tr("Б5-90 функция 0x42: Установка скорости обмена RS232")

#define B590_CMD_0x46       0x46
#define B590_CMD_0x46_TEXT tr("Б5-90 функция 0x46: Запрос идентификатора.")

#define B590_CMD_0x47       0x47
#define B590_CMD_0x47_TEXT tr("Б5-90 функция 0x47: Запрос текущих данных.")

#define B590_CMD_0x48       0x48
#define B590_CMD_0x48_TEXT tr("Б5-90 функция 0x48: Установка адреса MODBUS")

#define B590_CMD_0x49       0x49
#define B590_CMD_0x49_TEXT tr("Б5-90 функция 0x49: Установка U, I на выходах.")

#define B590_CMD_0x4A       0x4A
#define B590_CMD_0x4A_TEXT tr("Б5-90 функция 0x4A: Запрос состояния - биты ошибок.")
//--------------------------------------------------------------------------------
#define B590_CMD_0x54       0x54
#define B590_CMD_0x54_TEXT tr("Б5-90 функция 0x54: Запрос данных по профилям.")

#define B590_CMD_0x55       0x55
#define B590_CMD_0x55_TEXT tr("Б5-90 функция 0x55: Чтение значения мотоминут.")

#define B590_CMD_0x56       0x56
#define B590_CMD_0x56_TEXT tr("Б5-90 функция 0x56: Чтение профиля по точкам.")

#define B590_CMD_0x57       0x57
#define B590_CMD_0x57_TEXT tr("Б5-90 функция 0x57: Установка U,I на выходах в единицах ЦАП")

#define B590_CMD_0x5A       0x5A
#define B590_CMD_0x5A_TEXT tr("Б5-90 функция 0x5A: Запрос данных устройства выходах (ЦАП (U,I)  с интерполяцией, АЦП (U,I), температура, номер текущей точки в профиле")

#define B590_CMD_0x5B       0x5B
#define B590_CMD_0x5B_TEXT tr("Б5-90 функция 0x5B: Расширенный запрос данных устройства АЦП реальные значения")

#define B590_CMD_0x5C       0x5C
#define B590_CMD_0x5C_TEXT tr("Б5-90 функция 0x5C: Установка значений для корректировки АЦП")

#define B590_CMD_0x5D       0x5D
#define B590_CMD_0x5D_TEXT tr("Б5-90 функция 0x5D: Чтение значений для корректировки АЦП")

#define B590_CMD_0x5E       0x5E
#define B590_CMD_0x5E_TEXT tr("Б5-90 функция 0x5E: Запись данных точки в профиль для исполнения.")

#define B590_CMD_0x5F       0x5F
#define B590_CMD_0x5F_TEXT tr("Б5-90 функция 0x5F: Запуск профиля на исполнение.")

//--------------------------------------------------------------------------------
#define B590_CMD_0x60       0x60
#define B590_CMD_0x60_TEXT tr("Б5-90 функция 0x60: Переход в режим SLP (выключение).")

#define B590_CMD_0x61       0x61
#define B590_CMD_0x61_TEXT tr("Б5-90 функция 0x61: Чтение значений для корректировки АЦП напряжения смещение нуля коэффициент K")

#define B590_CMD_0x62       0x62
#define B590_CMD_0x62_TEXT tr("Б5-90 функция 0x62: Чтение значений для корректировки АЦП тока смещение нуля, коэффициент K")

#define B590_CMD_0x63       0x63
#define B590_CMD_0x63_TEXT tr("Б5-90 функция 0x63: Запись точки калибровки ЦАП")

#define B590_CMD_0x64       0x64
#define B590_CMD_0x64_TEXT tr("Б5-90 функция 0x64: Запуск калибровки АЦП, ЦАП (напряжение и ток)")

#define B590_CMD_0x65       0x65
#define B590_CMD_0x65_TEXT tr("Б5-90 функция 0x65: Запрос состояния калибровки ЦАП")

#define B590_CMD_0x66       0x66
#define B590_CMD_0x66_TEXT tr("Б5-90 функция 0x66: Запрос точки калибровки ЦАП")

#define B590_CMD_0x67       0x67
#define B590_CMD_0x67_TEXT tr("Б5-90 функция 0x67: Установка скорости вращение вентилятора (>=1600 – автоуправление)")

#define B590_CMD_0x68       0x68
#define B590_CMD_0x68_TEXT tr("Б5-90 функция 0x68: Установка ШИМ питания (>1600 – автоуправление)")

#define B590_CMD_0x69       0x69
#define B590_CMD_0x69_TEXT tr("Б5-90 функция 0x69: Запрос ШИМов вентилятора и питания")

#define B590_CMD_0x6A       0x6A
#define B590_CMD_0x6A_TEXT tr("Б5-90 функция 0x6A: Попытка захвата управления по данному интерфейсу.")

#define B590_CMD_0x6B       0x6B
#define B590_CMD_0x6B_TEXT tr("Б5-90 функция 0x6B: Снятие захвата управления по данному интерфейсу.")

#define B590_CMD_0x6C       0x6C
#define B590_CMD_0x6C_TEXT tr("Б5-90 функция 0x6C: Получение текущей версии программы.")

#define B590_CMD_0x6D       0x6D
#define B590_CMD_0x6D_TEXT tr("Б5-90 функция 0x6D: Служебные функции.")

#define B590_CMD_0x6E       0x6E
#define B590_CMD_0x6E_TEXT tr("Б5-90 функция 0x6E: Запись количества повторов в профиль.")

#define B590_CMD_0x6F       0x6F
#define B590_CMD_0x6F_TEXT tr("Б5-90 функция 0x6F: Очистка профиля")

//--------------------------------------------------------------------------------
#define B590_CMD_0x70       0x70
#define B590_CMD_0x70_TEXT tr("Б5-90 функция 0x70: Непрерывная посылка данных об АЦП (только в тестовой версии АЦП)")

#define B590_CMD_0x71       0x71
#define B590_CMD_0x71_TEXT tr("Б5-90 функция 0x71: Запрос статуса АЦП")

#define B590_CMD_0x72       0x72
#define B590_CMD_0x72_TEXT tr("Б5-90 функция 0x72: Установить конфигурацию АЦП")

#define B590_CMD_0x73       0x73
#define B590_CMD_0x73_TEXT tr("Б5-90 функция 0x73: Прочитать конфигурацию АЦП")

#define B590_CMD_0x75       0x75
#define B590_CMD_0x75_TEXT tr("Б5-90 функция 0x75: Запрос данных по интерполяции")
//--------------------------------------------------------------------------------
typedef struct {
    uint8_t address;
    uint8_t cmd;
    uint8_t count_data;
} B590_HEADER;
//--------------------------------------------------------------------------------
union B590_CMD_11_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_11_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_12_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_12_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_13_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_13_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_14_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_14_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_15_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_15_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_16_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_16_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_17_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_17_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
union B590_CMD_41_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_41_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_42_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t code_speed;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_42_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_46_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_46_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  type;
            uint16_t year;
            uint8_t  month;
            uint16_t serno;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_47_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_47_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            int32_t  in_voltage;
            uint16_t in_current;
            int32_t  out_voltage;
            uint16_t out_current;
            int8_t   temperature;
            uint8_t  active_profile;
            uint8_t  current_point;
            bool     flag_zummer;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_48_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t new_address_modbus;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_48_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t new_address_modbus;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_49_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            int32_t  voltage;
            uint16_t current;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_49_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            int32_t  voltage;
            uint16_t current;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_4A_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_4A_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint32_t error_bits;
            uint16_t error_code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_54_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_54_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
            uint8_t  count_point;
            uint8_t  count_repeat_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_54_ANSWER_ERROR
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_55_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_55_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint32_t mototime_min;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_56_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  profile;
            uint8_t  point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_56_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
            uint8_t  point;
            uint16_t voltage;
            uint16_t current;
            uint16_t time;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_56_ANSWER_ERROR
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
            uint8_t  point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_57_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_57_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5A_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5A_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t setting_DAC_U;
            uint16_t setting_DAC_I;
            uint32_t current_ADC_U;
            uint32_t current_ADC_I;
            uint8_t  temperature;
            uint8_t  active_point;
            uint8_t  active_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5B_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5B_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint32_t ADC_U;
            uint32_t ADC_I;
            uint32_t ADC_U_input;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5C_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            s_zero_b590_U zero_U;
            s_zero_b590_I zero_I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5C_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t error;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5D_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5D_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            s_zero_b590_U zero_U;
            s_zero_b590_I zero_I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5E_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  profile;
            uint16_t voltage;
            uint16_t current;
            uint16_t time;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5E_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t error;
            uint8_t  profile;
            uint16_t voltage;
            uint16_t current;
            uint16_t time;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_5F_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  number_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_5F_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  number_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_60_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_60_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t error;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_61_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_61_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            int32_t  Value_ADC_U_0;
            float factor_k_DAC;
            float factor_k_ADC;
            //double factor_K;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_62_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_62_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            int32_t  Value_ADC_I_0;
            float factor_k_DAC;
            float factor_k_ADC;
            //double factor_K;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_63_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  type_calibration;
            uint8_t  number_point;
            uint16_t data_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_63_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  number_point;
            uint16_t data_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_64_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_64_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_65_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_65_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t number_current_point;
            uint8_t state;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_66_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t code;
            uint8_t index_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_66_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t point_value;
            uint16_t points_crc16;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_67_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t speed_cooler;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_67_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t speed_cooler;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_68_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t data_PWM;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_68_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t data_PWM;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_69_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_69_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t OCR3A;
            uint16_t OCR3B;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_6A_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6A_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_6B_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6B_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_6C_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6C_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  version;
            uint16_t data_compile_year;
            uint8_t  data_compile_month;
            uint8_t  data_compile_day;
            uint8_t  time_compile_hour;
            uint8_t  time_compile_min;
            uint8_t  time_compile_sec;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
typedef struct {
    uint8_t  byte_0;
    uint8_t  byte_1;
    uint8_t  byte_2;
    uint8_t  byte_3;
    uint8_t  byte_4;
    uint8_t  byte_5;
} SIGNATURE;

union DATA_FRAM_GET_ID{
    struct {
        uint8_t ID;
        uint8_t reserved[128 - sizeof(ID)];
    } data;
    uint8_t buf[sizeof(data)];
};

union DATA_FRAM_8BIT{
    struct {
        uint16_t address;
        uint8_t  value;
        uint8_t  reserved[128 - sizeof(address) - sizeof(value)];
    } data;
    uint8_t buf[sizeof(data)];
};

union DATA_FRAM_16BIT{
    struct {
        uint16_t address;
        uint16_t value;
        uint8_t  reserved[128 - sizeof(address) - sizeof(value)];
    } data;
    uint8_t buf[sizeof(data)];
};

union DATA_FRAM_32BIT{
    struct {
        uint16_t address;
        uint32_t value;
        uint8_t reserved[128 - sizeof(address) - sizeof(value)];
    } data;
    uint8_t buf[sizeof(data)];
};

union DATA_FRAM_MORE_BYTES{
    struct {
        uint16_t address;
        uint8_t  data_len;
        uint8_t  fram_data[128 - sizeof(address) - sizeof(data_len)];
    } data;
    uint8_t buf[sizeof(data)];
};

union DATA_FRAM_DATE_AND_SERNO{
    struct {
        uint16_t year;
        uint8_t  month;
        uint16_t serno;
        uint8_t  reserved[128 - sizeof(year) - sizeof(month) - sizeof(serno)];
    } data;
    uint8_t buf[sizeof(data)];
};


#define SECRET_CMD_GET_ID               0x10
#define SECRET_CMD_RESET                0x12
#define SECRET_CMD_CLEAR_MOTOHOUR       0x14
#define SECRET_CMD_SAVE_DATE_AND_SERNO	0x16
#define SECRET_CMD_COPY_USER_TO_FACTORY 0x18

#define SECRET_CMD_CLEAR_FRAM           0x20

#define SECRET_CMD_READ_FRAM_8BIT       0x22
#define SECRET_CMD_READ_FRAM_16BIT      0x24
#define SECRET_CMD_READ_FRAM_32BIT      0x26
#define SECRET_CMD_READ_FRAM_MORE_BYTES	0x28

#define SECRET_CMD_SAVE_FRAM_8BIT       0x30
#define SECRET_CMD_SAVE_FRAM_16BIT      0x32
#define SECRET_CMD_SAVE_FRAM_32BIT      0x34
#define SECRET_CMD_SAVE_FRAM_MORE_BYTES	0x36

union B590_CMD_6D_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            SIGNATURE signature;
            uint8_t   secret_cmd;
            uint8_t   secret_data[128];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6D_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t secret_cmd;
            uint8_t result;
            uint8_t secret_data[128];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_6E_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t profile;
            uint8_t repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6E_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t error;
            uint8_t profile;
            uint8_t repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_6F_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_6F_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t error;
            uint8_t profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_70_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_70_ANSWER
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_71_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_71_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t state_ADC_U;
            uint8_t state_ADC_I;
            uint8_t state_ADC_Ui;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_72_QUESTION
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t conf_ADC;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_72_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t conf_ADC;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_73_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_73_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint16_t real_DAC_U;
            uint16_t real_DAC_I;
            uint8_t  revers;
            uint16_t temperature;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_75_QUESTION
{
    struct {
        B590_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B590_CMD_75_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t  count_ReStart_ADC_1;
            uint8_t  count_ReStart_ADC_0;
            uint8_t  count_ReStart_ADC_2;
            uint16_t count_Error_Answer_ADC;
            uint16_t count_Error_ADC;
            uint16_t MCUCSR;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B590_CMD_NO_FUNCTION_ANSWER
{
    struct {
        B590_HEADER header;
        struct {
            uint8_t error_code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
