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
#include <stdint.h>
#include <QWidget>
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
namespace Ui {
    class HID_device;
}
//--------------------------------------------------------------------------------
class QToolButton;
class QToolBar;
class QComboBox;
//--------------------------------------------------------------------------------
class HID_device : public MyWidget
{
    Q_OBJECT

public:
    HID_device(QWidget *parent);
    ~HID_device();

signals:
    void block_widget(bool);

private slots:
    void choice_test(void);
    void test_0(void);
    void test_1(void);
    void test_2(void);
    void test_3(void);
    void test_4(void);
    void test_5(void);

    void show_led1(bool state);
    void show_led2(bool state);
    void show_state(void);

    //---
    void dev_open(void);
    void dev_close(void);
    void dev_send(void);
    //---

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (HID_device::*func)(void);
    };
    Ui::HID_device *ui;

    //---
    hid_device *dev;

    uint8_t output_buf[0x40];

    //---

    QComboBox *cb_test;
    QList<CMD> commands;

    void init(void);

    void createTestBar(void);
    void updateText(void);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
