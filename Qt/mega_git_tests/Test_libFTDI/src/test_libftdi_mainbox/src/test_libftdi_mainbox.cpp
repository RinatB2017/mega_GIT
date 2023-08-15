/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "ui_test_libftdi_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_libftdi_mainbox.hpp"
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
    MyWidget(parent),
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

    connect(ui->btn_open,   &QToolButton::clicked,  this,   &MainBox::f_open);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainBox::f_close);

    connect(ui->btn_read_eeprom,    &QToolButton::clicked,  this,   &MainBox::f_read_eeprom);
    connect(ui->btn_get_eeprom_buf, &QToolButton::clicked,  this,   &MainBox::f_get_eeprom_buf);

    connect(ui->btn_write_eeprom,   &QToolButton::clicked,  this,   &MainBox::f_write_eeprom);
    connect(ui->btn_set_eeprom_buf, &QToolButton::clicked,  this,   &MainBox::f_set_eeprom_buf);

    connect(ui->btn_eeprom_decode,  &QToolButton::clicked,  this,   &MainBox::f_eeprom_decode);

    connect(ui->btn_eeprom_initdefaults,    &QToolButton::clicked,  this,   &MainBox::f_eeprom_initdefaults);

    connect(ui->btn_test,   &QToolButton::clicked,  this,   &MainBox::f_test);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",     &MainBox::test });
    commands.append({ id++, "test2",    &MainBox::test2 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->setProperty(NO_SAVE, true);
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

#if 0
    uint8_t x = '\xF3';
    emit info(QString("x: %1").arg(x, 2, 16, QChar('0')));
#endif

#if 1
    emit info("Test bitbang mode");

    int ret;
    emit info("set bitbang mode");
    ret = ftdi_set_bitmode(&ftdi, 0xFF, BITMODE_BITBANG);
    switch(ret)
    {
    case  0:
        emit info("all fine");
        break;
    case -1:
        emit error("can't enable bitbang mode");
        break;
    case -2:
        emit error("USB device unavailable");
        break;
    }

    unsigned char buf[100] = { 0 };
    ret = ftdi_read_data(&ftdi, buf, 10);
    emit info(QString("ret: %1").arg(ret));

    QByteArray ba;
    ba.append((char *)buf, 10);

    emit info(ba.toHex().toUpper());

    emit info("disabling bitbang mode");
    ret = ftdi_disable_bitbang(&ftdi);
    switch(ret)
    {
    case  0:
        emit info("all fine");
        break;
    case -1:
        emit error("can't disable bitbang mode");
        break;
    case -2:
        emit error("USB device unavailable");
        break;
    }
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test2(void)
{
#if 0
    DATA data;
    data.bites.AB   = 1;
    data.bites.BUF  = 1;
    data.bites.GA   = 1;
    data.bites.SHDN = 1;
    data.bites.data = 4095;

    U16 output;
    output.u16 = data.u16;

    uint8_t res_data[2];
    res_data[0] = output.u8_2.byte_0;
    res_data[1] = output.u8_2.byte_1;
#else
    // Пример отправки данных на MCP4921
    unsigned char res_data[2] = {0x55, 0xAA}; // Пример данных
#endif

    int ret;

    ret = ftdi_write_data(&ftdi, res_data, sizeof(res_data));
    if(ret == -666)
    {
        emit error("USB device unavailable");
        return false;
    }
    if(ret < 0)
    {
        emit error("error code from usb_bulk_write()");
        return false;
    }
    if(ret > 0)
    {
        emit info(QString("%1 sending").arg(ret));
    }

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::wait_msec(int timeout_msec)
{
    QElapsedTimer time;

#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
#endif

    emit debug(QString("Пауза %1 ms").arg(timeout_msec));
    if(timeout_msec < 1)
    {
        emit debug("timeout_msec < 1");
        return;
    }
    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_open(void)
{
    emit trace(Q_FUNC_INFO);

    uint16_t vid = get_VID();
    uint16_t pid = get_PID();
    emit info(QString("VID:PID %1:%2")
              .arg(vid, 4, 16, QChar('0'))
              .arg(pid, 4, 16, QChar('0')));

    int ret;

    ret = ftdi_init(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("couldn't allocate read buffer");
        return;
    case -2:
        emit error("couldn't allocate struct  buffer");
        return;
    case -3:
        emit error("libusb_init() failed");
        return;
    }

    ret = ftdi_usb_open(&ftdi, vid, pid);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -3:
        emit error("usb device not found");
        return;
    case -4:
        emit error("unable to open device");
        return;
    case -5:
        emit error("unable to claim device");
        return;
    case -6:
        emit error("reset failed");
        return;
    case -7:
        emit error("set baudrate failed");
        return;
    case -8:
        emit error("get product description failed");
        return;
    case -9:
        emit error("get serial number failed");
        return;
    case -12:
        emit error("libusb_get_device_list() failed");
        return;
    case -13:
        emit error("libusb_get_device_descriptor() failedc");
        return;
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::f_get_eeprom_buf(void)
{
    emit trace(Q_FUNC_INFO);

    int ret;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE] = { 0 };

    ret = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("struct ftdi_contxt or ftdi_eeprom missing");
        return;
    case -2:
        emit error("Not enough room to store eeprom");
        return;
    }

    QByteArray ba;
    ba.clear();
    for(int n=0; n<FTDI_MAX_EEPROM_SIZE; n++)
    {
        ba.append(buf[n]);
    }
    emit info(ba.toHex());
}
//--------------------------------------------------------------------------------
void MainBox::f_eeprom_initdefaults(void)
{
    char manufacturer[1024] = { 0 };
    char product[1024] = { 0 };
    char serial[1024] = { 0 };
    int ret = ftdi_eeprom_initdefaults(&ftdi, manufacturer, product, serial);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("No struct ftdi_context");
        return;
    case -2:
        emit error("No struct ftdi_eeprom");
        return;
    case -3:
        emit error("No connected device or device not yet opened");
        return;
    }
    return;

    emit info(QString("manufacturer [%1]").arg(manufacturer));
    emit info(QString("product [%1]").arg(product));
    emit info(QString("serial [%1]").arg(serial));
}
//--------------------------------------------------------------------------------
void MainBox::f_set_eeprom_buf(void)
{
    emit trace(Q_FUNC_INFO);

    int ret;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE] = { 0 };

    for(int n=0; n<10; n++)
    {
        buf[n] = n;
    }

    ret = ftdi_set_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("struct ftdi_contxt or ftdi_eeprom of buf missing");
        return;
    }

    ret = ftdi_write_eeprom(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("read failed");
        return;
    case -2:
        emit error("USB device unavailable");
        return;
    case -3:
        emit error("EEPROM not initialized for the connected device");
        return;
    default:
        emit error(QString("ftdi_write_eeprom return %1").arg(ret));
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_close(void)
{
    emit trace(Q_FUNC_INFO);

    int ret;
    ret = ftdi_usb_close(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("usb_release failed");
        return;
    case -3:
        emit error("ftdi context invalid");
        return;
    }
    ftdi_deinit(&ftdi);
}
//--------------------------------------------------------------------------------
void MainBox::f_test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");

    struct ftdi_device_list *devlist, *curdev;
    char manufacturer[128], description[128], serial[128];

    int ret;

    ret = ftdi_init(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("couldn't allocate read buffer");
        return;
    case -2:
        emit error("couldn't allocate struct buffer");
        return;
    case -3:
        emit error("libusb_init() failed");
        return;
    }

    // Select first interface
    ftdi_set_interface(&ftdi, INTERFACE_ANY);

    ret = ftdi_usb_find_all(&ftdi, &devlist, get_VID(), get_PID());
    //ret = ftdi_usb_find_all(&ftdi, &devlist, 0, 0);
    switch(ret)
    {
    case 0:
        emit info("device not found");
        return;
    case -3:
        emit info("out of memory");
        return;
    case -5:
        emit error("libusb_get_device_list() failed");
        return;
    case -6:
        emit error("libusb_get_device_descriptor() failed");
        return;
    default:
        emit info(QString("Number of FTDI devices found: %1").arg(ret));
        break;
    }

    for (curdev = devlist; curdev != NULL; curdev = curdev->next)
    {
        //ret = ftdi_usb_get_strings(&ftdi, curdev->dev, manufacturer, 128, description, 128, serial, 128);
        ret = ftdi_usb_get_strings(&ftdi, curdev->dev, manufacturer, 128, description, 128, NULL, 0);
        switch(ret)
        {
        case 0:
            emit info("all fine");
            emit info(QString("Manufacturer: [%1], Description: [%2], Serial: [%3]")
                      .arg(manufacturer)
                      .arg(description)
                      .arg(serial));
            break;
        case -1:
            emit error("wrong arguments");
            break;
        case -4:
            emit error("unable to open device");
            break;
        case -7:
            emit error("get product manufacturer failed");
            break;
        case -8:
            emit error("get product description failed");
            break;
        case -9:
            emit error("get serial number failed");
            break;
        case -11:
            emit error("libusb_get_device_descriptor() failed");
            break;
        }
    }
    ftdi_list_free(&devlist);
    ftdi_deinit(&ftdi);
}
//--------------------------------------------------------------------------------
void MainBox::f_read_eeprom(void)
{
    int ret;

    ret = ftdi_eeprom_build(&ftdi);
    switch(ret)
    {
    case -1:
        emit error("eeprom size (128 bytes) exceeded by custom strings");
        return;
    case -2:
        emit error("Invalid eeprom or ftdi pointer");
        return;
    case -3:
        emit error("Invalid cbus function setting     (FIXME: Not in the code?)");
        return;
    case -4:
        emit error("Chip doesn't support invert       (FIXME: Not in the code?)");
        return;
    case -5:
        emit error("Chip doesn't support high current drive         (FIXME: Not in the code?)");
        return;
    case -6:
        emit error("No connected EEPROM or EEPROM Type unknown");
        return;
    default:
        emit info(QString("size of eeprom user area in bytes: %1").arg(ret));
        break;
    }

    ret = ftdi_read_eeprom(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("read failed");
        break;
    case -2:
        emit error("USB device unavailable");
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_write_eeprom(void)
{
    int ret;
    ret = ftdi_write_eeprom(&ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("read failed");
        break;
    case -2:
        emit error("USB device unavailable");
        break;
    case -3:
        emit error("EEPROM not initialized for the connected device");
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_eeprom_decode(void)
{
    int ret;
    ret = ftdi_eeprom_decode(&ftdi, 1);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("something went wrong");
        break;
    }
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
