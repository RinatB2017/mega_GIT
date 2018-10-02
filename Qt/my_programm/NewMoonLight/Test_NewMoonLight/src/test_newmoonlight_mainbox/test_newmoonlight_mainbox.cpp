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
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QtMath>
#include <QMenu>

#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QSpinBox>
#include <QTimer>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "ui_test_newmoonlight_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5_fix_baudrate.hpp"
#include "test_newmoonlight_mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "led.hpp"
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
    if(serialBox)   serialBox->deleteLater();

    foreach (Led *led, leds) {
        led->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    createSerialBox();

    init_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->widget->setFixedSize(400, 400);
    ui->widget->setStyleSheet("background:white;");

    //---
    int index = 1;
    center_x = ui->widget->width() / 2;
    center_y = ui->widget->height() / 2;
    center_r = ui->widget->width() / 14;
    led_r = ui->widget->width() / 17;
    min_r = center_r + led_r + 10;
    max_r = ui->widget->width() / 2 - 20;
    min_angle = 270; //-30.0f;
    max_angle = -90; //330.0f;
    inc_r = (int)((max_r - min_r) / 2.4);
    qreal angle = min_angle;
    int i = 0;
    while(angle > max_angle)
    {
        for(int n=0; n<3; n++)
        {
            calc_line(center_x,
                      center_y,
                      angle,
                      inc_r * (n + 1),
                      &temp_x,
                      &temp_y);
            Led *led = new Led(led_r*2, led_r*2, ui->widget);

            led->setProperty("index", index);
            leds.append(led);
            led->move(temp_x-led_r,
                      temp_y-led_r);
#ifndef QT_DEBUG
            led->lock();
#endif
            index++;
        }
        i++;
        angle -= 60.0;
    }

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::calc_line(qreal center_x,
                        qreal center_y,
                        qreal angle,
                        qreal radius,
                        qreal *end_x,
                        qreal *end_y)
{
    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
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
    serialBox = new SerialBox5_fix_baudrate(this, "RS232", "RS232");
    serialBox->set_fix_baudrate(BAUDRATE);
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->setSpacing(0);
    ui->serial_layout->setMargin(0);

    connect(this,       SIGNAL(send(QByteArray)),   serialBox,  SLOT(input(QByteArray)));
    connect(serialBox,  SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
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
uint16_t MainBox::get_value(NewMoonLightPacket *packet, uint16_t address)
{
    union UINT16 {
        uint16_t value;
        struct {
            uint8_t c:4;
            uint8_t d:4;
            uint8_t a:4;
            uint8_t b:4;
        } bytes;
    };

    union UINT16 temp;
    temp.value = address;

    return packet->body.data[temp.bytes.a][temp.bytes.b] << 8 | packet->body.data[temp.bytes.c][temp.bytes.d];
}
//--------------------------------------------------------------------------------
void MainBox::analize(void)
{
    clean_data = QByteArray::fromHex(data_rs232);

    if(clean_data.size() != sizeof(NewMoonLightPacket))
    {
        emit error(QString("read %1 bytes [%2]")
                   .arg(clean_data.size())
                   .arg(sizeof(NewMoonLightPacket)));
        return;
    }
    NewMoonLightPacket *packet = (NewMoonLightPacket *)clean_data.data();
    emit debug(QString("address 0x%1").arg(packet->body.address));
    emit debug(QString("command 0x%1").arg(packet->body.command));
    emit debug(QString("cnt_data %1").arg(packet->body.cnt_data));

    int cmd = packet->body.command;
    switch(cmd)
    {
    case CMD_GET:   f_get();    break;
    case CMD_SET:   f_set();    break;
    default:
        emit error(QString("unknown cmd %1").arg(cmd));
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_get(void)
{
    NewMoonLightPacket packet;

    packet.body.address = 0;
    packet.body.cnt_data = (MAX_LED * 2);
    packet.body.command = CMD_GET;
    packet.body.data[0][0] = 5; //FIXME

    QByteArray temp;
    temp.append(':');
    for(unsigned int n=0; n<sizeof(NewMoonLightPacket); n++)
    {
        temp.append(convert_data_to_ascii(packet.buf[n]));
    }
    temp.append('\n');

    emit debug(temp);
    emit info(QString("sending %1 bytes").arg(temp.length()));
    emit send(temp);
}
//--------------------------------------------------------------------------------
void MainBox::f_set(void)
{
    NewMoonLightPacket *packet = (NewMoonLightPacket *)clean_data.data();
    if(packet->body.cnt_data != (MAX_LED * 2))
    {
        emit error(QString("error cnt_data %1").arg(packet->body.cnt_data));
        return;
    }

    leds.at(0)->set_color(get_value(packet,   0x2112));
    leds.at(1)->set_color(get_value(packet,   0x4151));
    leds.at(2)->set_color(get_value(packet,   0x3102));

    leds.at(3)->set_color(get_value(packet,   0x2255));
    leds.at(4)->set_color(get_value(packet,   0x3245));
    leds.at(5)->set_color(get_value(packet,   0x2535));

    leds.at(6)->set_color(get_value(packet,   0x4223));
    leds.at(7)->set_color(get_value(packet,   0x5213));
    leds.at(8)->set_color(get_value(packet,   0x0333));

    leds.at(9)->set_color(get_value(packet,   0x4314));
    leds.at(10)->set_color(get_value(packet,  0x5334));
    leds.at(11)->set_color(get_value(packet,  0x0424));

    leds.at(12)->set_color(get_value(packet,  0x5415));
    leds.at(13)->set_color(get_value(packet,  0x0510));
    leds.at(14)->set_color(get_value(packet,  0x4400));

    leds.at(15)->set_color(get_value(packet,  0x2001));
    leds.at(16)->set_color(get_value(packet,  0x4011));
    leds.at(17)->set_color(get_value(packet,  0x3050));

    NewMoonLightPacket out_packet;
    for(unsigned int n=0; n<sizeof(NewMoonLightPacket); n++)
    {
        out_packet.buf[n] = packet->buf[n];
    }

    QByteArray temp;
    temp.append(':');
    for(unsigned int n=0; n<sizeof(NewMoonLightPacket); n++)
    {
        temp.append(convert_data_to_ascii(out_packet.buf[n]));
    }
    temp.append('\n');

    emit debug(temp);
    emit info(QString("sending %1 bytes").arg(temp.length()));
    emit send(temp);
}
//--------------------------------------------------------------------------------
int MainBox::get_address(uint16_t value)
{
    int addr = -1;
    switch(value)
    {
    case 0x2112:    addr = 0;   break;
    case 0x4151:    addr = 1;   break;
    case 0x3102:    addr = 2;   break;

    case 0x2255:    addr = 3;   break;
    case 0x3245:    addr = 4;   break;
    case 0x2535:    addr = 5;   break;

    case 0x4223:    addr = 6;   break;
    case 0x5213:    addr = 7;   break;
    case 0x0333:    addr = 8;   break;

    case 0x4314:    addr = 9;   break;
    case 0x5334:    addr = 10;  break;
    case 0x0424:    addr = 11;  break;

    case 0x5415:    addr = 12;  break;
    case 0x0510:    addr = 13;  break;
    case 0x4400:    addr = 14;  break;

    case 0x2001:    addr = 15;  break;
    case 0x4011:    addr = 16;  break;
    case 0x3050:    addr = 17;  break;

    default:
        break;
    }
    return addr;
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
    emit debug("test_0()");
    for(int n=0; n<18; n++)
    {
        leds[n]->set_hot_color(128);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit debug("test_1()");
    for(int n=0; n<18; n++)
    {
        leds[n]->set_cold_color(128);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit debug("test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit debug("test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit debug("test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit debug("test_5()");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
