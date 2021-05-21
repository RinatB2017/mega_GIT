/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef USB_HPP
#define USB_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include </usr/include/hidapi/hidapi.h>
#   include </usr/include/libusb-1.0/libusb.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi.h"
#   include "libusb.h"
#endif
//--------------------------------------------------------------------------------
//#define VID 0x1366
//#define PID 0x0101
//--------------------------------------------------------------------------------
#define DEV_INTF    0    // номер интерфейса

#define USB_DEBUG_LEVEL 3

#define DATA_SIZE   4

#define EP_CTRL     0x00
#define EP_OUT      0x02
#define EP_HID      0x05

#define TIMEOUT     1000
//--------------------------------------------------------------------------------
class Usb : public MyWidget
{
    Q_OBJECT

signals:
    void is_opened(bool);

public:
    explicit Usb(QWidget *parent = nullptr);
    virtual ~Usb();

    bool f_list(void);
    bool f_open(uint16_t vid, uint16_t pid);
    bool f_read(void);
    bool f_write(void);
    bool f_close(void);

    bool f_send_cmd(uint8_t cmd, QByteArray param, QByteArray *res_ba);
    void print_info(void);

private:
    void dev_open(uint16_t vid, uint16_t pid);
    void dev_close(void);

    void print_devs(libusb_device **devs);
    QString get_error_string(int err);

    void interrupt_transfer_loop(libusb_device_handle *handle);
    void bulk_transfer_loop(libusb_device_handle *handle);

    hid_device *dev = nullptr;
    libusb_device_handle *handle = nullptr;
    libusb_context *ctx = nullptr;
    unsigned char buf[1024] = { 0 };
    int length = -1;
    int actual_length = -1;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // USB_HPP
