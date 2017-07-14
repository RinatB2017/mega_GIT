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
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QStyle>
#include <QDebug>
#include <QMenu>
#include <QTime>
//--------------------------------------------------------------------------------
#include <ftdi/src/ftdi_i.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include "qhexedit.h"
#include "grapherbox.hpp"
#include "serialbox.hpp"
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "doublehexeditor.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "qhexedit.h"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "indicator.hpp"
#include "ds18b20.hpp"
#include "bitbang.hpp"
#include "mcp4922.hpp"
#include "at93c56.hpp"
#include "ad8400.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    QWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    test_flag(false),
    grapher(0),
#ifdef SERIAL
    serial(0),
    serial_data(0)
#endif
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
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
    connect_log();

    createMenu();
    createTestBar();

    grapher = new GrapherBox(0, 1000, -100, 100, tr("График"), tr("Время"), tr("Напряжение"), parentWidget());

#ifdef SERIAL
    serial_data = new QByteArray;
    serial = new SerialBox(this, "Arduido");
    connect(serial, SIGNAL(output(QByteArray)), this, SLOT(input(QByteArray)));

    ui->serial_layout->addWidget(serial);
#endif

    ui->graph_layout->addWidget(grapher);

#if 0
    MainWindow *mw = (MainWindow *)parentWidget();
    if(mw)
    {
        qDebug() << "ccccccccccccc";
        LogBox *le = mw->get_logbox();
        ui->graph_layout->addWidget((QWidget *)le);
    }
#endif

    ui->sbWriteAddress->setMinimum(0);
    ui->sbWriteAddress->setMaximum(MAX_ADDRESS);

    ui->sbWriteData->setMinimum(0);
    ui->sbWriteData->setMaximum(0xFF);

    connect(ui->btnRead,  SIGNAL(clicked()), this, SLOT(read_data()));
    connect(ui->btnWrite, SIGNAL(clicked()), this, SLOT(write_data()));
}
//--------------------------------------------------------------------------------
#ifdef SERIAL
void MainBox::input(const QByteArray &data)
{
#if 0
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
#else
    emit debug(data);
#endif
}
#endif
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
    BitBang *bb = new BitBang(get_i2c_freq(), this);
    bb->power_on();
    bb->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::bitbang_off(void)
{
    BitBang *bb = new BitBang(get_i2c_freq(), this);
    bb->power_off();
    bb->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::read_all(void)
{
    emit info("read_all");

    QByteArray ba;
    bool flag_read = true;
    FT_STATUS ftStatus = FT_OK;
    AT93C56 *at92c56 = new AT93C56(get_i2c_freq(), this);

    ftStatus = at92c56->open(0);
    if(ftStatus == FT_OK)
    {
        for(int n=0; n<MAX_ADDRESS; n++)
        {
            unsigned char data = 0;
            FT_STATUS ftStatus = at92c56->read(160, n, &data);
            if(ftStatus != FT_OK)
            {
                flag_read = false;
                break;
            }
            else
            {
                ba.append(data);
            }
        }

        if(flag_read)
        {
            QHexEdit *hex = new QHexEdit();
            hex->setWindowIcon(QIcon(ICON_PROGRAMM));
            hex->setMinimumSize(800, 320);
            hex->setReadOnly(true);
            hex->setData(ba);
            hex->show();
        }

        at92c56->close();
    }
    at92c56->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::clear_all(void)
{
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0) == FT_OK)
    {
        at93c56->clear_all();
        at93c56->close();
    }

    at93c56->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::read_data(void)
{
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0) == FT_OK)
    {
        unsigned char data = 0;
        bool ok = at93c56->read(160, ui->sbReadAddress->value(), &data);
        if(ok)
        {
            emit info(QString("read %1").arg(data));
        }
        else
        {
            emit error("error read data!");
        }

        at93c56->close();
    }
    at93c56->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::write_data(void)
{
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0) == FT_OK)
    {
        bool ok = at93c56->write(160, ui->sbWriteAddress->value(), ui->sbWriteData->value());
        if(!ok)
            emit error("error write data!");

        at93c56->close();
    }
    at93c56->deleteLater();
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = (MainWindow *)parentWidget();
    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       "test",
                                       "test");

    connect(btn_test,  SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::add_menu_bitbang(QMenu *main_menu)
{
    QMenu *menu_bitbang = new QMenu("BitBang");
    QAction *a_bitBangOn  = menu_bitbang->addAction("ON");
    QAction *a_bitBangOff = menu_bitbang->addAction("OFF");
    main_menu->addMenu(menu_bitbang);

    connect(a_bitBangOn,  SIGNAL(triggered()), this, SLOT(bitbang_on()));
    connect(a_bitBangOff, SIGNAL(triggered()), this, SLOT(bitbang_off()));
}
//--------------------------------------------------------------------------------
void MainBox::add_menu_ftdi_eeprom(QMenu *main_menu)
{
    QMenu *menu_ftdi_eeprom = new QMenu("FFDI eeprom");
    QAction *a_ftdi_eeprom_read  = menu_ftdi_eeprom->addAction("read");
    QAction *a_ftdi_eeprom_save  = menu_ftdi_eeprom->addAction("save");
    main_menu->addMenu(menu_ftdi_eeprom);

    connect(a_ftdi_eeprom_read, SIGNAL(triggered()), this, SLOT(read()));
    connect(a_ftdi_eeprom_save, SIGNAL(triggered()), this, SLOT(save()));
}
//--------------------------------------------------------------------------------
void MainBox::add_menu_tests(QMenu *main_menu)
{
    QMenu *menu_tests = new QMenu("Test");
    QAction *a_test_AD8400  = menu_tests->addAction("test AD8400");
    QAction *a_test_MCP4922 = menu_tests->addAction("test MCP4922");
    QAction *a_test_at93c56 = menu_tests->addAction("test AT93C56");
    main_menu->addMenu(menu_tests);

    connect(a_test_AD8400,  SIGNAL(triggered()), this, SLOT(test_AD8400()));
    connect(a_test_MCP4922, SIGNAL(triggered()), this, SLOT(test_MCP4922()));
    connect(a_test_at93c56, SIGNAL(triggered()), this, SLOT(test_at93c56()));
}
//--------------------------------------------------------------------------------
void MainBox::add_menu_test_at24c02(QMenu *main_menu)
{
    QMenu *menu_test_at24c02 = new QMenu(tr("Проверка AT24C02 (I2C)"));
    QAction *a_test_at24c02_read_all  = menu_test_at24c02->addAction("read all");
    QAction *a_test_at24c02_clear_all = menu_test_at24c02->addAction("clear all");
    QAction *a_test_i2c_write = menu_test_at24c02->addAction("test_i2c_write");
    QAction *a_test_i2c_read  = menu_test_at24c02->addAction("test_i2c_read");
    main_menu->addMenu(menu_test_at24c02);

    connect(a_test_at24c02_read_all,  SIGNAL(triggered()), this, SLOT(read_all()));
    connect(a_test_at24c02_clear_all, SIGNAL(triggered()), this, SLOT(clear_all()));
    connect(a_test_i2c_write, SIGNAL(triggered()), this, SLOT(test_i2c_write()));
    connect(a_test_i2c_read,  SIGNAL(triggered()), this, SLOT(test_i2c_read()));
}
//--------------------------------------------------------------------------------
void MainBox::createMenu(void)
{
    MainWindow *mw = (MainWindow *)topLevelWidget();
    if(!mw) return;

    QMenu *menu = new QMenu("FT2232H");
    QAction *a_test_eeprom = menu->addAction("test_eeprom");
    QAction *a_test_i2c = menu->addAction("test_i2c");
    QAction *a_erase = menu->addAction("erase");

    a_test_eeprom->setIcon(QIcon(ICON_PROGRAMM));
    a_test_i2c->setIcon(QIcon(ICON_PROGRAMM));
    a_erase->setIcon(QIcon(ICON_PROGRAMM));

    connect(a_test_eeprom, SIGNAL(triggered()), this, SLOT(test_eeprom()));
    connect(a_test_i2c, SIGNAL(triggered()), this, SLOT(test_i2c()));
    connect(a_erase, SIGNAL(triggered()), this, SLOT(erase()));

    add_menu_bitbang(menu);
    add_menu_ftdi_eeprom(menu);
    add_menu_tests(menu);
    add_menu_test_at24c02(menu);

    mw->add_menu(2, menu);
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
void MainBox::test(void)
{
    //Indicator *indi = new Indicator(get_i2c_freq(), this);
    //indi->run();

    DS18B20 *ds = new DS18B20(get_i2c_freq(), this);
    ds->test();
}
//--------------------------------------------------------------------------------
void MainBox::read(void)
{
    int err;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE];

    emit info("read");
    ftdi_init(&ftdi);

    //    err = ftdi_set_interface(&ftdi, INTERFACE_A);
    //    emit debug(QString("INTERFACE_A %1").arg(err));
    //    err = ftdi_set_interface(&ftdi, INTERFACE_B);
    //    emit debug(QString("INTERFACE_B %1").arg(err));
    //    err = ftdi_set_interface(&ftdi, INTERFACE_C);
    //    emit debug(QString("INTERFACE_C %1").arg(err));
    //    err = ftdi_set_interface(&ftdi, INTERFACE_D);
    //    emit debug(QString("INTERFACE_D %1").arg(err));

    err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    if(err != 0)
    {
        switch(err)
        {
        case -3:  emit error("usb device not found"); break;
        case -4:  emit error("unable to open device"); break;
        case -5:  emit error("unable to claim device"); break;
        case -6:  emit error("reset failed"); break;
        case -7:  emit error("set baudrate failed"); break;
        case -8:  emit error("get product description failed"); break;
        case -9:  emit error("get serial number failed"); break;
        case -12: emit error("libusb_get_device_list() failed"); break;
        case -13: emit error("libusb_get_device_descriptor() failed"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }

        return;
    }
    emit debug(QString("INTERFACE %1").arg(ftdi.interface));

    err = ftdi_read_eeprom(&ftdi);
    if(err != 0)
    {
        switch(err)
        {
        case -1: emit error("read failed"); break;
        case -2: emit error("USB device unavailable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }
        return;
    }

    err = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(err != 0)
    {
        switch(err)
        {
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
    hex->setWindowIcon(QIcon(ICON_PROGRAMM));
    hex->setMinimumSize(800, 320);
    hex->setReadOnly(true);
    hex->setData(ba);
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
    if(err < 0)
    {
        switch(err)
        {
        case -3: emit error("usb device not found"); break;
        case -4: emit error("unable to open device"); break;
        case -5: emit error("unable to claim device"); break;
        case -6: emit error("reset failed"); break;
        case -7: emit error("set baudrate failed"); break;
        case -8: emit error("get product description failed"); break;
        case -9: emit error("get serial number failed"); break;
        case -12: emit error("libusb_get_device_list() failed"); break;
        case -13: emit error("libusb_get_device_descriptor() failed"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }

        return;
    }

#if 0
    err = ftdi_eeprom_initdefaults(&ftdi, "Work", "FT2232H", "0123456789");
    if(err < 0)
    {
        switch(err)
        {
        case -1: emit error("No struct ftdi_context"); break;
        case -2: emit error("No struct ftdi_eeprom"); break;
        case -3: emit error("No connected device or device not yet opened"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }
    }
#endif

#if 0
    err = ftdi_write_eeprom(&ftdi);
    if(err < 0)
    {
        switch(err)
        {
        case -1: emit error("read failed"); break;
        case -2: emit error("USB device unavailable"); break;
        case -3: emit error("EEPROM not initialized for the connected device;"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }
    }

#endif

#if 0
    err = ftdi_eeprom_build(&ftdi);
    if(err<0)
    {
        switch(err)
        {
        case -1: emit error("eeprom size (128 bytes) exceeded by custom strings"); break;
        case -2: emit error("Invalid eeprom or ftdi pointer"); break;
        case -3: emit error("Invalid cbus function setting     (FIXME: Not in the code?)"); break;
        case -4: emit error("Chip doesn't support invert       (FIXME: Not in the code?)"); break;
        case -5: emit error("Chip doesn't support high current drive         (FIXME: Not in the code?)"); break;
        case -6: emit error("No connected EEPROM or EEPROM Type unknown"); break;
        default:
            emit error(QString("Unknown error %1").arg(err));
            break;
        }
    }
#endif

#if 0
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
#endif

#if 0
    err = ftdi_set_eeprom_value(&ftdi, CHIP_SIZE, FTDI_MAX_EEPROM_SIZE);
    if(err<0)
    {
        switch(err)
        {
        case -1: emit error("Value doesn't exist"); break;
        case -2: emit error("Value not user settable"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }
#endif

#if 0
    err = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(err<0)
    {
        switch(err)
        {
        case -1: emit error("struct ftdi_contxt or ftdi_eeprom missing"); break;
        case -2: emit error("Not enough room to store eeprom"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }
    }
#endif

    QTextEdit *te = new QTextEdit();
    te->setReadOnly(true);
    te->clear();

    te->append(QString("VENDOR_ID\t\t\t0x%1").arg(read_eeprom_value(VENDOR_ID), 0, 16));
    te->append(QString("PRODUCT_ID\t\t0x%1").arg(read_eeprom_value(PRODUCT_ID), 0, 16));
    te->append(QString("SELF_POWERED\t\t%1").arg(read_eeprom_value(SELF_POWERED)));
    te->append(QString("REMOTE_WAKEUP\t\t%1").arg(read_eeprom_value(REMOTE_WAKEUP)));
    te->append(QString("IS_NOT_PNP\t\t\t%1").arg(read_eeprom_value(IS_NOT_PNP)));
    te->append(QString("SUSPEND_DBUS7\t\t%1").arg(read_eeprom_value(SUSPEND_DBUS7)));
    te->append(QString("IN_IS_ISOCHRONOUS\t\t%1").arg(read_eeprom_value(IN_IS_ISOCHRONOUS)));
    te->append(QString("OUT_IS_ISOCHRONOUS\t\t%1").arg(read_eeprom_value(OUT_IS_ISOCHRONOUS)));
    te->append(QString("SUSPEND_PULL_DOWNS\t\t%1").arg(read_eeprom_value(SUSPEND_PULL_DOWNS)));
    te->append(QString("USE_SERIAL\t\t%1").arg(read_eeprom_value(USE_SERIAL)));
    te->append(QString("USB_VERSION\t\t%1").arg(read_eeprom_value(USB_VERSION)));
    te->append(QString("USE_USB_VERSION\t\t%1").arg(read_eeprom_value(USE_USB_VERSION)));
    te->append(QString("MAX_POWER\t\t%1").arg(read_eeprom_value(MAX_POWER)));
    te->append(QString("CHANNEL_A_TYPE\t\t%1").arg(read_eeprom_value(CHANNEL_A_TYPE)));
    te->append(QString("CHANNEL_B_TYPE\t\t%1").arg(read_eeprom_value(CHANNEL_B_TYPE)));
    te->append(QString("CHANNEL_A_DRIVER\t\t%1").arg(read_eeprom_value(CHANNEL_A_DRIVER)));
    te->append(QString("CHANNEL_B_DRIVER\t\t%1").arg(read_eeprom_value(CHANNEL_B_DRIVER)));
    te->append(QString("CBUS_FUNCTION_0\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_0)));
    te->append(QString("CBUS_FUNCTION_1\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_1)));
    te->append(QString("CBUS_FUNCTION_2\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_2)));
    te->append(QString("CBUS_FUNCTION_3\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_3)));
    te->append(QString("CBUS_FUNCTION_4\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_4)));
    te->append(QString("CBUS_FUNCTION_5\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_5)));
    te->append(QString("CBUS_FUNCTION_6\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_6)));
    te->append(QString("CBUS_FUNCTION_7\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_7)));
    te->append(QString("CBUS_FUNCTION_8\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_8)));
    te->append(QString("CBUS_FUNCTION_9\t\t%1").arg(read_eeprom_value(CBUS_FUNCTION_9)));
    te->append(QString("HIGH_CURRENT\t\t%1").arg(read_eeprom_value(HIGH_CURRENT)));
    te->append(QString("HIGH_CURRENT_A\t\t%1").arg(read_eeprom_value(HIGH_CURRENT_A)));
    te->append(QString("HIGH_CURRENT_B\t\t%1").arg(read_eeprom_value(HIGH_CURRENT_B)));
    te->append(QString("INVERT\t\t\t%1").arg(read_eeprom_value(INVERT)));
    te->append(QString("GROUP0_DRIVE\t\t%1").arg(read_eeprom_value(GROUP0_DRIVE)));
    te->append(QString("GROUP0_SCHMITT\t\t%1").arg(read_eeprom_value(GROUP0_SCHMITT)));
    te->append(QString("GROUP0_SLEW\t\t%1").arg(read_eeprom_value(GROUP0_SLEW)));
    te->append(QString("GROUP1_DRIVE\t\t%1").arg(read_eeprom_value(GROUP1_DRIVE)));
    te->append(QString("GROUP1_SCHMITT\t\t%1").arg(read_eeprom_value(GROUP1_SCHMITT)));
    te->append(QString("GROUP1_SLEW\t\t%1").arg(read_eeprom_value(GROUP1_SLEW)));
    te->append(QString("GROUP2_DRIVE\t\t%1").arg(read_eeprom_value(GROUP2_DRIVE)));
    te->append(QString("GROUP2_SCHMITT\t\t%1").arg(read_eeprom_value(GROUP2_SCHMITT)));
    te->append(QString("GROUP2_SLEW\t\t%1").arg(read_eeprom_value(GROUP2_SLEW)));
    te->append(QString("GROUP3_DRIVE\t\t%1").arg(read_eeprom_value(GROUP3_DRIVE)));
    te->append(QString("GROUP3_SCHMITT\t\t%1").arg(read_eeprom_value(GROUP3_SCHMITT)));
    te->append(QString("GROUP3_SLEW\t\t%1").arg(read_eeprom_value(GROUP3_SLEW)));
    te->append(QString("CHIP_SIZE\t\t\t%1").arg(read_eeprom_value(CHIP_SIZE)));
    te->append(QString("CHIP_TYPE\t\t\t%1").arg(read_eeprom_value(CHIP_TYPE)));
    te->append(QString("POWER_SAVE\t\t%1").arg(read_eeprom_value(POWER_SAVE)));
    te->append(QString("CLOCK_POLARITY\t\t%1").arg(read_eeprom_value(CLOCK_POLARITY)));
    te->append(QString("DATA_ORDER\t\t%1").arg(read_eeprom_value(DATA_ORDER)));
    te->append(QString("FLOW_CONTROL\t\t%1").arg(read_eeprom_value(FLOW_CONTROL)));
    te->append(QString("CHANNEL_C_DRIVER\t\t%1").arg(read_eeprom_value(CHANNEL_C_DRIVER)));
    te->append(QString("CHANNEL_D_DRIVER\t\t%1").arg(read_eeprom_value(CHANNEL_D_DRIVER)));
    te->append(QString("CHANNEL_A_RS485\t\t%1").arg(read_eeprom_value(CHANNEL_A_RS485)));
    te->append(QString("CHANNEL_B_RS485\t\t%1").arg(read_eeprom_value(CHANNEL_B_RS485)));
    te->append(QString("CHANNEL_C_RS485\t\t%1").arg(read_eeprom_value(CHANNEL_C_RS485)));
    te->append(QString("CHANNEL_D_RS485\t\t%1").arg(read_eeprom_value(CHANNEL_D_RS485)));

    QFrame *frame = new QFrame;
    frame->setWindowIcon(QIcon(ICON_PROGRAMM));
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setStretch(0, 100);
    vbox->setStretch(1, 1);
    vbox->addWidget(te);

#if 0
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
#endif

    err = ftdi_get_eeprom_buf(&ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(err < 0)
    {
        switch(err)
        {
        case -1: emit error("struct ftdi_contxt or ftdi_eeprom missing"); break;
        case -2: emit error("Not enough room to store eeprom"); break;
        default: emit error(QString("Unknown error %1").arg(err)); break;
        }

        return;
    }

#if 0
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    for(int n=0; n<256; n++)
        out << buf[n];
#endif

    QByteArray ba;
    ba.clear();
    for(int n=0; n<FTDI_MAX_EEPROM_SIZE; n++)
        ba.append(buf[n]);
    QHexEdit *hex = new QHexEdit();
    hex->setWindowIcon(QIcon(ICON_PROGRAMM));
    hex->setMinimumSize(800, 320);
    hex->setReadOnly(true);
    hex->setData(ba);

    vbox->addWidget(hex);
    frame->setLayout(vbox);
    frame->show();

    ftdi_usb_close(&ftdi);
    emit info("test: OK");
}
//--------------------------------------------------------------------------------
bool MainBox::erase_eerpom(int VID, int PID)
{
    ftdi_context ftdi;
    int err;
    bool ok = false;

    emit info(QString("erase_eerpom(%1:%2)")
              .arg(VID, 0, 16)
              .arg(PID, 0, 16));
    ftdi_init(&ftdi);
    //int err = ftdi_usb_open(&ftdi, 0x0403, 0x6010);
    err = ftdi_usb_open(&ftdi, VID, PID);
    if(err)
    {
        emit error(QString("erase_eerpom: device %1:%2 not open!")
                   .arg(VID, 0, 16)
                   .arg(PID, 0, 16));
        return false;
    }

    err = ftdi_erase_eeprom(&ftdi);
    if(err)
    {
        emit error(QString("erase_eerpom: ftdi_erase_eeprom return %1")
                   .arg(err));
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
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0))
    {
        at93c56->test();
        at93c56->close();
    }
    at93c56->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_i2c_write(void)
{
    emit info("test_i2c_write");
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0))
    {
        at93c56->test_write();
        at93c56->close();
    }
    at93c56->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_i2c_read(void)
{
    emit info("test_i2c_read");
    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);

    if(at93c56->open(0))
    {
        at93c56->test_read();
        at93c56->close();
    }
    at93c56->deleteLater();

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_at93c56(void)
{
    emit info("test_at93c56 begin");

    AT93C56 *at93c56 = new AT93C56(get_i2c_freq(), this);
    if(!at93c56->test()) emit error("test error");
    at93c56->deleteLater();

    emit info("test_at93c56 end");
}
//--------------------------------------------------------------------------------
void MainBox::test_MCP4922(void)
{
    FT_STATUS ftStatus = FT_OK;

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
            ftStatus = spi->set_voltage(n);
            if(ftStatus != FT_OK) emit error("set_voltage error");
            QCoreApplication::processEvents();
            Sleeper::msleep(100);
        }
        for(float n=3.132f; n>0.0f; n-=0.1f)
        {
            ftStatus = spi->set_voltage(n);
            if(ftStatus != FT_OK) emit error("set_voltage error");
            QCoreApplication::processEvents();
            Sleeper::msleep(100);
        }
    }
    spi->deleteLater();

    emit info("test_MCP4922 end");
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
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
