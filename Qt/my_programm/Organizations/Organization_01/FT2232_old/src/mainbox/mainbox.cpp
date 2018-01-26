/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QToolButton>
#include <QToolBar>
#include <QAction>
#include <QStyle>
#include <QMenu>
#include <QTime>

#include <time.h>
#include <ftdi/src/ftdi_i.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include "qhexedit.h"
#include "grapherbox.hpp"
#include "serialbox5.hpp"
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "doublehexeditor.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"

#include "qhexedit.h"

#include "ad8400.hpp"
#include "bitbang.hpp"
#include "mcp4922.hpp"
#include "i2c.hpp"
#include "spi.hpp"
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
void MainBox::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(-100, 100);
    grapher->set_title("График");
    grapher->set_title_axis_X("Время");
    grapher->set_title_axis_Y("Напряжение");

    serial_data = new QByteArray;
    serial = new SerialBox5(this, "Arduino", "Arduino");
    connect(serial, SIGNAL(output(QByteArray)), this, SLOT(input(QByteArray)));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch();
    hbox->addWidget(serial);
    hbox->addStretch();

    ui->graph_layout->addLayout(hbox);
    ui->graph_layout->addWidget(grapher);
    ui->graph_layout->setStretch(0, 1);
    ui->graph_layout->setStretch(1, 100);

    ui->sbAddress->setMinimum(0);
    ui->sbAddress->setMaximum(MAX_ADDRESS);

    ui->sbData->setMinimum(0);
    ui->sbData->setMaximum(0xFF);

    connect(ui->btnCheckWrite, SIGNAL(clicked()), this, SLOT(test_i2c_write()));
    connect(ui->btnCheckRead,  SIGNAL(clicked()), this, SLOT(test_i2c_read()));

    connect(ui->btnReadAll, SIGNAL(clicked()), this, SLOT(read_all()));
    connect(ui->btnWrite, SIGNAL(clicked()), this, SLOT(write_data()));

    connect(ui->btnClearAll, SIGNAL(clicked()), this, SLOT(clear_all()));

    connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(test_at93c56()));

    connect(ui->btnMCP4922, SIGNAL(clicked()), this, SLOT(test_MCP4922()));

    connect(ui->btnBitBangOn,  SIGNAL(clicked()), this, SLOT(bitbang_on()));
    connect(ui->btnBitBangOff, SIGNAL(clicked()), this, SLOT(bitbang_off()));

    connect(ui->btnAD8400, SIGNAL(clicked()), this, SLOT(test_AD8400()));

    connect(ui->btnRead, SIGNAL(clicked()), this, SLOT(read()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(save()));
}
//--------------------------------------------------------------------------------
void MainBox::input(const QByteArray &data)
{
    if(!serial_data) return;

    if(data.right(1).at(0) != '\n')
    {
        serial_data->append(data);
    }
    else
    {
        emit info(serial_data->data());
        serial_data->clear();
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_AD8400(void)
{
    AD8400 *ad = new AD8400(get_i2c_freq(), this);
    ad->test();
    ad->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::bitbang_on(void)
{
    BitBang *bb = new BitBang(this);
    bb->power_on();
    bb->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::bitbang_off(void)
{
    BitBang *bb = new BitBang(this);
    bb->power_off();
    bb->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::read_all(void)
{
    QByteArray ba;
    bool flag_read = true;
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        for(int n=0; n<MAX_ADDRESS; n++)
        {
            unsigned char data = 0;
            bool ok = i2c->read(160, n, &data);
            if(!ok)
            {
                flag_read = false;
                break;
            }

            ba.append(data);
        }

        if(flag_read)
        {
            QHexEdit *hex = new QHexEdit();
            hex->setMinimumSize(800, 320);
            hex->setReadOnly(true);
            hex->setData(QHexEditData::fromMemory(ba));
            hex->show();
        }

        i2c->close();
    }
    i2c->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::clear_all(void)
{
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        i2c->clear_all();
        i2c->close();
    }

    i2c->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::write_data(void)
{
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        bool ok = i2c->write(160, ui->sbAddress->value(), ui->sbData->value());
        if(!ok)
            emit error("error write data!");

        i2c->close();
    }
    i2c->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = (MainWindow *)parentWidget();
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_i2c_test = add_button(testbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                           "test STM32F0",
                                           "test STM32F0");
    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       "test",
                                       "test");
    QToolButton *btn_erase = add_button(testbar,
                                        new QToolButton(this),
                                        qApp->style()->standardIcon(QStyle::SP_DialogResetButton),
                                        "erase",
                                        "erase");

    connect(btn_i2c_test,  SIGNAL(clicked()), this, SLOT(test_i2c()));
    connect(btn_erase,  SIGNAL(clicked()), this, SLOT(erase()));
    connect(btn_test,  SIGNAL(clicked()), this, SLOT(test_eeprom()));
    //connect(btn_test,  SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
int MainBox::read_eeprom_value(ftdi_eeprom_value value_name)
{
    int value = 0;
    int err = ftdi_get_eeprom_value(&ftdi, value_name, &value);
    if(err < 0)
    {
        emit error("Value doesn't exist");
        return -1;
    }
    return value;
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::erase(void)
{
    int button = QMessageBox::question(this,
                                       tr("Стирание"),
                                       tr("Стереть ftdi_eeprom?"),
                                       QMessageBox::Yes | QMessageBox::No);
    if(button != QMessageBox::Yes)
        return;

    erase_eerpom(0x0403, 0x6010);
}
//--------------------------------------------------------------------------------
void MainBox::read(void)
{
    int err;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE];

    emit info("read");
    ftdi_init(&ftdi);
    err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -3: emit error("usb device not found"); break;
        case -4: emit error("unable to open device"); break;
        case -5: emit error("unable to claim device"); break;
        case -6: emit error("reset failed"); break;
        case -7: emit error("set baudrate failed"); break;
        case -8: emit error("get product description failed"); break;
        case -9: emit error("get serial number failed"); break;
        case -12: emit error("libusb_get_device_list() failed"); break;
        case -13: emit error("libusb_get_device_descriptor() failed"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

    err = ftdi_read_eeprom(&ftdi);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -1: emit error("read failed"); break;
        case -2: emit error("USB device unavailable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }
        return;
    }

    err = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -1: emit error("struct ftdi_contxt or ftdi_eeprom missing"); break;
        case -2: emit error("Not enough room to store eeprom"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

    //    QFile file("file.dat");
    //    file.open(QIODevice::WriteOnly);
    //    QDataStream out(&file);
    //    for(int n=0; n<256; n++)
    //        out << buf[n];

    QByteArray ba;
    ba.clear();
    for(int n=0; n<FTDI_MAX_EEPROM_SIZE; n++)
        ba.append(buf[n]);
    QHexEdit *hex = new QHexEdit();
    hex->setMinimumSize(800, 320);
    hex->setReadOnly(true);
    hex->setData(QHexEditData::fromMemory(ba));
    hex->show();

    ftdi_usb_close(&ftdi);
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    int err;
    unsigned char buf[256];

    emit info("save()");
    QFile file("file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    if(in.atEnd())
    {
        emit error("data stream is empty!");
        return;
    }
    for(int n=0; n<256; n++)
        in >> buf[n];

    ftdi_init(&ftdi);
    err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -3: emit error("usb device not found"); break;
        case -4: emit error("unable to open device"); break;
        case -5: emit error("unable to claim device"); break;
        case -6: emit error("reset failed"); break;
        case -7: emit error("set baudrate failed"); break;
        case -8: emit error("get product description failed"); break;
        case -9: emit error("get serial number failed"); break;
        case -12: emit error("libusb_get_device_list() failed"); break;
        case -13: emit error("libusb_get_device_descriptor() failed"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

    err = ftdi_write_data(&ftdi, buf, 256);
    if(err != 256)
    {
        emit error("error write data");
    }
    ftdi_usb_close(&ftdi);
}
//--------------------------------------------------------------------------------
void MainBox::test_eeprom(void)
{
    int err;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE];

    ftdi_init(&ftdi);
    err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -3: emit error("usb device not found"); break;
        case -4: emit error("unable to open device"); break;
        case -5: emit error("unable to claim device"); break;
        case -6: emit error("reset failed"); break;
        case -7: emit error("set baudrate failed"); break;
        case -8: emit error("get product description failed"); break;
        case -9: emit error("get serial number failed"); break;
        case -12: emit error("libusb_get_device_list() failed"); break;
        case -13: emit error("libusb_get_device_descriptor() failed"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

    err = ftdi_read_eeprom(&ftdi);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -1: emit error("read failed"); break;
        case -2: emit error("USB device unavailable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }
        return;
    }

#if 0
    err = ftdi_set_eeprom_value(&ftdi, CHIP_SIZE, FTDI_MAX_EEPROM_SIZE);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -1: emit error("Value doesn't exist"); break;
        case -2: emit error("Value not user settable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }
#endif

    emit info(QString("VENDOR_ID %1").arg(read_eeprom_value(VENDOR_ID)));
    emit info(QString("PRODUCT_ID %1").arg(read_eeprom_value(PRODUCT_ID)));
    emit info(QString("SELF_POWERED %1").arg(read_eeprom_value(SELF_POWERED)));
    emit info(QString("REMOTE_WAKEUP %1").arg(read_eeprom_value(REMOTE_WAKEUP)));
    emit info(QString("IS_NOT_PNP %1").arg(read_eeprom_value(IS_NOT_PNP)));
    emit info(QString("SUSPEND_DBUS7 %1").arg(read_eeprom_value(SUSPEND_DBUS7)));
    emit info(QString("IN_IS_ISOCHRONOUS %1").arg(read_eeprom_value(IN_IS_ISOCHRONOUS)));
    emit info(QString("OUT_IS_ISOCHRONOUS %1").arg(read_eeprom_value(OUT_IS_ISOCHRONOUS)));
    emit info(QString("SUSPEND_PULL_DOWNS %1").arg(read_eeprom_value(SUSPEND_PULL_DOWNS)));
    emit info(QString("USE_SERIAL %1").arg(read_eeprom_value(USE_SERIAL)));
    emit info(QString("USB_VERSION %1").arg(read_eeprom_value(USB_VERSION)));
    emit info(QString("USE_USB_VERSION %1").arg(read_eeprom_value(USE_USB_VERSION)));
    emit info(QString("MAX_POWER %1").arg(read_eeprom_value(MAX_POWER)));
    emit info(QString("CHANNEL_A_TYPE %1").arg(read_eeprom_value(CHANNEL_A_TYPE)));
    emit info(QString("CHANNEL_B_TYPE %1").arg(read_eeprom_value(CHANNEL_B_TYPE)));
    emit info(QString("CHANNEL_A_DRIVER %1").arg(read_eeprom_value(CHANNEL_A_DRIVER)));
    emit info(QString("CHANNEL_B_DRIVER %1").arg(read_eeprom_value(CHANNEL_B_DRIVER)));
    emit info(QString("CBUS_FUNCTION_0 %1").arg(read_eeprom_value(CBUS_FUNCTION_0)));
    emit info(QString("CBUS_FUNCTION_1 %1").arg(read_eeprom_value(CBUS_FUNCTION_1)));
    emit info(QString("CBUS_FUNCTION_2 %1").arg(read_eeprom_value(CBUS_FUNCTION_2)));
    emit info(QString("CBUS_FUNCTION_3 %1").arg(read_eeprom_value(CBUS_FUNCTION_3)));
    emit info(QString("CBUS_FUNCTION_4 %1").arg(read_eeprom_value(CBUS_FUNCTION_4)));
    emit info(QString("CBUS_FUNCTION_5 %1").arg(read_eeprom_value(CBUS_FUNCTION_5)));
    emit info(QString("CBUS_FUNCTION_6 %1").arg(read_eeprom_value(CBUS_FUNCTION_6)));
    emit info(QString("CBUS_FUNCTION_7 %1").arg(read_eeprom_value(CBUS_FUNCTION_7)));
    emit info(QString("CBUS_FUNCTION_8 %1").arg(read_eeprom_value(CBUS_FUNCTION_8)));
    emit info(QString("CBUS_FUNCTION_9 %1").arg(read_eeprom_value(CBUS_FUNCTION_9)));
    emit info(QString("HIGH_CURRENT %1").arg(read_eeprom_value(HIGH_CURRENT)));
    emit info(QString("HIGH_CURRENT_A %1").arg(read_eeprom_value(HIGH_CURRENT_A)));
    emit info(QString("HIGH_CURRENT_B %1").arg(read_eeprom_value(HIGH_CURRENT_B)));
    emit info(QString("INVERT %1").arg(read_eeprom_value(INVERT)));
    emit info(QString("GROUP0_DRIVE %1").arg(read_eeprom_value(GROUP0_DRIVE)));
    emit info(QString("GROUP0_SCHMITT %1").arg(read_eeprom_value(GROUP0_SCHMITT)));
    emit info(QString("GROUP0_SLEW %1").arg(read_eeprom_value(GROUP0_SLEW)));
    emit info(QString("GROUP1_DRIVE %1").arg(read_eeprom_value(GROUP1_DRIVE)));
    emit info(QString("GROUP1_SCHMITT %1").arg(read_eeprom_value(GROUP1_SCHMITT)));
    emit info(QString("GROUP1_SLEW %1").arg(read_eeprom_value(GROUP1_SLEW)));
    emit info(QString("GROUP2_DRIVE %1").arg(read_eeprom_value(GROUP2_DRIVE)));
    emit info(QString("GROUP2_SCHMITT %1").arg(read_eeprom_value(GROUP2_SCHMITT)));
    emit info(QString("GROUP2_SLEW %1").arg(read_eeprom_value(GROUP2_SLEW)));
    emit info(QString("GROUP3_DRIVE %1").arg(read_eeprom_value(GROUP3_DRIVE)));
    emit info(QString("GROUP3_SCHMITT %1").arg(read_eeprom_value(GROUP3_SCHMITT)));
    emit info(QString("GROUP3_SLEW %1").arg(read_eeprom_value(GROUP3_SLEW)));
    emit info(QString("CHIP_SIZE %1").arg(read_eeprom_value(CHIP_SIZE)));
    emit info(QString("CHIP_TYPE %1").arg(read_eeprom_value(CHIP_TYPE)));
    emit info(QString("POWER_SAVE %1").arg(read_eeprom_value(POWER_SAVE)));
    emit info(QString("CLOCK_POLARITY %1").arg(read_eeprom_value(CLOCK_POLARITY)));
    emit info(QString("DATA_ORDER %1").arg(read_eeprom_value(DATA_ORDER)));
    emit info(QString("FLOW_CONTROL %1").arg(read_eeprom_value(FLOW_CONTROL)));
    emit info(QString("CHANNEL_C_DRIVER %1").arg(read_eeprom_value(CHANNEL_C_DRIVER)));
    emit info(QString("CHANNEL_D_DRIVER %1").arg(read_eeprom_value(CHANNEL_D_DRIVER)));
    emit info(QString("CHANNEL_A_RS485 %1").arg(read_eeprom_value(CHANNEL_A_RS485)));
    emit info(QString("CHANNEL_B_RS485 %1").arg(read_eeprom_value(CHANNEL_B_RS485)));
    emit info(QString("CHANNEL_C_RS485 %1").arg(read_eeprom_value(CHANNEL_C_RS485)));
    emit info(QString("CHANNEL_D_RS485 %1").arg(read_eeprom_value(CHANNEL_D_RS485)));

    unsigned char pins = 0;
    err = ftdi_read_pins(&ftdi, &pins);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("all fine"); break;
        case -1: emit error("read pins failed"); break;
        case -2: emit error("USB device unavailable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }
    emit info(QString("pins %1").arg(pins, 0, 16).toUpper());

    err = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(err)
    {
        switch(err)
        {
        case 0: emit error("All fine"); break;
        case -1: emit error("struct ftdi_contxt or ftdi_eeprom missing"); break;
        case -2: emit error("Not enough room to store eeprom"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

    //    QFile file("file.dat");
    //    file.open(QIODevice::WriteOnly);
    //    QDataStream out(&file);
    //    for(int n=0; n<256; n++)
    //        out << buf[n];

    QByteArray ba;
    ba.clear();
    for(int n=0; n<FTDI_MAX_EEPROM_SIZE; n++)
        ba.append(buf[n]);
    QHexEdit *hex = new QHexEdit();
    hex->setMinimumSize(800, 320);
    hex->setReadOnly(true);
    hex->setData(QHexEditData::fromMemory(ba));
    hex->show();

    ftdi_usb_close(&ftdi);
    emit info("test: OK");
}
//--------------------------------------------------------------------------------
bool MainBox::erase_eerpom(int VID, int PID)
{
    ftdi_context ftdi;
    int err;
    bool ok = false;

    ftdi_init(&ftdi);
    // err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    err = ftdi_usb_open(&ftdi, VID, PID);
    if(err)
    {
        emit error(QString("erase_eerpom: device %1:%2 not open!")
                   .arg(VID, 0, 16)
                   .arg(PID, 0, 16));
        return false;
    }

    err = ftdi_erase_eeprom(&ftdi);
    if(err != 0)
    {
        switch(err)
        {
        case -1: emit error("Erase failed"); break;
        case -2: emit error("USB device unavailable"); break;
        case -3: emit error("Writing magic failed"); break;
        case -4: emit error("Read EEPROM failed"); break;
        case -5: emit error("Unexpected EEPROM value"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }
        ok = false;
    }

    ftdi_usb_close(&ftdi);
    ok = true;
    emit info("erase_eerpom: OK");
    return ok;
}
//--------------------------------------------------------------------------------
void MainBox::test_i2c(void)
{
    emit info("test_i2c");
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        i2c->test();
        i2c->close();
    }
    i2c->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_i2c_write(void)
{
    emit info("test_i2c_write");
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        i2c->test_write();
        i2c->close();
    }
    i2c->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_i2c_read(void)
{
    emit info("test_i2c_read");
    I2C *i2c = new I2C(get_i2c_freq(), this);

    if(i2c->open(0))
    {
        i2c->test_read();
        i2c->close();
    }
    i2c->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_at93c56(void)
{
    emit info("test_at93c56 begin");

    SPI *spi = new SPI(this);
    if(!spi->test_at93c56()) emit error("test error");
    spi->deleteLater();

    emit info("test_at93c56 end");
}
//--------------------------------------------------------------------------------
void MainBox::test_MCP4922(void)
{
    emit info("test_MCP4922 begin");

    MCP4922 *spi = new MCP4922(get_i2c_freq(), this);
    if(!spi->init())
    {
        emit error("test error");
    }
    else
    {
        for(float n=0.0f; n<3.132f; n+=0.1f)
        {
            if(!spi->set_voltage(n)) emit error("set_voltage error");
            QCoreApplication::processEvents();
            Sleeper::msleep(100);
        }
        for(float n=3.132f; n>0.0f; n-=0.1f)
        {
            if(!spi->set_voltage(n)) emit error("set_voltage error");
            QCoreApplication::processEvents();
            Sleeper::msleep(100);
        }
    }
    spi->deleteLater();

    emit info("test_MCP4922 end");
}
//--------------------------------------------------------------------------------
I2C_Freq MainBox::get_i2c_freq(void)
{
    if(ui->rb_400->isChecked()) return I2C_400kHz;
    if(ui->rb_200->isChecked()) return I2C_200kHz;
    if(ui->rb_100->isChecked()) return I2C_100kHz;
    return I2C_100kHz;
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
