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
    connect(ui->btn_read,   &QToolButton::clicked,  this,   &MainBox::f_read);
    connect(ui->btn_write,  &QToolButton::clicked,  this,   &MainBox::f_write);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainBox::f_close);
    connect(ui->btn_test,   &QToolButton::clicked,  this,   &MainBox::f_test);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

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

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());    //TODO странно
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
void MainBox::f_open(void)
{
    emit trace(Q_FUNC_INFO);

    uint16_t vid = get_VID();
    uint16_t pid = get_PID();
    emit info(QString("%1:%2")
              .arg(vid, 4, 16, QChar('0'))
              .arg(pid, 4, 16, QChar('0')));

    ftdi = (struct ftdi_context *)malloc(sizeof(struct ftdi_context));
    if (ftdi == NULL)
    {
        emit error("Malloc return NULL");
        return;
    }

    if (ftdi_init(ftdi) != 0)
    {
        emit error("ftdi_init() != 0");
        free(ftdi);
        return;
    }

    int ret;
    if ((ret = ftdi_usb_open(ftdi, vid, pid)) < 0)
    {
        emit error(QString("%1").arg(ftdi_get_error_string(ftdi)));
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_read(void)
{
    emit trace(Q_FUNC_INFO);

    int ret;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE] = { 0 };

    if(ftdi == nullptr)
    {
        emit error("ftdi not init");
        return;
    }

    ret = ftdi_read_eeprom(ftdi);
    if(ret != 0)
    {
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
        return;
    }

    ret = ftdi_eeprom_decode(ftdi, 1);
    if(ret != 0)
    {
        switch(ret)
        {
        case 0:
            emit error("all fine");
            break;
        case -1:
            emit error("something went wrong");
            break;
        }
    }

    ret = ftdi_get_eeprom_buf(ftdi, buf, FTDI_MAX_EEPROM_SIZE);
    if(ret != 0)
    {
        switch(ret)
        {
        case 0:
            emit info("all fine");
            break;
        case -1:
            emit error("struct ftdi_contxt or ftdi_eeprom missing");
            break;
        case -2:
            emit error("Not enough room to store eeprom");
            break;
        }
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
void MainBox::f_write(void)
{
    emit trace(Q_FUNC_INFO);

    if(ftdi == nullptr)
    {
        emit error("ftdi not init");
        return;
    }

    int ret;
    unsigned char buf[FTDI_MAX_EEPROM_SIZE] = { 0 };

    char manufacturer[1024] = { 0 };
    char product[1024] = { 0 };
    char serial[1024] = { 0 };
    ret = ftdi_eeprom_initdefaults(ftdi, manufacturer, product, serial);
    if(ret != 0)
    {
        switch(ret)
        {
        case 0:
            emit info("all fine");
            break;
        case -1:
            emit error("No struct ftdi_context");
            break;
        case -2:
            emit error("No struct ftdi_eeprom");
            break;
        case -3:
            emit error("No connected device or device not yet opened");
            break;
        }
        return;
    }

    emit info(QString("manufacturer [%1]").arg(manufacturer));
    emit info(QString("product [%1]").arg(product));
    emit info(QString("serial [%1]").arg(serial));

    buf[0] = 6;

    ret = ftdi_set_eeprom_buf(ftdi, buf, 10);
    if(ret != 0)
    {
        switch(ret)
        {
        case 0:
            emit info("All fine");
            break;
        case -1:
            emit error("struct ftdi_contxt or ftdi_eeprom of buf missing");
            break;
        }
        return;
    }

    ret = ftdi_write_eeprom(ftdi);
    if(ret != 0)
    {
        switch(ret)
        {
        case -1:
            emit error("read failed");
            break;
        case -2:
            emit error("USB device unavailable");
            break;
        case -3:
            emit error("EEPROM not initialized for the connected device");
            break;
        default:
            emit error(QString("ftdi_write_eeprom return %1").arg(ret));
            break;
        }

        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_close(void)
{
    emit trace(Q_FUNC_INFO);

    if(ftdi == nullptr)
    {
        emit error("ftdi not init");
        return;
    }

    int ret;
    ret = ftdi_usb_close(ftdi);
    switch(ret)
    {
    case 0:
        emit info("all fine");
        break;
    case -1:
        emit error("usb_release failed");
        break;
    case -3:
        emit error("ftdi context invalid");
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
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
