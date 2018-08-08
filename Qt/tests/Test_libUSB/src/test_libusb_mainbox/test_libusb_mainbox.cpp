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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#if 0
#include <sys/time.h>  // to use gettimeofday() function
#include <time.h>      // to use time() function
#include "libusb/libusb.h"
#endif
//--------------------------------------------------------------------------------
#include "ui_test_libusb_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_libusb_mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    set_VID(0x08BB);
    set_PID(0x2704);

    connect(ui->btn_list,   SIGNAL(clicked(bool)),  this,   SLOT(f_list()));
    connect(ui->btn_read,   SIGNAL(clicked(bool)),  this,   SLOT(f_read()));
    connect(ui->btn_write,  SIGNAL(clicked(bool)),  this,   SLOT(f_write()));

    createTestBar();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());    //TODO странно
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

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

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
bool MainBox::f_list(void)
{
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
bool MainBox::f_read(void)
{
    libusb_init(NULL);   // инициализация

    //libusb_set_debug(NULL, USB_DEBUG_LEVEL);  // уровень вывода отладочных сообщений
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, USB_DEBUG_LEVEL);

    libusb_device_handle *handle = libusb_open_device_with_vid_pid(NULL, get_VID(), get_PID());
    if (handle == NULL)
    {
        emit error("Устройство не подключено");
        return false;
    }
    else
    {
        emit info("Устройство найдено");
    }

    if (libusb_kernel_driver_active(handle, DEV_INTF))
    {
        libusb_detach_kernel_driver(handle, DEV_INTF);
    }

    if (libusb_claim_interface(handle,  DEV_INTF) < 0)
    {
        emit error("Ошибка захвата интерфейса");
        return false;
    }

    //---
#if 0
    int rc = libusb_set_interface_alt_setting(handle, DEV_INTF, 1);
    if(rc != 0)
    {
        emit error("Cannot configure alternate setting");
        return false;
    }
#endif
    //---

    interrupt_transfer_loop(handle);
    bulk_transfer_loop(handle);

    libusb_attach_kernel_driver(handle, DEV_INTF);
    libusb_close(handle);
    libusb_exit(NULL);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_write(void)
{
    libusb_init(NULL);   // инициализация

    //libusb_set_debug(NULL, USB_DEBUG_LEVEL);  // уровень вывода отладочных сообщений
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, USB_DEBUG_LEVEL);

    libusb_device_handle *handle = libusb_open_device_with_vid_pid(NULL, get_VID(), get_PID());
    if (handle == NULL)
    {
        emit error("Устройство не подключено");
        return false;
    }
    else
    {
        emit info("Устройство найдено");
    }

    if (libusb_kernel_driver_active(handle, DEV_INTF))
    {
        libusb_detach_kernel_driver(handle, DEV_INTF);
    }

    if (libusb_claim_interface(handle,  DEV_INTF) < 0)
    {
        emit error("Ошибка захвата интерфейса");
        return false;
    }

    //---
    //Write data
    int actual = 0;
    unsigned char *data = new unsigned char[4];
    data[0]='a';
    data[1]='b';
    data[2]='c';
    data[3]='d';

    int rc = libusb_bulk_transfer(handle, EP_OUT, data, sizeof(data), &actual, TIMEOUT);
    if(rc != 100)
    {
        emit error(QString("Cannot write data. rc = %1").arg(rc));
    }
    else
    {
        interrupt_transfer_loop(handle);
        bulk_transfer_loop(handle);
    }

    libusb_attach_kernel_driver(handle, DEV_INTF);
    libusb_close(handle);
    libusb_exit(NULL);
    return true;
}
//--------------------------------------------------------------------------------
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
    int iVID = 0x0a12;
    int iPID = 0x0042;

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(iVID, iPID);
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
        dev = hid_open(get_VID(), get_PID(), NULL);
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
void MainBox::interrupt_transfer_loop(libusb_device_handle *handle)
{
    emit info("Цикл считывания (interrupt).");
    unsigned char buf[DATA_SIZE];
    int ret;
    int i=0xF;

    int cc=0;

    struct timeval start, end;
    long mtime, seconds, useconds;
    gettimeofday(&start, NULL);

    while (i--)
    {
        int returned = libusb_interrupt_transfer(handle, EP_CTRL, buf, DATA_SIZE, &ret, TIMEOUT);

        if (returned >= 0)
        {
            for (short i=0; i < DATA_SIZE; i++)
            {
                emit info(QString("buf[%1] = %2").arg(i).arg((int)buf[i]));
                cc++;
            }
        }
        else
        {
            emit error(QString("interrupt_transfer_loop: %1").arg(libusb_error_name(returned)));
        }
    }

    emit info(QString("Считано: %1").arg(cc));

    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    emit info(QString("Прошло: %1 мс").arg(mtime));
}
//--------------------------------------------------------------------------------
void MainBox::bulk_transfer_loop(libusb_device_handle *handle)
{
    /// TODO:
    /// write bulk transfer code for ucontroller & this section
    emit info("Цикл считывания (bulk).");

    unsigned char buf[DATA_SIZE];
    int act_len = 0;

    int cc=0;
    int i=0xF;

    struct timeval start, end;
    long mtime, seconds, useconds;
    gettimeofday(&start, NULL);

    while(i--)
    {
        int returned = libusb_bulk_transfer(handle, EP_CTRL, buf, DATA_SIZE, &act_len, TIMEOUT);

        // parce transfer errors
        if (returned >= 0)
        {
            for (short i=0; i < DATA_SIZE; i++)
            {
                emit info(QString("buf[%1] = %2").arg(i).arg((int)buf[i]));
                cc++;
            }
        }
        else
        {
            emit error(QString("bulk_transfer_loop: %1").arg(libusb_error_name(returned)));
        }
    }

    emit info(QString("Считано: %1").arg(cc));

    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    emit info(QString("Прошло: %1 мс").arg(mtime));
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_VID(void)
{
    return ui->sb_VID->value();
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_PID(void)
{
    return ui->sb_PID->value();
}
//--------------------------------------------------------------------------------
void MainBox::set_VID(uint16_t value)
{
    ui->sb_VID->setValue(value);
}
//--------------------------------------------------------------------------------
void MainBox::set_PID(uint16_t value)
{
    ui->sb_PID->setValue(value);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
