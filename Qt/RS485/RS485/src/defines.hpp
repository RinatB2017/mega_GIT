#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "RS485"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#define CMD_34  0x34
#define CMD_36  0x36
#define CMD_37  0x37
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

union CMD_QUESTION
{
    struct BODY{
        uint16_t prefix;                // префикс
        uint16_t len;                   // длина пакета
        uint8_t  addr;                  // адрес модуля
        uint8_t  reserv_1;              // резерв
        uint8_t  cmd;                   // команда
        uint8_t  reserv_2;              // резерв
        uint16_t time_interval;         // интервал дворника
        uint16_t reserv_3;              // резерв
        uint32_t time_washout;          // время помывки
        uint32_t time_pause_washout;    // время между помывками
        uint32_t addr_cam;              // адрес камеры
        uint32_t preset_washout;        // пресет помывки
        uint32_t time_preset_washout;   // времен помывки
    } body;
    unsigned char buf[sizeof(BODY)];
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
