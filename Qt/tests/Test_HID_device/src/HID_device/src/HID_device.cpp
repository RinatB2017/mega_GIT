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
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
//--------------------------------------------------------------------------------
#include "ui_HID_device.h"
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
#include "HID_device.hpp"
//--------------------------------------------------------------------------------
#ifdef GRAPHER
#   include "curvebox.hpp"
#endif
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
#ifdef GRAPHER
    if(curve) curve->deleteLater();
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void HID_device::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(ui->btn_open,   SIGNAL(clicked()), this, SLOT(dev_open()));
    connect(ui->btn_close,  SIGNAL(clicked()), this, SLOT(dev_close()));
    connect(ui->btn_send,   SIGNAL(clicked()), this, SLOT(show_state()));

    connect(ui->btn_led1,   SIGNAL(toggled(bool)), this, SLOT(show_led1(bool)));
    connect(ui->btn_led2,   SIGNAL(toggled(bool)), this, SLOT(show_led2(bool)));

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void HID_device::show_led1(bool state)
{
    if(state)
    {
        memset(output_buf, 0, sizeof(output_buf));

        output_buf[0] = 0x01;
        output_buf[1] = 0xFF;
        output_buf[2] = 0x00;

        dev_send();
    }
    else
    {
        memset(output_buf, 0, sizeof(output_buf));

        output_buf[0] = 0x01;
        output_buf[1] = 0x00;
        output_buf[2] = 0x00;

        dev_send();
    }
}
//--------------------------------------------------------------------------------
void HID_device::show_led2(bool state)
{
    if(state)
    {
        memset(output_buf, 0, sizeof(output_buf));

        output_buf[0] = 0x02;
        output_buf[1] = 0xFF;
        output_buf[2] = 0x00;

        dev_send();
    }
    else
    {
        memset(output_buf, 0, sizeof(output_buf));

        output_buf[0] = 0x02;
        output_buf[1] = 0x00;
        output_buf[2] = 0x00;

        dev_send();
    }
}
//--------------------------------------------------------------------------------
void HID_device::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &HID_device::test_0 });
    commands.append({ ID_TEST_1, "test 1", &HID_device::test_1 });
    commands.append({ ID_TEST_2, "test 2", &HID_device::test_2 });
    commands.append({ ID_TEST_3, "test 3", &HID_device::test_3 });
    commands.append({ ID_TEST_4, "test 4", &HID_device::test_4 });
    commands.append({ ID_TEST_5, "test 5", &HID_device::test_5 });

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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (HID_device::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void HID_device::test_0(void)
{
    emit info("Test_0()");
}
//--------------------------------------------------------------------------------
void HID_device::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void HID_device::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void HID_device::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void HID_device::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void HID_device::test_5(void)
{
    emit info("Test_5()");
}
//--------------------------------------------------------------------------------
void HID_device::dev_open(void)
{
    int res;
    wchar_t wstr[MAX_STR];
    uint16_t VID = 0x0483;
    uint16_t PID = 0x5711;

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(VID, PID);
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
        emit info("");
        emit info(QString("  Manufacturer: %1").arg(QString::fromWCharArray(cur_dev->manufacturer_string)));
        emit info(QString("  Product:      %1").arg(QString::fromWCharArray(cur_dev->product_string)));
        emit info("");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    int cnt_err = 0;
    while(dev == nullptr)
    {
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
}
//--------------------------------------------------------------------------------
void HID_device::dev_close(void)
{
    if(dev != nullptr)
    {
        hid_close(dev);
        dev = nullptr;
    }
}
//--------------------------------------------------------------------------------
void HID_device::dev_send(void)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return;
    }

    size_t len = sizeof(output_buf);
    int res = 0;
    res = hid_send_feature_report(dev, output_buf, len);
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }

    //QByteArray ba;
    //ba.append((char *)&output_buf, len);
    //emit info(ba.toHex().data());

    emit info("OK");
}
//--------------------------------------------------------------------------------
void HID_device::show_state(void)
{
    if(dev == nullptr)
    {
        emit error("dev not open!");
        return;
    }

    size_t len = sizeof(output_buf);

    memset(output_buf, 0, len);
    output_buf[0] = 0x04;

    int res = 0;
    res = hid_send_feature_report(dev, output_buf, len);
    if(res < 0)
    {
        emit error(QString("hid_send_feature_report return %1").arg(res));
        return;
    }

    QByteArray ba;
    ba.append((char *)&output_buf, static_cast<int>(len));
    emit info(ba.toHex().data());

    emit info("OK");

}
//--------------------------------------------------------------------------------
void HID_device::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void HID_device::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
