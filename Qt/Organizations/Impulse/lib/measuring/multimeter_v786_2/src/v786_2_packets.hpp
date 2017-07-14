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
#ifndef PACKETS_V786_2_HPP
#define PACKETS_V786_2_HPP
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
#define V764_2_CMD_0x01         0x01
#define V764_2_CMD_0x01_TEXT    tr("Запрос статуса прибора.")

#define V764_2_CMD_0x02         0x02
#define V764_2_CMD_0x02_TEXT    tr("Включение удаленного управления")

#define V764_2_CMD_0x03         0x03
#define V764_2_CMD_0x03_TEXT    tr("Отключение удаленного управления")

#define V764_2_CMD_0x04         0x04
#define V764_2_CMD_0x04_TEXT    tr("Включение сервисных команд (защищено паролем)")

#define V764_2_CMD_0x05         0x05
#define V764_2_CMD_0x05_TEXT    tr("Отключение питания прибора")

#define V764_2_CMD_0x06         0x06
#define V764_2_CMD_0x06_TEXT    tr("Измерение напряжения постоянного тока")

#define V764_2_CMD_0x07         0x07
#define V764_2_CMD_0x07_TEXT    tr("Измерение напряжения переменного тока")

#define V764_2_CMD_0x08         0x08
#define V764_2_CMD_0x08_TEXT    tr("Измерение силы постоянного тока")

#define V764_2_CMD_0x09         0x09
#define V764_2_CMD_0x09_TEXT    tr("Измерение силы переменного тока")

#define V764_2_CMD_0x0A         0x0A
#define V764_2_CMD_0x0A_TEXT    tr("Измерение сопротивления")

#define V764_2_CMD_0x0B         0x0B
#define V764_2_CMD_0x0B_TEXT    tr("Измерение емкости")

#define V764_2_CMD_0x0C         0x0C
#define V764_2_CMD_0x0C_TEXT    tr("Измерение индуктивности")

#define V764_2_CMD_0x0D         0x0D
#define V764_2_CMD_0x0D_TEXT    tr("Прозвонка цепи")

#define V764_2_CMD_0x0E         0x0E
#define V764_2_CMD_0x0E_TEXT    tr("Измерение частоты")

#define V764_2_CMD_0x0F         0x0F
#define V764_2_CMD_0x0F_TEXT    tr("Измерение активной мощности")

#define V764_2_CMD_0x10         0x10
#define V764_2_CMD_0x10_TEXT    tr("Измерение реактивной мощности")

#define V764_2_CMD_0x11         0x11
#define V764_2_CMD_0x11_TEXT    tr("Измерение полной мощности")

#define V764_2_CMD_0x12         0x12
#define V764_2_CMD_0x12_TEXT    tr("Измерение коэффициента мощности")

#define V764_2_CMD_0x13         0x13
#define V764_2_CMD_0x13_TEXT    tr("Калибровка напряжения постоянного тока")

#define V764_2_CMD_0x14         0x14
#define V764_2_CMD_0x14_TEXT    tr("Калибровка напряжения переменного тока")

#define V764_2_CMD_0x15         0x15
#define V764_2_CMD_0x15_TEXT    tr("Калибровка силы постоянного тока")

#define V764_2_CMD_0x16         0x16
#define V764_2_CMD_0x16_TEXT    tr("Калибровка силы переменного тока")

#define V764_2_CMD_0x17         0x17
#define V764_2_CMD_0x17_TEXT    tr("Калибровка сопротивления")

#define V764_2_CMD_0x18         0x18
#define V764_2_CMD_0x18_TEXT    tr("Включение режима заряда АКБ")

#define V764_2_CMD_0x19         0x19
#define V764_2_CMD_0x19_TEXT    tr("Выключение режима заряда АКБ")

#define V764_2_CMD_0x1A         0x1A
#define V764_2_CMD_0x1A_TEXT    tr("Запрос параметров заряда АКБ")

#define V764_2_CMD_0x1B         0x1B
#define V764_2_CMD_0x1B_TEXT    tr("Запись коэффициентов во flash")

#define V764_2_CMD_0x1C         0x1C
#define V764_2_CMD_0x1C_TEXT    tr("Получение ID")

//включение руками - специальной команды нет
#define V764_2_CMD_0x1D         0x1D
#define V764_2_CMD_0x1D_TEXT    tr("Выключение проводного интерфейса")
//--------------------------------------------------------------------------------
typedef struct
{
    unsigned short SerialNo;
    unsigned short SerialYear;
} SERIAL;

typedef struct
{
    unsigned char address;
    unsigned char cmd;
    unsigned char count_data;
} B7_86_HEADER;
//--------------------------------------------------------------------------------
union V764_2_HEADER_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        SERIAL serial_year;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_ERROR_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        unsigned int xxx;
        unsigned char error;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_MEASURING_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned char data_0;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_MEASURING_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        SERIAL serial_year;
        char DisplaySymbols[7];
        struct {
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
union V764_2_01_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_01_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char Range; // предел
            unsigned char Mode;  // режим
            unsigned char Math_Hold;
            unsigned short BattVoltage;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_02_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_02_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_03_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_03_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_04_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            char password[6];
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_04_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_05_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_05_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_06_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_06_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_07_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_07_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_08_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_08_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_09_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_09_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0A_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0A_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0B_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0B_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0C_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0C_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0D_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0D_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0E_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0E_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_0F_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_0F_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_10_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_10_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_11_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_11_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_12_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_12_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            char DisplaySymbols[7];
            unsigned char limits;
            unsigned char deviation_retention;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_13_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_13_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_14_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
            unsigned char data_1;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_14_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_15_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_15_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_16_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
            unsigned char data_1;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_16_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_17_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            unsigned char data_0;
        } data;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_17_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_18_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_18_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_19_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_19_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_1A_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_1A_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_1B_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_1B_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_1C_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_1C_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
            unsigned char byte_C1;
            unsigned char byte_C2;
            unsigned char byte_C3;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union V764_2_1D_QUESTION
{
    struct BODY{
        B7_86_HEADER header;
        unsigned short crc16;
        unsigned char channel;
    } body;
    unsigned char buf[sizeof(BODY)];
};
union V764_2_1D_ANSWER
{
    struct BODY{
        B7_86_HEADER header;
        struct {
            SERIAL serial_year;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
