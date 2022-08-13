/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef HID_DEVICE_HPP
#define HID_DEVICE_HPP
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
    #include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
    #include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
#define MAX_STR 255
//--------------------------------------------------------------------------------
//#define SIZE_BUF 64
#ifdef Q_OS_LINUX
#   define SIZE_BUF 128
#endif
#ifdef Q_OS_WIN
#   define SIZE_BUF 256
#endif
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

#define AIN_NUM 9
typedef struct
{
    uint32_t	time;

    uint16_t	v_bat_min;
    uint16_t	v_bat_max;
    uint32_t	v_bat_avg;

    uint16_t	v_sol_min;
    uint16_t	v_sol_max;
    uint32_t	v_sol_avg;

    uint16_t	wind_angle_min;
    uint16_t	wind_angle_max;
    uint32_t	wind_angle_avg;

    struct
    {
        uint16_t	min;
        uint16_t	max;
        uint32_t	avg;
    } v_in[AIN_NUM];

    uint16_t	pressure_avg;   // макс. значение давления - 800, значит накопить можно не больше 65535/800=81 измерение, потом будет переполнение
    uint16_t	pressure_max;
    uint16_t	pressure_min;

    uint16_t	itemp_avg;	// макс. значение температуры - 1000, значит накопить можно не больше 65 измерений, потом будет переполнение
                            // но обычное значение будет не больше 600, значит скорее всего удастся накапливать по 65535/600=109 измерений
    uint16_t	itemp_max;
    uint16_t	itemp_min;

    uint32_t	wind_spd_avg;
    uint32_t	wind_spd_max;
    uint32_t	wind_spd_min;

    uint32_t	rain_count;

    uint8_t		meas_count;

    uint16_t	error_flags;
} MEASURES_DATA;

#pragma pack (pop)
//--------------------------------------------------------------------------------
namespace Ui {
    class HID_device;
}
//--------------------------------------------------------------------------------
class HID_device : public MyWidget
{
    Q_OBJECT

public:
    explicit HID_device(QWidget *parent);
    virtual ~HID_device();

signals:
    void block_widget(bool);

private slots:
    void choice_test(void);
    void f_test(void);

    //---
    void dev_list(void);
    void dev_open(void);
    void dev_close(void);
    void dev_read(void);
    void dev_write(void);
    //---
    void leds_state(bool state);
    //---
    void test(void);

private:
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (HID_device::*func)(void);
    };
    Ui::HID_device *ui;

    //---
    hid_device *dev = nullptr;

    uint8_t buf[SIZE_BUF];
    //---

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    wchar_t wstr[MAX_STR];

    void init(void);
    void createTestBar(void);
    void block_buttons(bool state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
    bool send_cmd(int cmd, int state);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
