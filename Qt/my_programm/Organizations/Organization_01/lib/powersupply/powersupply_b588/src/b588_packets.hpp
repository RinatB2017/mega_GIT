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
#ifndef PACKETS_B588_HPP
#define PACKETS_B588_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
#include "defines.hpp"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
#define B588_CMD_0x01       0x01
#define B588_CMD_0x01_TEXT tr("Б5-88 функция 0x01: Установка напряжения - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x02       0x02
#define B588_CMD_0x02_TEXT tr("Б5-88 функция 0x02: Запрос измерянного напряжения - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x03       0x03
#define B588_CMD_0x03_TEXT tr("Б5-88 функция 0x03: Запрос установленного напряжения - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x04       0x04
#define B588_CMD_0x04_TEXT tr("Б5-88 функция 0x04: Установка тока - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x05       0x05
#define B588_CMD_0x05_TEXT tr("Б5-88 функция 0x05: Запрос измерянного тока - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x06       0x06
#define B588_CMD_0x06_TEXT tr("Б5-88 функция 0x06: Запрос установленного тока - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x07       0x07
#define B588_CMD_0x07_TEXT tr("Б5-88 функция 0x07: Запрос текущей температуры - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x08       0x08
#define B588_CMD_0x08_TEXT tr("Б5-88 функция 0x08: Запрос версии ПО - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x09       0x09
#define B588_CMD_0x09_TEXT tr("Б5-88 функция 0x09: Запрос даты компиляции ПО - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0A       0x0A
#define B588_CMD_0x0A_TEXT tr("Б5-88 функция 0x0A: Запрос времени компиляции - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0B       0x0B
#define B588_CMD_0x0B_TEXT tr("Б5-88 функция 0x0B: Запрос заводского номера - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0C       0x0C
#define B588_CMD_0x0C_TEXT tr("Б5-88 функция 0x0C: Запрос даты выпуска - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0D       0x0D
#define B588_CMD_0x0D_TEXT tr("Б5-88 функция 0x0D: Запрос состояния профилей - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0E       0x0E
#define B588_CMD_0x0E_TEXT tr("Б5-88 функция 0x0E: Запрос текущего выполняемого профиля и текущей точки - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x0F       0x0F
#define B588_CMD_0x0F_TEXT tr("Б5-88 функция 0x0F: Запрос установки заводских калибровок - ТОЛЬКО ДЛЯ GPIB")


#define B588_CMD_0x10       0x10
#define B588_CMD_0x10_TEXT tr("Б5-88 функция 0x10: Запрос установки пользовательских калибровок - ТОЛЬКО ДЛЯ GPIB")

#define B588_CMD_0x11       0x11
#define B588_CMD_0x11_TEXT tr("Б5-88 функция 0x11: Перейти на дистанционное управление через USB порт")

#define B588_CMD_0x12       0x12
#define B588_CMD_0x12_TEXT tr("Б5-88 функция 0x12: Перейти на местное управление с ДУ через USB порт")

#define B588_CMD_0x13       0x13
#define B588_CMD_0x13_TEXT tr("Б5-88 функция 0x13: Перейти на дистанционное управление через GPIB")

#define B588_CMD_0x14       0x14
#define B588_CMD_0x14_TEXT tr("Б5-88 функция 0x14: Перейти на местное управление с ДУ через GPIB")

#define B588_CMD_0x15       0x15
#define B588_CMD_0x15_TEXT tr("Б5-88 функция 0x15: Запись данных точки в профиль для исполнения")

#define B588_CMD_0x16       0x16
#define B588_CMD_0x16_TEXT tr("Б5-88 функция 0x16: Запуск профиля на исполнение")

#define B588_CMD_0x17       0x17
#define B588_CMD_0x17_TEXT tr("Б5-88 функция 0x17: Переход к режим SLP (выключение) и выход ЦАП на землю через 1кОм")


#define B588_CMD_0x20       0x20
#define B588_CMD_0x20_TEXT tr("Б5-88 функция 0x20: Пока не реализовано")

#define B588_CMD_0x21       0x21
#define B588_CMD_0x21_TEXT tr("Б5-88 функция 0x21: Пока не реализовано")

#define B588_CMD_0x22       0x22
#define B588_CMD_0x22_TEXT tr("Б5-88 функция 0x22: Пока не реализовано")

#define B588_CMD_0x23       0x23
#define B588_CMD_0x23_TEXT tr("Б5-88 функция 0x23: Пока не реализовано")

#define B588_CMD_0x24       0x24
#define B588_CMD_0x24_TEXT tr("Б5-88 функция 0x24: Пока не реализовано")

#define B588_CMD_0x25       0x25
#define B588_CMD_0x25_TEXT tr("Б5-88 функция 0x25: Пока не реализовано")

#define B588_CMD_0x26       0x26
#define B588_CMD_0x26_TEXT tr("Б5-88 функция 0x26: Пока не реализовано")

#define B588_CMD_0x27       0x27
#define B588_CMD_0x27_TEXT tr("Б5-88 функция 0x27: Пока не реализовано")

#define B588_CMD_0x28       0x28
#define B588_CMD_0x28_TEXT tr("Б5-88 функция 0x28: Пока не реализовано")

#define B588_CMD_0x29       0x29
#define B588_CMD_0x29_TEXT tr("Б5-88 функция 0x29: Пока не реализовано")

#define B588_CMD_0x2A       0x2A
#define B588_CMD_0x2A_TEXT tr("Б5-88 функция 0x2A: Пока не реализовано")

#define B588_CMD_0x2B       0x2B
#define B588_CMD_0x2B_TEXT tr("Б5-88 функция 0x2B: Пока не реализовано")

#define B588_CMD_0x2C       0x2C
#define B588_CMD_0x2C_TEXT tr("Б5-88 функция 0x2C: Пока не реализовано")

#define B588_CMD_0x2D       0x2D
#define B588_CMD_0x2D_TEXT tr("Б5-88 функция 0x2D: Пока не реализовано")

#define B588_CMD_0x2E       0x2E
#define B588_CMD_0x2E_TEXT tr("Б5-88 функция 0x2E: Пока не реализовано")

#define B588_CMD_0x2F       0x2F
#define B588_CMD_0x2F_TEXT tr("Б5-88 функция 0x2F: Пока не реализовано")


#define B588_CMD_0x30       0x30
#define B588_CMD_0x30_TEXT tr("Б5-88 функция 0x30: Пока не реализовано")

#define B588_CMD_0x31       0x31
#define B588_CMD_0x31_TEXT tr("Б5-88 функция 0x31: Пока не реализовано")

#define B588_CMD_0x32       0x32
#define B588_CMD_0x32_TEXT tr("Б5-88 функция 0x32: Пока не реализовано")

#define B588_CMD_0x33       0x33
#define B588_CMD_0x33_TEXT tr("Б5-88 функция 0x33: Пока не реализовано")

#define B588_CMD_0x34       0x34
#define B588_CMD_0x34_TEXT tr("Б5-88 функция 0x34: Пока не реализовано")

#define B588_CMD_0x35       0x35
#define B588_CMD_0x35_TEXT tr("Б5-88 функция 0x35: Пока не реализовано")

#define B588_CMD_0x36       0x36
#define B588_CMD_0x36_TEXT tr("Б5-88 функция 0x36: Пока не реализовано")

#define B588_CMD_0x37       0x37
#define B588_CMD_0x37_TEXT tr("Б5-88 функция 0x37: Пока не реализовано")

#define B588_CMD_0x38       0x38
#define B588_CMD_0x38_TEXT tr("Б5-88 функция 0x38: Пока не реализовано")

#define B588_CMD_0x39       0x39
#define B588_CMD_0x39_TEXT tr("Б5-88 функция 0x39: Пока не реализовано")

#define B588_CMD_0x3A       0x3A
#define B588_CMD_0x3A_TEXT tr("Б5-88 функция 0x3A: Пока не реализовано")

#define B588_CMD_0x3B       0x3B
#define B588_CMD_0x3B_TEXT tr("Б5-88 функция 0x3B: Пока не реализовано")

#define B588_CMD_0x3C       0x3C
#define B588_CMD_0x3C_TEXT tr("Б5-88 функция 0x3C: Пока не реализовано")

#define B588_CMD_0x3D       0x3D
#define B588_CMD_0x3D_TEXT tr("Б5-88 функция 0x3E: Пока не реализовано")

#define B588_CMD_0x3E       0x3E
#define B588_CMD_0x3E_TEXT tr("Б5-88 функция 0x3E: Пока не реализовано")

#define B588_CMD_0x3F       0x3F
#define B588_CMD_0x3F_TEXT tr("Б5-88 функция 0x3F: Пока не реализовано")


#define B588_CMD_0x41       0x41
#define B588_CMD_0x41_TEXT tr("Б5-88 функция 0x41: Сброс в исходное состояние.")

#define B588_CMD_0x42       0x42
#define B588_CMD_0x42_TEXT tr("Б5-88 функция 0x42: Установка скорости обмена RS232")

#define B588_CMD_0x43       0x43
#define B588_CMD_0x43_TEXT tr("Б5-88 функция 0x43: Блокировка коридора")

#define B588_CMD_0x44       0x44
#define B588_CMD_0x44_TEXT tr("Б5-88 функция 0x44: Снятие блокировки коридора")

#define B588_CMD_0x46       0x46
#define B588_CMD_0x46_TEXT tr("Б5-88 функция 0x46: Запрос идентификатора.")

#define B588_CMD_0x47       0x47
#define B588_CMD_0x47_TEXT tr("Б5-88 функция 0x47: Запрос текущих данных.")

#define B588_CMD_0x48       0x48
#define B588_CMD_0x48_TEXT tr("Б5-88 функция 0x48: Установка адреса MODBUS")

#define B588_CMD_0x49       0x49
#define B588_CMD_0x49_TEXT tr("Б5-88 функция 0x49: Установка U, I на выходах.")

#define B588_CMD_0x4A       0x4A
#define B588_CMD_0x4A_TEXT tr("Б5-88 функция 0x4A: Запрос состояния - биты ошибок.")


#define B588_CMD_0x54       0x54
#define B588_CMD_0x54_TEXT tr("Б5-88 функция 0x54: Запрос данных по профилям.")

#define B588_CMD_0x55       0x55
#define B588_CMD_0x55_TEXT tr("Б5-88 функция 0x55: Чтение значения мотоминут.")

#define B588_CMD_0x56       0x56
#define B588_CMD_0x56_TEXT tr("Б5-88 функция 0x56: Чтение профиля по точкам.")

#define B588_CMD_0x57       0x57
#define B588_CMD_0x57_TEXT tr("Б5-88 функция 0x57: Установка U,I на выходах в единицах ЦАП")

#define B588_CMD_0x58       0x58
#define B588_CMD_0x58_TEXT tr("Б5-88 функция 0x58: Установка U,I на выходах без коррекцией по калибровочной таблице")

#define B588_CMD_0x59       0x59
#define B588_CMD_0x59_TEXT tr("Б5-88 функция 0x59: Установка U,I на выходах с коррекцией по калибровочной таблице")

#define B588_CMD_0x5A       0x5A
#define B588_CMD_0x5A_TEXT tr("Б5-88 функция 0x5A: Запрос данных устройства выходах (ЦАП (U,I)  с интерполяцией, АЦП (U,I), температура, номер текущей точки в профиле)")

#define B588_CMD_0x5B       0x5B
#define B588_CMD_0x5B_TEXT tr("Б5-88 функция 0x5B: Расширенный запрос данных устройства АЦП реальные значения")

#define B588_CMD_0x5C       0x5C
#define B588_CMD_0x5C_TEXT tr("Б5-88 функция 0x5C: Установка значений для корректировки АЦП напряжения смещение нуля, разница в десятках мВ, та же разница в единицах АЦП (для вычисления коэффициента K)")

#define B588_CMD_0x5D       0x5D
#define B588_CMD_0x5D_TEXT tr("Б5-88 функция 0x5D: Установка значений для корректировки АЦП тока смещение нуля, разница в десятках мА, та же разница в единицах АЦП (для вычисления коэффициента K)")

#define B588_CMD_0x5E       0x5E
#define B588_CMD_0x5E_TEXT tr("Б5-88 функция 0x5E: Запись данных точки в профиль для исполнения.")

#define B588_CMD_0x5F       0x5F
#define B588_CMD_0x5F_TEXT tr("Б5-88 функция 0x5F: Запуск профиля на исполнение.")


#define B588_CMD_0x60       0x60
#define B588_CMD_0x60_TEXT tr("Б5-88 функция 0x60: Переход в режим SLP (выключение).")

#define B588_CMD_0x61       0x61
#define B588_CMD_0x61_TEXT tr("Б5-88 функция 0x61: Чтение значений для корректировки АЦП напряжения смещение нуля, коэффициент K")

#define B588_CMD_0x62       0x62
#define B588_CMD_0x62_TEXT tr("Б5-88 функция 0x62: Чтение значений для корректировки АЦП тока смещение нуля, коэффициент K")

#define B588_CMD_0x63       0x63
#define B588_CMD_0x63_TEXT tr("Б5-88 функция 0x63: Запись точки калибровки ЦАП")

#define B588_CMD_0x64       0x64
#define B588_CMD_0x64_TEXT tr("Б5-88 функция 0x64: Запуск калибровки АЦП, ЦАП (напряжение и ток)")

#define B588_CMD_0x65       0x65
#define B588_CMD_0x65_TEXT tr("Б5-88 функция 0x65: Запрос состояния калибровки ЦАП")

#define B588_CMD_0x66       0x66
#define B588_CMD_0x66_TEXT tr("Б5-88 функция 0x66: Запрос точки калибровки ЦАП")

#define B588_CMD_0x67       0x67
#define B588_CMD_0x67_TEXT tr("Б5-88 функция 0x67: Установка скорости вращение вентилятора (>1600 – автоуправление)")

#define B588_CMD_0x68       0x68
#define B588_CMD_0x68_TEXT tr("Б5-88 функция 0x68: Установка ШИМ питания (>1600 – автоуправление)")

#define B588_CMD_0x69       0x69
#define B588_CMD_0x69_TEXT tr("Б5-88 функция 0x69: Запрос ШИМов вентилятора и питания")

#define B588_CMD_0x6A       0x6A
#define B588_CMD_0x6A_TEXT tr("Б5-88 функция 0x6A: Попытка захвата управления по данному интерфейсу.")

#define B588_CMD_0x6B       0x6B
#define B588_CMD_0x6B_TEXT tr("Б5-88 функция 0x6B: Снятие захвата управления по данному интерфейсу.")

#define B588_CMD_0x6C       0x6C
#define B588_CMD_0x6C_TEXT tr("Б5-88 функция 0x6C: Получение текущей версии программы.")

#define B588_CMD_0x6D       0x6D
#define B588_CMD_0x6D_TEXT tr("Б5-88 функция 0x6D: Служебная функция.")

#define B588_CMD_0x6E       0x6E
#define B588_CMD_0x6E_TEXT tr("Б5-88 функция 0x6E: Запись количества повторов в профиль.")

#define B588_CMD_0x6F       0x6F
#define B588_CMD_0x6F_TEXT tr("Б5-88 функция 0x6F: Очистка профиля")


#define B588_CMD_0x70       0x70
#define B588_CMD_0x70_TEXT tr("Б5-88 функция 0x70: Непрерывная посылка данных об АЦП (только в тестовой версии АЦП)")

#define B588_CMD_0x71       0x71
#define B588_CMD_0x71_TEXT tr("Б5-88 функция 0x71: Запрос статуса АЦП")

#define B588_CMD_0x72       0x72
#define B588_CMD_0x72_TEXT tr("Б5-88 функция 0x72: Установить режим работы с ЦАП - 1 - установка значения по изменению значения 2 - установка значения ЦАП непрерывно с частотой опроса АЦП")

#define B588_CMD_0x73       0x73
#define B588_CMD_0x73_TEXT tr("Б5-88 функция 0x73: Чтение реально установленных значений ЦАП")

#define B588_CMD_0x75       0x75
#define B588_CMD_0x75_TEXT tr("Б5-88 функция 0x75: Запрос данных по интерполяции")

#define B588_CMD_0x7B       0x7B
#define B588_CMD_0x7B_TEXT tr("Б5-88 функция 0x7B: Запись количества повторов профиля")
//--------------------------------------------------------------------------------
#define uint8_t  uint8_t
#define uint16_t uint16_t
//--------------------------------------------------------------------------------
typedef struct {
    uint8_t address;
    uint8_t cmd;
    uint8_t count_data;
} B588_HEADER;
//--------------------------------------------------------------------------------
union B588_CMD_01_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_01_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t error_code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_02_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_02_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t current_U;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_03_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_03_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t setting_U;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_04_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_04_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t error_code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_05_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_05_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            short current_I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_06_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_06_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            short setting_I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_07_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_07_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t temperature;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_08_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_08_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t version;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_09_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_09_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            char data_compile[11];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0A_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            char time_compile[8];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0B_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t serno;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0C_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0C_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t year;
            uint8_t  month;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0D_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0D_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            char profiles[9];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0E_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0E_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t active_profil;
            uint8_t active_point_profil;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_0F_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_0F_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_10_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_10_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_11_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_11_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_12_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_12_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_13_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_13_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_14_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_14_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_15_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_15_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_16_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_16_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_17_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_17_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_20_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_20_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_21_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_21_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_22_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_22_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_23_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_23_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_24_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_24_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_25_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_25_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_26_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_26_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_27_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_27_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_28_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_28_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_29_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_29_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2A_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2B_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2C_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2C_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2D_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2D_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2E_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2E_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_2F_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_2F_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_30_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_30_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_31_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_31_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_32_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_32_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_33_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_33_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_34_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_34_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_35_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_35_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_36_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_36_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_37_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_37_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_38_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_38_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_39_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_39_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3A_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3B_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3C_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3C_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3D_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3D_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3E_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3E_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_3F_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_3F_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_41_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_41_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_42_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t code_speed;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_42_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_43_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_43_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_44_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_44_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_45_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_45_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_46_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_46_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_47_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_47_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t in_voltage;
            uint16_t in_current;
            uint16_t out_voltage;
            uint16_t out_current;
            uint8_t  temperature;
            uint8_t  active_profile;
            uint8_t  current_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_48_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t new_address_modbus;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_48_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t new_address_modbus;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_49_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t voltage;
            uint16_t current;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_49_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  error;
            uint16_t voltage;
            uint16_t current;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_4A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_4A_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
          uint32_t error_data;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_4B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_4B_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_54_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_54_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_54_ANSWER_ERROR
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_55_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_55_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint32_t mototime_min;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_56_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  profile;
            uint8_t  point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_56_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_56_ANSWER_ERROR
{
    struct {
        B588_HEADER header;
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
union B588_CMD_57_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_57_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_58_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_58_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_59_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_59_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t U;
            uint16_t I;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_5A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_5A_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_5B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_5B_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint32_t  ADC_U;
            uint32_t  ADC_I;
            uint32_t  ADC_U_input;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_5C_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_Zero_ADC_U;
            int32_t  Value_delta_ADC_U;
            uint16_t Value_DAC_MIN;
            uint16_t Value_DAC_MAX;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_5C_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_Zero_ADC_U;
            int32_t  Value_delta_ADC_U;
            uint16_t Value_DAC_MIN;
            uint16_t Value_DAC_MAX;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_5D_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_Zero_ADC_I;
            int32_t  Value_delta_ADC_I;
            uint16_t Value_DAC_MIN;
            uint16_t Value_DAC_MAX;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_5D_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_Zero_ADC_I;
            int32_t  Value_delta_ADC_I;
            uint16_t Value_DAC_MIN;
            uint16_t Value_DAC_MAX;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_5E_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
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
union B588_CMD_5E_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t error;
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
//--------------------------------------------------------------------------------
union B588_CMD_5F_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  number_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_5F_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  number_profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_60_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_60_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t error;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_61_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_61_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_ADC_Zero_U;
            double factor_K;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_62_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_62_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            int32_t  Value_ADC_Zero_I;
            double factor_K;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_63_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  type_calibration;
            uint8_t  number_point;
            uint16_t data_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_63_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  number_point;
            uint16_t data_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_64_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_64_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t code;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_65_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_65_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            int8_t number_current_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_66_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t code;
            uint8_t index_point;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_66_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t point_value;
            uint16_t points_crc16;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_67_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t speed_cooler;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_67_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t speed_cooler;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_68_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t data_PWM;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_68_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t data_PWM;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_69_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_69_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t OCR3A;
            uint16_t OCR3B;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_6A_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6A_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_6B_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6B_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t current_driver;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_6C_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6C_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t version;
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

union B588_CMD_6D_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            SIGNATURE signature;
            uint8_t   secret_cmd;
            uint8_t   secret_data[128];
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6D_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_6E_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  profile;
            uint8_t  repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6E_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
            uint8_t  repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_6F_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_6F_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  error;
            uint8_t  profile;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_70_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_70_ANSWER
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_71_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_71_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t  state_ADC_U;
            uint8_t  state_ADC_I;
            uint8_t  state_ADC_Ui;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_72_QUESTION
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t conf_ADC;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_72_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint16_t conf_ADC;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_73_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_73_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_75_QUESTION
{
    struct {
        B588_HEADER header;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_75_ANSWER
{
    struct {
        B588_HEADER header;
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
union B588_CMD_7B_QUESTION //only B5-88
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t number_profile;
            uint8_t repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
union B588_CMD_7B_ANSWER
{
    struct {
        B588_HEADER header;
        struct {
            uint8_t error;
            uint8_t number_profile;
            uint8_t repeat;
        } data;
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
union B588_CMD_NO_FUNCTION_ANSWER
{
    struct {
        B588_HEADER header;
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
