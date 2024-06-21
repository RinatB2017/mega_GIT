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
#include "ui_test_libusb_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_libusb_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <sys/time.h>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    Usb(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    connect(ui->btn_list,   &QToolButton::clicked,  this,   &MainBox::s_list);
    connect(ui->btn_open,   &QToolButton::clicked,  this,   &MainBox::s_open);
    connect(ui->btn_info,   &QToolButton::clicked,  this,   &MainBox::s_info);
    connect(ui->btn_read,   &QToolButton::clicked,  this,   &MainBox::s_read);
    connect(ui->btn_write,  &QToolButton::clicked,  this,   &MainBox::s_write);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainBox::s_close);

    connect(this,   &Usb::is_opened,    ui->btn_info,   &QPushButton::setEnabled);
    connect(this,   &Usb::is_opened,    ui->btn_read,   &QPushButton::setEnabled);
    connect(this,   &Usb::is_opened,    ui->btn_write,  &QPushButton::setEnabled);

    ui->btn_info->setEnabled(false);
    ui->btn_read->setEnabled(false);
    ui->btn_write->setEnabled(false);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test",     &MainBox::test });
        commands.append({ id++, "test2",    &MainBox::test2 });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        cb_test->setProperty(NO_SAVE, true);
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
    }
    else
    {
        emit error("mw not found!");
    }
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
    
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
        );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
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
bool MainBox::test_interface_number(int interface_number)
{
    // Откройте устройство
    libusb_context *ctx = NULL;
    libusb_init(&ctx);

    libusb_device **device_list = NULL;
    ssize_t device_count = libusb_get_device_list(ctx, &device_list);
    if (device_count < 0) {
        emit error("Произошла ошибка при получении списка устройств");
        return false;
    }

    libusb_device_handle *handle = NULL;
    handle = libusb_open_device_with_vid_pid(ctx, ui->sb_VID->value(), ui->sb_PID->value());
    if (handle == nullptr)
    {
        emit error("Устройство не подключено");
        return false;
    }

    // Выберите интерфейс
    int res = libusb_claim_interface(handle, interface_number);
    if (res != LIBUSB_SUCCESS)
    {
        emit error(QString("Ошибка захвата интерфейса: N = %1 err = %2")
                       .arg(interface_number)
                       .arg(res));
        return false;
    }

    // Отправьте данные на плату
    for (int i = 0; i <= 0x7F; i++)
    {
        uint8_t data[1] = { (uint8_t)i };
        int err = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS, 0x01, 0x0000, 0x0000, data, 1, 0);
        switch(err)
        {
        case LIBUSB_ERROR_TIMEOUT:
            emit error("LIBUSB_ERROR_TIMEOUT");
            break;
        case LIBUSB_ERROR_PIPE:
            emit error("LIBUSB_ERROR_PIPE");
            break;
        case LIBUSB_ERROR_NO_DEVICE:
            emit error("LIBUSB_ERROR_NO_DEVICE");
            break;
        case LIBUSB_ERROR_OVERFLOW:
            emit error("LIBUSB_ERROR_OVERFLOW");
            break;
        case LIBUSB_ERROR_OTHER:
            emit error("LIBUSB_ERROR_OTHER");
            break;

        default :
            emit error(QString("unknown error: %1").arg(err));
            break;
        }

        // Ожидайте, пока данные не будут обработаны
        wait_msec(10);
    }

    // Закройте устройство
    libusb_release_interface(handle, interface_number);
    libusb_close(handle);
    libusb_free_device_list(device_list, 1);
    libusb_exit(ctx);

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");

    for(int n=0; n<100; n++)
    {
        test_interface_number(n);
    }

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
#include </usr/include/libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

bool MainBox::test2(void)
{
    libusb_context *ctx = NULL;
    libusb_init(&ctx);

    libusb_device **device_list = NULL;
    ssize_t device_count = libusb_get_device_list(ctx, &device_list);
    device_list = (libusb_device **)malloc(device_count * sizeof(libusb_device *));
    // device_count = libusb_get_device_list(ctx, &device_list);
    if (device_count < 0) {
        emit error(QString("Error getting device list: %1").arg(device_count));
        libusb_exit(ctx);
        return false;
    }

    for (ssize_t i = 0; i < device_count; i++) {
        libusb_device *dev = device_list[i];
        libusb_device_descriptor desc;
        if(dev == nullptr)
        {
            emit error("NULL");
            return false;
        }
        int ret = libusb_get_device_descriptor(dev, &desc);
        if (ret < 0) {
            emit error(QString("Error getting device descriptor: %d\n").arg(ret));
            continue;
        }

        if (desc.idVendor == 0x0403 && desc.idProduct == 0x6010) {
            emit error("Found FTDI device");

            libusb_device_handle *handle = NULL;
            ret = libusb_open(dev, &handle);
            if (ret < 0)
{
                emit error(QString("Error opening device: %1").arg(ret));
                continue;
            }

            const struct libusb_interface_descriptor *interface_descriptor;
            for (int j = 0; j < desc.bNumConfigurations; j++) {
                struct libusb_config_descriptor *config;
                ret = libusb_get_config_descriptor(dev, j, &config);
                if (ret < 0) {
                    emit error(QString("Error getting config descriptor: %d\n").arg(ret));
                    continue;
                }

                for (int k = 0; k < config->bNumInterfaces; k++) {
                    interface_descriptor = &config->interface[k].altsetting[0];
                    emit info(QString("Interface %1").arg(interface_descriptor->bInterfaceNumber));
                    emit info(QString("  Interface Number: %1").arg(interface_descriptor->bInterfaceNumber));
                    emit info(QString("  Interface Class: %1").arg(interface_descriptor->bInterfaceClass));
                    emit info(QString("  Interface SubClass: %1").arg(interface_descriptor->bInterfaceSubClass));
                    emit info(QString("  Interface Protocol: %1").arg(interface_descriptor->bInterfaceProtocol));

                    // Try to claim the interface
                    ret = libusb_claim_interface(handle, interface_descriptor->bInterfaceNumber);
                    if (ret == 0) {
                        emit info("  Interface claimed successfully");
                    } else {
                        emit error(QString("  Error claiming interface: %d\n").arg(ret));
                    }
                }

                libusb_free_config_descriptor(config);
            }

            libusb_close(handle);
        }
    }

    libusb_free_device_list(device_list, 1);
    libusb_exit(ctx);

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::wait_msec(int msec)
{
    QElapsedTimer time;

    emit debug(QString("Пауза %1 ms").arg(msec));
    if(msec < 1)
    {
        emit debug("timeout_ms < 1");
        return;
    }
    time.start();
    while(time.elapsed() < msec)
    {
        QCoreApplication::processEvents();
    }
    emit debug(QString("Прошло %1 msec").arg(msec));
}
//--------------------------------------------------------------------------------
void MainBox::s_list(void)
{
    lock_this_button();
    f_list();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_open(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();

    uint16_t vid = get_VID();
    uint16_t pid = get_PID();
    emit info(QString("%1:%2")
                  .arg(vid, 4, 16, QChar('0'))
                  .arg(pid, 4, 16, QChar('0')));
    f_open(vid, pid);

    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_info(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    set_interface_number(ui->sb_interface_number->value());
    print_info();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_read(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    set_interface_number(ui->sb_interface_number->value());
    f_read();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_write(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    set_interface_number(ui->sb_interface_number->value());
    f_write();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
void MainBox::s_close(void)
{
    emit trace(Q_FUNC_INFO);
    lock_this_button();
    set_interface_number(ui->sb_interface_number->value());
    f_close();
    unlock_this_button();
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_VID(void)
{
    return static_cast<uint16_t>(ui->sb_VID->value());
}
//--------------------------------------------------------------------------------
uint16_t MainBox::get_PID(void)
{
    return static_cast<uint16_t>(ui->sb_PID->value());
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
