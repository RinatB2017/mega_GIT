/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#ifndef Q_OS_LINUX
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#include "version.hpp"
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_RGB_display"
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_PATCH.VER_BUILD
#define QMAKE_TARGET_COMPANY    ORGNAME
#define QMAKE_TARGET_PRODUCT    APPNAME
//--------------------------------------------------------------------------------
#define BAUDRATE  115200
//--------------------------------------------------------------------------------
#define MAX_SCREEN_X    1024
#define MAX_SCREEN_Y    1024

#define SCREEN_WIDTH    128
#define SCREEN_HEIGTH   64

#define LED_DEFAULT_W   8
#define LED_DEFAULT_H   8

#define LED_BORDER_SIZE 0.5

#define LED_SIZE_W_MM     2.0
#define LED_SIZE_H_MM     2.0

#define LED_BORDER_W_MM   0.1
#define LED_BORDER_H_MM   0.1
//--------------------------------------------------------------------------------
#define CMD_01          0x01
//--------------------------------------------------------------------------------
#define NUM_LEDS        6
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

typedef struct LED
{
    uint8_t color_R;
    uint8_t color_G;
    uint8_t color_B;
} led_t;

union PACKET
{
    struct {
        uint8_t brightness;
        LED leds[SCREEN_WIDTH][SCREEN_HEIGTH];
    } body;
    uint8_t buf[sizeof(body)];
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
