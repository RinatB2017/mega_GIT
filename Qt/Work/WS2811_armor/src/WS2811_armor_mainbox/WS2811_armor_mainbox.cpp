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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "ui_WS2811_armor_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "WS2811_armor_mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
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
    save_widgets("WS2811_armor");
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    createSerialBox();

    init_widgets();

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    load_widgets("WS2811_armor");
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->sb_brightness->setRange(1, 150);
    ui->sb_delay_ms->setRange(0, 1000);

    connect(ui->btn_set_brightness, SIGNAL(clicked(bool)),  this,   SLOT(set_brightness()));
    connect(ui->btn_set_delay_ms,   SIGNAL(clicked(bool)),  this,   SLOT(set_delay_ms()));

    connect(ui->btn_cmd_1,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_1()));
    connect(ui->btn_cmd_2,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_2()));
    connect(ui->btn_cmd_3,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_3()));
    connect(ui->btn_cmd_4,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_4()));
    connect(ui->btn_cmd_5,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_5()));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

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
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    ui->serial_widget->set_caption("RS232");
    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug(QString("read_data [%1]").arg(ba.data()));
    for(int n=0; n<ba.size(); n++)
    {
        char s = ba.at(n);
        switch (s)
        {
        case ':':
            data_rs232.clear();
            break;

        case 0x0A:
        case 0x0D:
            analize();
            break;

        default:
            data_rs232.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::analize(void)
{
    clean_data = QByteArray::fromHex(data_rs232);
    is_ready = true;
}
//--------------------------------------------------------------------------------
QString MainBox::convert_data_to_ascii(uint8_t data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    QString hi_str;
    switch(hi)
    {
    case 0x00: hi_str="0"; break;
    case 0x01: hi_str="1"; break;
    case 0x02: hi_str="2"; break;
    case 0x03: hi_str="3"; break;
    case 0x04: hi_str="4"; break;
    case 0x05: hi_str="5"; break;
    case 0x06: hi_str="6"; break;
    case 0x07: hi_str="7"; break;
    case 0x08: hi_str="8"; break;
    case 0x09: hi_str="9"; break;
    case 0x0A: hi_str="A"; break;
    case 0x0B: hi_str="B"; break;
    case 0x0C: hi_str="C"; break;
    case 0x0D: hi_str="D"; break;
    case 0x0E: hi_str="E"; break;
    case 0x0F: hi_str="F"; break;
    default: break;
    }

    QString lo_str;
    switch(lo)
    {
    case 0x00: lo_str="0"; break;
    case 0x01: lo_str="1"; break;
    case 0x02: lo_str="2"; break;
    case 0x03: lo_str="3"; break;
    case 0x04: lo_str="4"; break;
    case 0x05: lo_str="5"; break;
    case 0x06: lo_str="6"; break;
    case 0x07: lo_str="7"; break;
    case 0x08: lo_str="8"; break;
    case 0x09: lo_str="9"; break;
    case 0x0A: lo_str="A"; break;
    case 0x0B: lo_str="B"; break;
    case 0x0C: lo_str="C"; break;
    case 0x0D: lo_str="D"; break;
    case 0x0E: lo_str="E"; break;
    case 0x0F: lo_str="F"; break;
    default: break;
    }

    return (hi_str+lo_str);
}
//--------------------------------------------------------------------------------
uint8_t MainBox::convert_ascii_to_value(char hi, char lo)
{
    uint8_t b_hi = 0;
    uint8_t b_lo = 0;

    //---
    switch(hi)
    {
    case '0':  b_hi = 0x0;  break;
    case '1':  b_hi = 0x1;  break;
    case '2':  b_hi = 0x2;  break;
    case '3':  b_hi = 0x3;  break;
    case '4':  b_hi = 0x4;  break;
    case '5':  b_hi = 0x5;  break;
    case '6':  b_hi = 0x6;  break;
    case '7':  b_hi = 0x7;  break;
    case '8':  b_hi = 0x8;  break;
    case '9':  b_hi = 0x9;  break;
    case 'A':  b_hi = 0xA;  break;
    case 'B':  b_hi = 0xB;  break;
    case 'C':  b_hi = 0xC;  break;
    case 'D':  b_hi = 0xD;  break;
    case 'E':  b_hi = 0xE;  break;
    case 'F':  b_hi = 0xF;  break;
    default:
        break;
    }
    //---
    switch(lo)
    {
    case '0':  b_lo = 0x0;  break;
    case '1':  b_lo = 0x1;  break;
    case '2':  b_lo = 0x2;  break;
    case '3':  b_lo = 0x3;  break;
    case '4':  b_lo = 0x4;  break;
    case '5':  b_lo = 0x5;  break;
    case '6':  b_lo = 0x6;  break;
    case '7':  b_lo = 0x7;  break;
    case '8':  b_lo = 0x8;  break;
    case '9':  b_lo = 0x9;  break;
    case 'A':  b_lo = 0xA;  break;
    case 'B':  b_lo = 0xB;  break;
    case 'C':  b_lo = 0xC;  break;
    case 'D':  b_lo = 0xD;  break;
    case 'E':  b_lo = 0xE;  break;
    case 'F':  b_lo = 0xF;  break;
    default:
        break;
    }
    //---
    uint8_t r_byte = (b_hi << 4) | b_lo;
    return r_byte;
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
            typedef void (MainBox::*my_mega_function)(void);
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
void MainBox::test_0(void)
{
    emit info("test_0");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("test_1");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("test_2");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("test_3");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("test_4");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("test_5");
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::command(uint8_t cmd, uint16_t data)
{
    clean_data.clear();
    data_rs232.clear();
    is_ready = false;

    //---
    PACKET packet;
    packet.cmd_8 = cmd;
    packet.data = data;

    QByteArray temp;
    temp.clear();
    temp.append((char *)&packet, sizeof(PACKET));

    QByteArray ba;
    ba.clear();
    ba.append(":");
    ba.append(temp.toHex().toUpper());
    ba.append("\n");
    //---

    emit debug(ba.data());
    emit send(ba);

    wait(1000);
    if(is_ready == false)
    {
        emit error("No data!");
        return;
    }

    if(clean_data.isEmpty())
    {
        emit error("clean_data is empty!");
        return;
    }
    if(clean_data.length() != 3)
    {
        emit error(QString("receive [%1]").arg(clean_data.toHex().data()));
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::set_delay_ms(void)
{
    command(CMD_SET_DELAY_MS, ui->sb_delay_ms->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_brightness(void)
{
    command(CMD_SET_BRIGHTNESS, ui->sb_brightness->value());
}
//--------------------------------------------------------------------------------
void MainBox::cmd_1(void)
{
    command(CMD_01, 0);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_2(void)
{
    command(CMD_02, 0);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_3(void)
{
    command(CMD_03, 0);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_4(void)
{
    command(CMD_04, 0);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_5(void)
{
    command(CMD_05, 0);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
