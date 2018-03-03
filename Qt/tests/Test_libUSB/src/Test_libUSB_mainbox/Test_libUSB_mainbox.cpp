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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "ui_Test_libUSB_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "Test_libUSB_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    load_config();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
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
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(NULL);
    if (r < 0)
    {
        return false;
    }

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
    {
        return false;
    }

    print_devs(devs);
    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

#if 1
    emit info("info");
    emit debug("debug");
    emit error("error");
    emit trace("trace");
#else
    emit info("info", QColor(Qt::red));
    emit debug("debug", QColor(Qt::red));
    emit error("error", QColor(Qt::red));
    emit trace("trace", QColor(Qt::red));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
#include <QDebug>
void MainBox::print_devs(libusb_device **devs)
{
    libusb_device *dev = 0;
    int i = 0;

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            emit error("failed to get device descriptor");
            return;
        }

        libusb_device_handle *handle = 0;
        QString iProduct;

        int res = libusb_open(dev, &handle);
        if(res >= 0)
        {
            unsigned char strDesc[256];
            res = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiManufacturer: %1").arg((char *)strDesc));
            }
            res = libusb_get_string_descriptor_ascii(handle, desc.idProduct, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tidProduct: %1").arg((char *)strDesc));
            }
            res = libusb_get_string_descriptor_ascii(handle, desc.iProduct, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiProduct: %1").arg((char *)strDesc));
                iProduct.append(QString("%1").arg((char *)strDesc));
            }
            res = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiSerialNumber: %1").arg((char *)strDesc));
            }
            libusb_close(handle);
        }
        emit info(QString("Bus %1 Device %2: ID %3:%4 %5")
                  .arg(libusb_get_bus_number(dev),     3, 10, QChar('0'))
                  .arg(libusb_get_device_address(dev), 3, 10, QChar('0'))
                  .arg(desc.idVendor,  4, 16, QChar('0'))
                  .arg(desc.idProduct, 4, 16, QChar('0'))
                  .arg(iProduct));

    }
}
//--------------------------------------------------------------------------------
void MainBox::dev_open(void)
{
#define MAX_STR 255

    int res;
    wchar_t wstr[MAX_STR];
    int VID = 0x0a12;
    int PID = 0x0042;

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(VID, PID);
    cur_dev = devs;
    if(cur_dev == NULL)
    {
        messagebox_critical("Ошибка", "Устройство не найдено!");
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
    while(dev == 0)
    {
        dev = hid_open(VID, PID, NULL);
        if(dev == 0)
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
void MainBox::dev_close(void)
{
    if(dev != 0)
    {
        hid_close(dev);
        dev = 0;
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
