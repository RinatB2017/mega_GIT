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
    libusb_exit(ctx);

    save_widgets();
    save_setting();
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
    connect(ui->btn_read,   &QToolButton::clicked,  this,   &MainBox::s_read);
    connect(ui->btn_write,  &QToolButton::clicked,  this,   &MainBox::s_write);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainBox::s_close);

    libusb_init(nullptr);   // инициализация
    //libusb_set_debug(nullptr, USB_DEBUG_LEVEL);  // уровень вывода отладочных сообщений
    libusb_set_option(nullptr, LIBUSB_OPTION_LOG_LEVEL, USB_DEBUG_LEVEL);

    load_widgets();
    load_setting();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

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
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::s_list(void)
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn)
    {
        btn->setDisabled(true);
    }
    f_list();
    if(btn)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::s_open(void)
{
    emit trace(Q_FUNC_INFO);
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn)
    {
        btn->setDisabled(true);
    }
    f_open();
    if(btn)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::s_read(void)
{
    emit trace(Q_FUNC_INFO);
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn)
    {
        btn->setDisabled(true);
    }
    f_read();
    if(btn)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::s_write(void)
{
    emit trace(Q_FUNC_INFO);
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn)
    {
        btn->setDisabled(true);
    }
    f_write();
    if(btn)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::s_close(void)
{
    emit trace(Q_FUNC_INFO);
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn)
    {
        btn->setDisabled(true);
    }
    f_close();
    if(btn)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
QString MainBox::get_error_string(int err)
{
    QString temp;

    switch(err)
    {
    /** Success (no error) */
    case LIBUSB_SUCCESS:
        temp = "Success (no error)";
        break;

        /** Input/output error */
    case LIBUSB_ERROR_IO:
        temp = "Input/output error";
        break;

        /** Invalid parameter */
    case LIBUSB_ERROR_INVALID_PARAM:
        temp = "Invalid parameter";
        break;

        /** Access denied (insufficient permissions) */
    case LIBUSB_ERROR_ACCESS:
        temp = "Access denied (insufficient permissions)";
        break;

        /** No such device (it may have been disconnected) */
    case LIBUSB_ERROR_NO_DEVICE:
        temp = "No such device (it may have been disconnected)";
        break;

        /** Entity not found */
    case LIBUSB_ERROR_NOT_FOUND:
        temp = "Entity not found";
        break;

        /** Resource busy */
    case LIBUSB_ERROR_BUSY:
        temp = "Resource busy";
        break;

        /** Operation timed out */
    case LIBUSB_ERROR_TIMEOUT:
        temp = "Operation timed out";
        break;

        /** Overflow */
    case LIBUSB_ERROR_OVERFLOW:
        temp = "Overflow";
        break;

        /** Pipe error */
    case LIBUSB_ERROR_PIPE:
        temp = "Pipe error";
        break;

        /** System call interrupted (perhaps due to signal) */
    case LIBUSB_ERROR_INTERRUPTED:
        temp = "System call interrupted (perhaps due to signal)";
        break;

        /** Insufficient memory */
    case LIBUSB_ERROR_NO_MEM:
        temp = "Insufficient memory";
        break;

        /** Operation not supported or unimplemented on this platform */
    case LIBUSB_ERROR_NOT_SUPPORTED:
        temp = "Operation not supported or unimplemented on this platform";
        break;

        /* NB: Remember to update LIBUSB_ERROR_COUNT below as well as the
       message strings in strerror.c when adding new error codes here. */

        /** Other error */
    case LIBUSB_ERROR_OTHER:
        temp = "Other error";
        break;

    default:
        temp = QString("unknown error: %1").arg(err);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
bool MainBox::f_list(void)
{
    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(nullptr);
    if (r < 0)
    {
        return false;
    }

    cnt = libusb_get_device_list(nullptr, &devs);
    if (cnt < 0)
    {
        return false;
    }

    print_devs(devs);
    libusb_free_device_list(devs, 1);

    libusb_exit(nullptr);
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::print_info(void)
{
    struct libusb_device_descriptor dev_desc;
    libusb_device *dev = libusb_get_device(handle);
    uint8_t bus = libusb_get_bus_number(dev);
    libusb_get_device_descriptor(dev, &dev_desc);

    emit info("---");
    emit info(QString("length: %1").arg(dev_desc.bLength));
    emit info(QString("device class: %1").arg(dev_desc.bDeviceClass));
    emit info(QString("S/N: %1").arg(dev_desc.iSerialNumber));
    emit info(QString("VID:PID: %1:%2")
              .arg(dev_desc.idVendor,  4, 16, QChar('0'))
              .arg(dev_desc.idProduct, 4, 16, QChar('0')));
    emit info(QString("bcdDevice: %1").arg(dev_desc.bcdDevice));
    emit info(QString("iManufacturer: %1")
              .arg(dev_desc.iManufacturer));
    emit info(QString("iProduct: %1")
              .arg(dev_desc.iProduct));
    emit info(QString("iSerialNumber: %1")
              .arg(dev_desc.iSerialNumber));
    emit info(QString("nb confs: %1").arg(dev_desc.bNumConfigurations));
    emit info("---");
    emit info(QString("bus %1").arg(bus));

    int speed = libusb_get_device_speed(dev);
    emit info(QString("speed %1").arg(speed));

    int active_cfg = -5;
    int err = libusb_get_configuration(handle, &active_cfg);
    if(err != LIBUSB_SUCCESS)
    {
        emit error(QString("libusb_get_configuration return: %1").arg(get_error_string(err)));
    }
    else
    {
        emit info(QString("active_cfg %1").arg(active_cfg));
    }
    emit info("---");
}
//--------------------------------------------------------------------------------
bool MainBox::f_open(void)
{
    handle = libusb_open_device_with_vid_pid(ctx, get_VID(), get_PID());
    if (handle == nullptr)
    {
        emit error("Устройство не подключено");
        return false;
    }
    else
    {
        emit info("Устройство найдено");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_read(void)
{
    if(handle == nullptr)
    {
        emit error("Device not open!");
        return false;
    }

    if (libusb_kernel_driver_active(handle, DEV_INTF))
    {
        libusb_detach_kernel_driver(handle, DEV_INTF);
    }

    int res = libusb_claim_interface(handle,  DEV_INTF);
    if (res != LIBUSB_SUCCESS)
    {
        emit error(QString("Ошибка захвата интерфейса: err = %1").arg(get_error_string(res)));
        libusb_close(handle);
        libusb_exit(nullptr);
        return false;
    }

    print_info();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_write(void)
{
    if(handle == nullptr)
    {
        emit error("Device not open!");
        return false;
    }

    if (libusb_kernel_driver_active(handle, DEV_INTF))
    {
        libusb_detach_kernel_driver(handle, DEV_INTF);
    }

    int res = libusb_claim_interface(handle,  DEV_INTF);
    if (res != LIBUSB_SUCCESS)
    {
        emit error(QString("Ошибка захвата интерфейса: err = %1").arg(get_error_string(res)));
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

    /* Send some data to the device */
    res = libusb_bulk_transfer(handle, EP_OUT, data, sizeof(data), &actual, TIMEOUT);
    if(res != LIBUSB_SUCCESS)
    {
        emit error(QString("libusb_bulk_transfer (send): err = %1").arg(get_error_string(res)));
    }
    else
    {
        length = 1;
        /* Receive data from the device */
        res = libusb_bulk_transfer(handle, 0x81, buf, length, &actual_length, TIMEOUT);
        if(res != LIBUSB_SUCCESS)
        {
            emit error(QString("libusb_bulk_transfer (receive): err = %1").arg(get_error_string(res)));
        }
        else
        {
            for(int n=0; n<actual_length; n++)
            {
                emit info(QString("0x%1 ").arg(buf[n], 2, 16, QChar('0')));
            }
        }
    }

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_close(void)
{
    libusb_close(handle);
    handle = nullptr;
    ctx = nullptr;
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::print_devs(libusb_device **devs)
{
    libusb_device *dev = nullptr;
    int i = 0;

    while ((dev = devs[i++]) != nullptr)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            emit error("failed to get device descriptor");
            return;
        }

        QString iProduct;

        int res = libusb_open(dev, &handle);
        if(res >= 0)
        {
            unsigned char strDesc[256];
            res = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiManufacturer: %1").arg(reinterpret_cast<char *>(strDesc)));
            }
            res = libusb_get_string_descriptor_ascii(handle,    static_cast<uint8_t>(desc.idProduct),   strDesc,    256);
            if(res > 0)
            {
                emit error(QString("\tidProduct: %1").arg(reinterpret_cast<char *>(strDesc)));
            }
            res = libusb_get_string_descriptor_ascii(handle, desc.iProduct, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiProduct: %1").arg(reinterpret_cast<char *>(strDesc)));
                iProduct.append(QString("%1").arg(reinterpret_cast<char *>(strDesc)));
            }
            res = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, strDesc, 256);
            if(res > 0)
            {
                emit error(QString("\tiSerialNumber: %1").arg(reinterpret_cast<char *>(strDesc)));
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
    uint16_t iVID = 0x0a12;
    uint16_t iPID = 0x0042;

    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(iVID, iPID);
    cur_dev = devs;
    if(cur_dev == nullptr)
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
    while(dev == nullptr)
    {
        dev = hid_open(get_VID(), get_PID(), nullptr);
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
void MainBox::dev_close(void)
{
    if(dev != nullptr)
    {
        hid_close(dev);
        dev = nullptr;
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
    gettimeofday(&start, nullptr);

    while (i--)
    {
        int returned = libusb_interrupt_transfer(handle, EP_CTRL, buf, DATA_SIZE, &ret, TIMEOUT);

        if (returned >= 0)
        {
            for (int n=0; n < DATA_SIZE; n++)
            {
                emit info(QString("buf[%1] = %2")
                          .arg(n)
                          .arg(static_cast<int>(buf[n])));
                cc++;
            }
        }
        else
        {
            emit error(QString("interrupt_transfer_loop: %1").arg(libusb_error_name(returned)));
        }
    }

    emit info(QString("Считано: %1").arg(cc));

    gettimeofday(&end, nullptr);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = static_cast<long>(((seconds) * 1000.0 + useconds/1000.0) + 0.5);

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
    gettimeofday(&start, nullptr);

    while(i--)
    {
        int returned = libusb_bulk_transfer(handle, EP_CTRL, buf, DATA_SIZE, &act_len, TIMEOUT);

        // parce transfer errors
        if (returned >= 0)
        {
            for (int n=0; n < DATA_SIZE; n++)
            {
                emit info(QString("buf[%1] = %2")
                          .arg(n)
                          .arg(static_cast<int>(buf[n])));
                cc++;
            }
        }
        else
        {
            emit error(QString("bulk_transfer_loop: %1").arg(libusb_error_name(returned)));
        }
    }

    emit info(QString("Считано: %1").arg(cc));

    gettimeofday(&end, nullptr);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = static_cast<long>(((seconds) * 1000 + useconds/1000.0) + 0.5);

    emit info(QString("Прошло: %1 мс").arg(mtime));
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
