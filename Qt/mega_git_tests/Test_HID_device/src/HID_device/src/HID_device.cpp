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
#include "ui_HID_device.h"
#include <stdint.h>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "AD9106_box.hpp"
#include "HID_device.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
HID_device::HID_device(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::HID_device)
{
    init();
}
//--------------------------------------------------------------------------------
HID_device::~HID_device()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void HID_device::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    connect(ui->btn_list_usb,   &QPushButton::clicked,  this,   &HID_device::dev_list);
    connect(ui->btn_open,       &QPushButton::clicked,  this,   &HID_device::dev_open);
    connect(ui->btn_read,       &QPushButton::clicked,  this,   &HID_device::dev_read);
    connect(ui->btn_write,      &QPushButton::clicked,  this,   &HID_device::dev_write);
    connect(ui->btn_close,      &QPushButton::clicked,  this,   &HID_device::dev_close);

    connect(ui->btn_led1,   &QPushButton::toggled,  this,   &HID_device::leds_state);
    connect(ui->btn_led2,   &QPushButton::toggled,  this,   &HID_device::leds_state);
    connect(ui->btn_led3,   &QPushButton::toggled,  this,   &HID_device::leds_state);
    connect(ui->btn_led4,   &QPushButton::toggled,  this,   &HID_device::leds_state);

    connect(ui->btn_test,   &QPushButton::clicked,  this,   &HID_device::test);

#if 0
    QFont font("Liberation Mono", 16);
    ui->hexedit_widget->setFont(font);

    QByteArray fram_data;
    for(int n=0; n<10; n++)
    {
        fram_data.append(static_cast<char>(n));
    }
    ui->hexedit_widget->setData(QHexEditData::fromMemory(fram_data));
#endif

    //setFixedSize(sizeHint());
    load_widgets();

    block_buttons(true);
}
//--------------------------------------------------------------------------------
void HID_device::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw != nullptr);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &HID_device::f_test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void HID_device::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef void (HID_device::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void HID_device::dev_list(void)
{
    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0, 0);
    cur_dev = devs;
    if(cur_dev == nullptr)
    {
        //messagebox_critical("Ошибка", "HID_device не найден!");
        emit error("HID_device не найден!");
        return;
    }
    while (cur_dev)
    {
        emit info("Device Found");
        emit info(QString("  type: %1:%2 ").arg(cur_dev->vendor_id).arg(cur_dev->product_id));
        emit info(QString("  path: %1").arg(cur_dev->path));
        emit info(QString("  serial_number: %1").arg(QString::fromWCharArray(cur_dev->serial_number)));
        emit error(QString("  VID:PID %1:%2")
                   .arg(cur_dev->vendor_id, 4, 16, QChar('0'))
                   .arg(cur_dev->product_id, 4, 16, QChar('0'))
                   .toUpper());
        emit info("");
        emit info(QString("  Manufacturer: %1").arg(QString::fromWCharArray(cur_dev->manufacturer_string)));
        emit info(QString("  Product:      %1").arg(QString::fromWCharArray(cur_dev->product_string)));
        emit info("");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
}
//--------------------------------------------------------------------------------
void HID_device::dev_open(void)
{
    uint16_t VID = static_cast<uint16_t>(ui->vid_widget->value());
    uint16_t PID = static_cast<uint16_t>(ui->pid_widget->value());

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    int cnt_err = 0;
    int res = 0;
    while(dev == nullptr)
    {
        res = hid_init();
        if(res != 0)
        {
            emit error(QString("hid_int return %1").arg(res));
        }
        dev = hid_open(VID, PID, nullptr);
        if(dev == nullptr)
        {
            cnt_err++;
        }
        if(cnt_err > 9)
        {
            emit error("hid_open not open!");
            return;
        }
    }

    for(int n=0; n<MAX_STR; n++) wstr[n] = 0;

    // Read the Manufacturer String
    res = hid_get_manufacturer_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Manufacturer String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_manufacturer_string");
    }

    // Read the Product String
    res = hid_get_product_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Product String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_product_string");
    }

    // Read the Serial Number String
    res = hid_get_serial_number_string(dev, wstr, MAX_STR);
    if(res == 0)
    {
        emit info(QString("Serial Number String: %1").arg(QString::fromWCharArray(wstr)));
    }
    else
    {
        emit error("Ошибка: hid_get_serial_number_string");
    }

    hid_set_nonblocking(dev, 1);
    block_buttons(false);
}
//--------------------------------------------------------------------------------
void HID_device::dev_close(void)
{
    if(dev != nullptr)
    {
        emit info("Closed");
        hid_close(dev);
        dev = nullptr;
    }
    block_buttons(true);
}
//--------------------------------------------------------------------------------
void HID_device::test(void)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return;
    }

    int res = 0;
    size_t len = 0;

    //---
    buf[0] = 0;
    buf[1] = 0xAA;

    len = 2;
    res = hid_send_feature_report(dev, buf, len);
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }
    emit debug(QString("res = %1").arg(res));
    //---
    memset(buf, 0x00, SIZE_BUF);
    res = hid_get_feature_report(dev, buf, SIZE_BUF);
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }
    if(res > 0)
    {
        QByteArray ba;
        ba.append(reinterpret_cast<char *>(&buf), res);
        emit info(ba.toHex().data());
    }
    //---

    //---
    buf[0] = 0;
    buf[1] = 0xBB;

    len = 2;
    res = hid_send_feature_report(dev, buf, len);
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }
    emit debug(QString("res = %1").arg(res));
    //---
    memset(buf, 0x00, SIZE_BUF);
    res = hid_get_feature_report(dev, buf, SIZE_BUF);
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }
    if(res > 0)
    {
        QByteArray ba;
        ba.append(reinterpret_cast<char *>(&buf), res);
        emit info(ba.toHex().data());
    }
    //---

    emit info("OK");
}
//--------------------------------------------------------------------------------
void HID_device::dev_read(void)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return;
    }

    int res = 0;
    size_t len = 0;

    buf[0] = 0;

    len = SIZE_BUF;
    res = hid_get_feature_report(dev, buf, len);
    if(res < 0)
    {
        emit error(QString("hid_get_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }

    if(res > 0)
    {
        QByteArray ba;
        ba.append(reinterpret_cast<char *>(&buf), res);
        emit info(ba.toHex().data());
    }
    emit debug(QString("res = %1").arg(res));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void HID_device::dev_write(void)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return;
    }

    int res = 0;
    size_t len = 0;
    memset(buf, 0x00, SIZE_BUF);

    buf[0] = 0;
    buf[1] = 0xBB;
    //buf[1] = 0x02;  //Report_Buf[0] LED1-4
    //buf[2] = 0x00;  //Report_Buf[1] 1-on 0-off

    len = 2; // можно 2 или 3
    res = hid_send_feature_report(dev, buf, len);
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return;
    }
    emit debug(QString("res = %1").arg(res));
    emit info("OK");
}
//--------------------------------------------------------------------------------
bool HID_device::send_cmd(int cmd, int state)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return false;
    }

    int res = 0;
    size_t len = 0;
    memset(buf, 0x00, SIZE_BUF);

    buf[0] = 0;
    buf[1] = static_cast<uint8_t>(cmd);     //Report_Buf[0] LED1-4
    buf[2] = static_cast<uint8_t>(state);   //Report_Buf[1] 1-on 0-off

    len = 3;
    res = hid_send_feature_report(dev, buf, len);
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        emit error(QString("hid_error = [%1]").arg(QString::fromWCharArray(hid_error(dev))));
        return false;
    }
    emit debug(QString("res = %1").arg(res));
    return true;
}
//--------------------------------------------------------------------------------
void HID_device::leds_state(bool state)
{
    bool ok = false;
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    if(dev == nullptr)
    {
        btn->setChecked(false);
        return;
    }
    if(btn == ui->btn_led1)
    {
        emit info("1");
        ok = send_cmd(1, state);
        if(ok)
        {
            if(state)
            {
                btn->setStyleSheet("background:red;");
            }
            else
            {
                btn->setStyleSheet("");
            }
        }
        return;
    }
    if(btn == ui->btn_led2)
    {
        emit info("2");
        ok = send_cmd(2, state);
        if(ok)
        {
            if(state)
            {
                btn->setStyleSheet("background:red;");
            }
            else
            {
                btn->setStyleSheet("");
            }
        }
        return;
    }
    if(btn == ui->btn_led3)
    {
        emit info("3");
        ok = send_cmd(3, state);
        if(ok)
        {
            if(state)
            {
                btn->setStyleSheet("background:red;");
            }
            else
            {
                btn->setStyleSheet("");
            }
        }
        return;
    }
    if(btn == ui->btn_led4)
    {
        emit info("4");
        ok = send_cmd(4, state);
        if(ok)
        {
            if(state)
            {
                btn->setStyleSheet("background:red;");
            }
            else
            {
                btn->setStyleSheet("");
            }
        }
        return;
    }
}
//--------------------------------------------------------------------------------
void HID_device::block_buttons(bool state)
{
    ui->btn_led1->setDisabled(state);
    ui->btn_led2->setDisabled(state);
    ui->btn_led3->setDisabled(state);
    ui->btn_led4->setDisabled(state);

    ui->btn_read->setDisabled(state);
    ui->btn_write->setDisabled(state);
    ui->btn_test->setDisabled(state);
}
//--------------------------------------------------------------------------------
void HID_device::f_test(void)
{
    emit info("Test");

#if 0
    uint unixTime = 0xFFFFFFFF;
    QDateTime timestamp;
    timestamp.setTime_t(unixTime);
    emit info(timestamp.toString(Qt::SystemLocaleLongDate));

    timestamp.setDate(QDate(2101, 11, 18));
    timestamp.setTime(QTime(6, 57, 3));
    emit info(QString("%1").arg(timestamp.toTime_t(), 2, 16, QChar('0')));
#endif

#if 0
    emit info(QString("MEASURES_DATA size = %1").arg(sizeof(MEASURES_DATA)));
#endif

#if 1
    AD9106_Box *box = new AD9106_Box();
    connect(box,    &AD9106_Box::info,  this,   &HID_device::info);
    connect(box,    &AD9106_Box::debug, this,   &HID_device::debug);
    connect(box,    &AD9106_Box::error, this,   &HID_device::error);
    connect(box,    &AD9106_Box::trace, this,   &HID_device::trace);
    box->show();
#endif
}
//--------------------------------------------------------------------------------
void HID_device::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool HID_device::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void HID_device::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void HID_device::save_setting(void)
{

}
//--------------------------------------------------------------------------------
