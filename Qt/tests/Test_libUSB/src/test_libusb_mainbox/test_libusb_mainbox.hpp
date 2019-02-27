/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/hidapi/hidapi.h>
#   include </usr/include/libusb-1.0/libusb.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi_win/hidapi.h"
#   include "libusb/libusb.h"
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
//#define VID 0x08bb
//#define PID 0x2704

#define DEV_INTF    1    // номер интерфейса

#define USB_DEBUG_LEVEL 3

#define DATA_SIZE   4

#define EP_CTRL     0x00
#define EP_OUT      0x02
#define EP_HID      0x05

#define TIMEOUT     1000
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QCheckBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    bool f_list(void);
    bool f_read(void);
    bool f_write(void);

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
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    MySplashScreen *splash;
    Ui::MainBox *ui;

    QComboBox *cb_test;
    QCheckBox *cb_block;
    QList<CMD> commands;

    void init(void);
    void createTestBar(void);
    void updateText(void);

    //---
    hid_device *dev = 0;

    void dev_open(void);
    void dev_close(void);

    uint16_t get_VID(void);
    uint16_t get_PID(void);
    void set_VID(uint16_t value);
    void set_PID(uint16_t value);

    void print_devs(libusb_device **devs);
    //---
    void interrupt_transfer_loop(libusb_device_handle *handle);
    void bulk_transfer_loop(libusb_device_handle *handle);
    //---
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
