/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QVariant>
#include <QTimer>
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QPushButton>
#include <QToolBar>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QWidget>
#include <QLabel>
#include <QTimer>
//--------------------------------------------------------------------------------
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "b588_serialport_test.hpp"
#include "b588_packets.hpp"
#include "B588_fram.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_B588_test::SerialPort_B588_test()
{
    input.clear();
    output.clear();

    is_open = false;

    setting_U = 0;
    setting_I = 0;

    current_U = 0;
    current_I = 0;

    temperature = MIN_TEMPERATURE;

    dac_point = 0;

    regenerate_dac_U_values(0);
    regenerate_dac_I_values(0);
    init_fram();
    QTimer::singleShot(1000, this, SLOT(create_fake_toolbar()));
}
//--------------------------------------------------------------------------------
SerialPort_B588_test::~SerialPort_B588_test()
{
#ifdef QT_DEBUG
    qDebug() << "~SerialPort_B588_test()";
#endif
    //if(timer) timer->deleteLater();
    //if(fram) fram->deleteLater();
    //if(sb_noise) sb_noise->deleteLater();
    //if(sb_R) sb_R->deleteLater();
    //if(btn_set_noice) btn_set_noice->deleteLater();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::init_fram(void)
{
    fram = new B588_Fram(this);
    connect(fram, SIGNAL(info(QString)),    this, SIGNAL(info(QString)));
    connect(fram, SIGNAL(debug(QString)),   this, SIGNAL(debug(QString)));
    connect(fram, SIGNAL(error(QString)),   this, SIGNAL(error(QString)));
    connect(fram, SIGNAL(trace(QString)), this, SIGNAL(trace(QString)));

    fram->init();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::create_fake_toolbar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(QApplication::activeWindow());
    Q_CHECK_PTR(mw);

    sb_noise = new QSpinBox();
    sb_noise->setMinimum(0);
    sb_noise->setMaximum(5);

    sb_R = new QDoubleSpinBox();
    sb_R->setMinimum(0.0000001f);
    sb_R->setMaximum(65535);
    sb_R->setSingleStep(0.0000001f);
    sb_R->setDecimals(7);

    btn_set_noice = new QPushButton("regen");
    connect(btn_set_noice, SIGNAL(pressed()), this, SLOT(regenerate_all()));

    QToolBar *toolBar = new QToolBar(tr("fake_toolbar B5-88"));
    toolBar->setObjectName("fake_toolbar_B588");
    toolBar->addWidget(new QLabel(tr("fake Б5-88")));
    toolBar->addSeparator();
    toolBar->addWidget(new QLabel(tr("Noise")));
    toolBar->addWidget(sb_noise);
    toolBar->addWidget(new QLabel(tr("R")));
    toolBar->addWidget(sb_R);
    toolBar->addWidget(btn_set_noice);

    mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::update(void)
{
    // нагрев током
    temperature += (current_I / MAX_CURRENT_B588);

    // вентиляция
    temperature -= 0.1;

    if(temperature > MAX_TEMPERATURE) temperature = MAX_TEMPERATURE;
    if(temperature < MIN_TEMPERATURE) temperature = MIN_TEMPERATURE;
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::regenerate_all(void)
{
    int noise = sb_noise->value();
    regenerate_dac_U_values(noise);
    regenerate_dac_I_values(noise);
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::regenerate_dac_U_values(int noise_value)
{
#if 1
    Q_UNUSED(noise_value)
    dac_U[0]=58;
    dac_U[1]=1073;
    dac_U[2]=2065;
    dac_U[3]=3056;
    dac_U[4]=4047;
    dac_U[5]=5039;
    dac_U[6]=6031;
    dac_U[7]=7022;
    dac_U[8]=8014;
    dac_U[9]=9005;
    dac_U[10]=9998;
    dac_U[11]=10991;
    dac_U[12]=11983;
    dac_U[13]=12975;
    dac_U[14]=13966;
    dac_U[15]=14958;
    dac_U[16]=15949;
    dac_U[17]=16941;
    dac_U[18]=17933;
    dac_U[19]=18926;
    dac_U[20]=19917;
    dac_U[21]=20909;
    dac_U[22]=21901;
    dac_U[23]=22893;
    dac_U[24]=23885;
    dac_U[25]=24877;
    dac_U[26]=25869;
    dac_U[27]=26862;
    dac_U[28]=27854;
    dac_U[29]=28847;
    dac_U[30]=29838;
    dac_U[31]=30829;
    dac_U[32]=31823;
    dac_U[33]=32815;
    dac_U[34]=33808;
    dac_U[35]=34801;
    dac_U[36]=35791;
    dac_U[37]=36783;
    dac_U[38]=37775;
    dac_U[39]=38767;
    dac_U[40]=39759;
    dac_U[41]=40751;
    dac_U[42]=41743;
    dac_U[43]=42735;
    dac_U[44]=43727;
    dac_U[45]=44718;
    dac_U[46]=45709;
    dac_U[47]=46701;
    dac_U[48]=47693;
    dac_U[49]=48685;
    dac_U[50]=49677;
    dac_U[51]=50668;
    dac_U[52]=51660;
    dac_U[53]=52650;
    dac_U[54]=53642;
    dac_U[55]=54634;
    dac_U[56]=55624;
    dac_U[57]=56618;
    dac_U[58]=57609;
    dac_U[59]=59944;
#else
    for(uint16_t n=0; n<0xFFFF; n++)
    {
        if(noise_value == 0)
        {
            dac_U[n] = n;
        }
        else
        {
            int noise = qrand() % noise_value;
            int new_value = n + noise;
            if(new_value > 0xFFFF) new_value = 0xFFFF;
            dac_U[n] = (uint16_t)new_value;
        }
    }
#endif
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::regenerate_dac_I_values(int noise_value)
{
#if 1
    Q_UNUSED(noise_value)
    dac_I[0]=226;
    dac_I[1]=1208;
    dac_I[2]=2193;
    dac_I[3]=3178;
    dac_I[4]=4163;
    dac_I[5]=5147;
    dac_I[6]=6134;
    dac_I[7]=7120;
    dac_I[8]=8107;
    dac_I[9]=9094;
    dac_I[10]=10081;
    dac_I[11]=11068;
    dac_I[12]=12053;
    dac_I[13]=13040;
    dac_I[14]=14028;
    dac_I[15]=15014;
    dac_I[16]=16001;
    dac_I[17]=16989;
    dac_I[18]=17976;
    dac_I[19]=18964;
    dac_I[20]=19952;
    dac_I[21]=20940;
    dac_I[22]=21927;
    dac_I[23]=22913;
    dac_I[24]=23900;
    dac_I[25]=24887;
    dac_I[26]=25873;
    dac_I[27]=26860;
    dac_I[28]=27847;
    dac_I[29]=28833;
    dac_I[30]=29820;
    dac_I[31]=30807;
    dac_I[32]=31794;
    dac_I[33]=32780;
    dac_I[34]=33767;
    dac_I[35]=34752;
    dac_I[36]=35739;
    dac_I[37]=36725;
    dac_I[38]=37710;
    dac_I[39]=38696;
    dac_I[40]=39683;
    dac_I[41]=40670;
    dac_I[42]=41657;
    dac_I[43]=42642;
    dac_I[44]=43627;
    dac_I[45]=44613;
    dac_I[46]=45598;
    dac_I[47]=46583;
    dac_I[48]=47568;
    dac_I[49]=48553;
#else
    for(uint16_t n=0; n<0xFFFF; n++)
    {
        if(noise_value == 0)
        {
            dac_I[n] = n;
        }
        else
        {
            int noise = qrand() % noise_value;
            int new_value = n + noise;
            if(new_value > 0xFFFF) new_value = 0xFFFF;
            dac_I[n] = (uint16_t)new_value;
        }
    }
#endif
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::isOpen(void)
{
#ifdef QT_DEBUG
    qDebug() << "isOpen" << is_open;
#endif
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#else
    Q_UNUSED(mode)
#endif
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#else
    Q_UNUSED(info)
#endif
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#else
    Q_UNUSED(speed)
#endif
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_B588_test::readAll(void)
{
    return output;
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::write(QByteArray data)
{
    if(data.isEmpty()) return;

    input.clear();
    input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case B588_CMD_0x01: prepare_answer_01(); break;
    case B588_CMD_0x02: prepare_answer_02(); break;
    case B588_CMD_0x03: prepare_answer_03(); break;
    case B588_CMD_0x04: prepare_answer_04(); break;
    case B588_CMD_0x05: prepare_answer_05(); break;
    case B588_CMD_0x06: prepare_answer_06(); break;
    case B588_CMD_0x07: prepare_answer_07(); break;
    case B588_CMD_0x08: prepare_answer_08(); break;
    case B588_CMD_0x09: prepare_answer_09(); break;
    case B588_CMD_0x0A: prepare_answer_0A(); break;
    case B588_CMD_0x0B: prepare_answer_0B(); break;
    case B588_CMD_0x0C: prepare_answer_0C(); break;
    case B588_CMD_0x0D: prepare_answer_0D(); break;
    case B588_CMD_0x0E: prepare_answer_0E(); break;
    case B588_CMD_0x0F: prepare_answer_0F(); break;

    case B588_CMD_0x11: prepare_answer_11(); break;
    case B588_CMD_0x12: prepare_answer_12(); break;
    case B588_CMD_0x13: prepare_answer_13(); break;
    case B588_CMD_0x14: prepare_answer_14(); break;
    case B588_CMD_0x15: prepare_answer_15(); break;
    case B588_CMD_0x16: prepare_answer_16(); break;
    case B588_CMD_0x17: prepare_answer_17(); break;

    case B588_CMD_0x28: prepare_answer_28(); break;
    case B588_CMD_0x29: prepare_answer_29(); break;
    case B588_CMD_0x2A: prepare_answer_2A(); break;
    case B588_CMD_0x2B: prepare_answer_2B(); break;
    case B588_CMD_0x2C: prepare_answer_2C(); break;
    case B588_CMD_0x2D: prepare_answer_2D(); break;
    case B588_CMD_0x2E: prepare_answer_2E(); break;
    case B588_CMD_0x2F: prepare_answer_2F(); break;

    case B588_CMD_0x31: prepare_answer_31(); break;
    case B588_CMD_0x32: prepare_answer_32(); break;
    case B588_CMD_0x33: prepare_answer_33(); break;
    case B588_CMD_0x34: prepare_answer_34(); break;
    case B588_CMD_0x35: prepare_answer_35(); break;
    case B588_CMD_0x36: prepare_answer_36(); break;
    case B588_CMD_0x37: prepare_answer_37(); break;
    case B588_CMD_0x38: prepare_answer_38(); break;
    case B588_CMD_0x39: prepare_answer_39(); break;
    case B588_CMD_0x3A: prepare_answer_3A(); break;
    case B588_CMD_0x3B: prepare_answer_3B(); break;
    case B588_CMD_0x3C: prepare_answer_3C(); break;
    case B588_CMD_0x3D: prepare_answer_3D(); break;
    case B588_CMD_0x3E: prepare_answer_3E(); break;
    case B588_CMD_0x3F: prepare_answer_3F(); break;

    case B588_CMD_0x41: prepare_answer_41(); break;
    case B588_CMD_0x42: prepare_answer_42(); break;
    case B588_CMD_0x43: prepare_answer_43(); break;
    case B588_CMD_0x44: prepare_answer_44(); break;
    case B588_CMD_0x46: prepare_answer_46(); break;
    case B588_CMD_0x47: prepare_answer_47(); break;
    case B588_CMD_0x48: prepare_answer_48(); break;
    case B588_CMD_0x49: prepare_answer_49(); break;
    case B588_CMD_0x4A: prepare_answer_4A(); break;

    case B588_CMD_0x54: prepare_answer_54(); break;
    case B588_CMD_0x55: prepare_answer_55(); break;
    case B588_CMD_0x56: prepare_answer_56(); break;
    case B588_CMD_0x57: prepare_answer_57(); break;
    case B588_CMD_0x58: prepare_answer_58(); break;
    case B588_CMD_0x59: prepare_answer_59(); break;
    case B588_CMD_0x5A: prepare_answer_5A(); break;
    case B588_CMD_0x5B: prepare_answer_5B(); break;
    case B588_CMD_0x5C: prepare_answer_5C(); break;
    case B588_CMD_0x5D: prepare_answer_5D(); break;
    case B588_CMD_0x5E: prepare_answer_5E(); break;
    case B588_CMD_0x5F: prepare_answer_5F(); break;

    case B588_CMD_0x61: prepare_answer_61(); break;
    case B588_CMD_0x62: prepare_answer_62(); break;
    case B588_CMD_0x63: prepare_answer_63(); break;
    case B588_CMD_0x64: prepare_answer_64(); break;
    case B588_CMD_0x65: prepare_answer_65(); break;
    case B588_CMD_0x66: prepare_answer_66(); break;
    case B588_CMD_0x67: prepare_answer_67(); break;
    case B588_CMD_0x68: prepare_answer_68(); break;
    case B588_CMD_0x69: prepare_answer_69(); break;
    case B588_CMD_0x6A: prepare_answer_6A(); break;
    case B588_CMD_0x6B: prepare_answer_6B(); break;
    case B588_CMD_0x6C: prepare_answer_6C(); break;
    case B588_CMD_0x6D: prepare_answer_6D(); break;
    case B588_CMD_0x6E: prepare_answer_6E(); break;
    case B588_CMD_0x6F: prepare_answer_6F(); break;

    case B588_CMD_0x71: prepare_answer_71(); break;
    case B588_CMD_0x72: prepare_answer_72(); break;
    case B588_CMD_0x73: prepare_answer_73(); break;
    case B588_CMD_0x75: prepare_answer_75(); break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::write(const char *data, int len)
{
#ifdef QT_DEBUG
    qDebug() << "write";
#endif

    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_01(void)
{
    B588_CMD_01_QUESTION *question = (B588_CMD_01_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_01_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_01: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_02(void)
{
    B588_CMD_02_QUESTION *question = (B588_CMD_02_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_02_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_02: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_03(void)
{
    B588_CMD_03_QUESTION *question = (B588_CMD_03_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_03_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_03: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_04(void)
{
    B588_CMD_04_QUESTION *question = (B588_CMD_04_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_04_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_04: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_05(void)
{
    B588_CMD_05_QUESTION *question = (B588_CMD_05_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_05_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_05: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_06(void)
{
    B588_CMD_06_QUESTION *question = (B588_CMD_06_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_06_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_06: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_07(void)
{
    B588_CMD_07_QUESTION *question = (B588_CMD_07_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_07_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_07: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_08(void)
{
    B588_CMD_08_QUESTION *question = (B588_CMD_08_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_08_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_08: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_09(void)
{
    B588_CMD_09_QUESTION *question = (B588_CMD_09_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_09_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_09: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0A(void)
{
    B588_CMD_0A_QUESTION *question = (B588_CMD_0A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0B(void)
{
    B588_CMD_0B_QUESTION *question = (B588_CMD_0B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0C(void)
{
    B588_CMD_0C_QUESTION *question = (B588_CMD_0C_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0C_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0C: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0D(void)
{
    B588_CMD_0D_QUESTION *question = (B588_CMD_0D_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0D_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0D: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0E(void)
{
    B588_CMD_0E_QUESTION *question = (B588_CMD_0E_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0E_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0E: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_0F(void)
{
    B588_CMD_0F_QUESTION *question = (B588_CMD_0F_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_0F_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_0F: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_11(void)
{
    B588_CMD_11_QUESTION *question = (B588_CMD_11_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_11_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_11: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_12(void)
{
    B588_CMD_12_QUESTION *question = (B588_CMD_12_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_12_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_12: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_13(void)
{
    B588_CMD_13_QUESTION *question = (B588_CMD_13_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_13_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_13: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_14(void)
{
    B588_CMD_14_QUESTION *question = (B588_CMD_14_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_14_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_14: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_15(void)
{
    B588_CMD_15_QUESTION *question = (B588_CMD_15_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_15_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_15: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_16(void)
{
    B588_CMD_16_QUESTION *question = (B588_CMD_16_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_16_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_16: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_17(void)
{
    B588_CMD_17_QUESTION *question = (B588_CMD_17_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_17_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_17: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_20(void)
{
    B588_CMD_20_QUESTION *question = (B588_CMD_20_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_20_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_20: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_21(void)
{
    B588_CMD_21_QUESTION *question = (B588_CMD_21_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_21_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_21: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_22(void)
{
    B588_CMD_22_QUESTION *question = (B588_CMD_22_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_22_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_22: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_23(void)
{
    B588_CMD_23_QUESTION *question = (B588_CMD_23_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_23_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_23: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_24(void)
{
    B588_CMD_24_QUESTION *question = (B588_CMD_24_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_24_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_24: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_25(void)
{
    B588_CMD_25_QUESTION *question = (B588_CMD_25_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_25_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_25: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_26(void)
{
    B588_CMD_26_QUESTION *question = (B588_CMD_26_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_26_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_26: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_27(void)
{
    B588_CMD_27_QUESTION *question = (B588_CMD_27_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_27_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_27: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_28(void)
{
    B588_CMD_28_QUESTION *question = (B588_CMD_28_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_28_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_28: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_29(void)
{
    B588_CMD_29_QUESTION *question = (B588_CMD_29_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_29_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_29: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2A(void)
{
    B588_CMD_2A_QUESTION *question = (B588_CMD_2A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2B(void)
{
    B588_CMD_2B_QUESTION *question = (B588_CMD_2B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2C(void)
{
    B588_CMD_2C_QUESTION *question = (B588_CMD_2C_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2C_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2C: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2D(void)
{
    B588_CMD_2D_QUESTION *question = (B588_CMD_2D_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2D_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2D: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2E(void)
{
    B588_CMD_2E_QUESTION *question = (B588_CMD_2E_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2E_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2E: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_2F(void)
{
    B588_CMD_2F_QUESTION *question = (B588_CMD_2F_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_2F_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_2F: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_31(void)
{
    B588_CMD_31_QUESTION *question = (B588_CMD_31_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_31_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_31: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_32(void)
{
    B588_CMD_32_QUESTION *question = (B588_CMD_32_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_32_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_32: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_33(void)
{
    B588_CMD_33_QUESTION *question = (B588_CMD_33_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_33_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_33: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_34(void)
{
    B588_CMD_34_QUESTION *question = (B588_CMD_34_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_34_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_34: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_35(void)
{
    B588_CMD_35_QUESTION *question = (B588_CMD_35_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_35_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_35: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_36(void)
{
    B588_CMD_36_QUESTION *question = (B588_CMD_36_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_36_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_36: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_37(void)
{
    B588_CMD_37_QUESTION *question = (B588_CMD_37_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_37_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_37: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_38(void)
{
    B588_CMD_38_QUESTION *question = (B588_CMD_38_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_38_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_38: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_39(void)
{
    B588_CMD_39_QUESTION *question = (B588_CMD_39_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_39_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_39: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3A(void)
{
    B588_CMD_3A_QUESTION *question = (B588_CMD_3A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3B(void)
{
    B588_CMD_3B_QUESTION *question = (B588_CMD_3B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3C(void)
{
    B588_CMD_3C_QUESTION *question = (B588_CMD_3C_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3C_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3C: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3D(void)
{
    B588_CMD_3D_QUESTION *question = (B588_CMD_3D_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3D_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3D: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3E(void)
{
    B588_CMD_3E_QUESTION *question = (B588_CMD_3E_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3E_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3E: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_3F(void)
{
    B588_CMD_3F_QUESTION *question = (B588_CMD_3F_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_3F_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_3F: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_41(void)
{
    B588_CMD_41_QUESTION *question = (B588_CMD_41_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_41_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_41: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_42(void)
{
    B588_CMD_42_QUESTION *question = (B588_CMD_42_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_42_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_42: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_43(void)
{
    B588_CMD_43_QUESTION *question = (B588_CMD_43_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_43_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_43: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_44(void)
{
    B588_CMD_44_QUESTION *question = (B588_CMD_44_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_44_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_44: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_45(void)
{
    B588_CMD_45_QUESTION *question = (B588_CMD_45_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_45_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_45: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_46(void)
{
    B588_CMD_46_QUESTION *question = (B588_CMD_46_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_46_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_46: bad crc16" << crc << question->body.crc16;
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_47(void)
{
    B588_CMD_47_QUESTION *question = (B588_CMD_47_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_47_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_47: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_48(void)
{
    B588_CMD_48_QUESTION *question = (B588_CMD_48_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_48_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_48: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_49(void)
{
    B588_CMD_49_QUESTION *question = (B588_CMD_49_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_49_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_49: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_4A(void)
{
    B588_CMD_4A_QUESTION *question = (B588_CMD_4A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_4A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_4A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_4B(void)
{
    B588_CMD_4B_QUESTION *question = (B588_CMD_4B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_4B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_4B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_54(void)
{
    B588_CMD_54_QUESTION *question = (B588_CMD_54_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_54_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_54: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_55(void)
{
    B588_CMD_55_QUESTION *question = (B588_CMD_55_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_55_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_55: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_56(void)
{
    B588_CMD_56_QUESTION *question = (B588_CMD_56_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_56_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_56: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_57(void)
{
    B588_CMD_57_QUESTION *question = (B588_CMD_57_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_57_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_57: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_58(void)
{
    B588_CMD_58_QUESTION *question = (B588_CMD_58_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_58_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_58: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_59(void)
{
    B588_CMD_59_QUESTION *question = (B588_CMD_59_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_59_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_59: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5A(void)
{
    B588_CMD_5A_QUESTION *question = (B588_CMD_5A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5B(void)
{
    B588_CMD_5B_QUESTION *question = (B588_CMD_5B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5C(void)
{
    B588_CMD_5C_QUESTION *question = (B588_CMD_5C_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5C_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5C: bad crc16";
#endif
        return false;
    }

    Value_Zero_ADC_U  = question->body.data.Value_Zero_ADC_U;
    Value_delta_ADC_U = question->body.data.Value_delta_ADC_U;
    Value_DAC_MIN_U   = question->body.data.Value_DAC_MIN;
    Value_DAC_MAX_U   = question->body.data.Value_DAC_MAX;

    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5D(void)
{
    B588_CMD_5D_QUESTION *question = (B588_CMD_5D_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5D_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5D: bad crc16";
#endif
        return false;
    }

    Value_Zero_ADC_I  = question->body.data.Value_Zero_ADC_I;
    Value_delta_ADC_I = question->body.data.Value_delta_ADC_I;
    Value_DAC_MIN_I   = question->body.data.Value_DAC_MIN;
    Value_DAC_MAX_I   = question->body.data.Value_DAC_MAX;

    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5E(void)
{
    B588_CMD_5E_QUESTION *question = (B588_CMD_5E_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5E_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5E: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_5F(void)
{
    B588_CMD_5F_QUESTION *question = (B588_CMD_5F_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_5F_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_5F: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_61(void)
{
    B588_CMD_61_QUESTION *question = (B588_CMD_61_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_61_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_61: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_62(void)
{
    B588_CMD_62_QUESTION *question = (B588_CMD_62_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_62_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_62: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_63(void)
{
    B588_CMD_63_QUESTION *question = (B588_CMD_63_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_63_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_63: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_64(void)
{
    B588_CMD_64_QUESTION *question = (B588_CMD_64_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_64_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_64: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_65(void)
{
    B588_CMD_65_QUESTION *question = (B588_CMD_65_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_65_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_65: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_66(void)
{
    B588_CMD_66_QUESTION *question = (B588_CMD_66_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_66_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_66: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_67(void)
{
    B588_CMD_67_QUESTION *question = (B588_CMD_67_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_67_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_67: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_68(void)
{
    B588_CMD_68_QUESTION *question = (B588_CMD_68_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_68_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_68: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_69(void)
{
    B588_CMD_69_QUESTION *question = (B588_CMD_69_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_69_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_69: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6A(void)
{
    B588_CMD_6A_QUESTION *question = (B588_CMD_6A_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6A_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6A: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6B(void)
{
    B588_CMD_6B_QUESTION *question = (B588_CMD_6B_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6B_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6B: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6C(void)
{
    B588_CMD_6C_QUESTION *question = (B588_CMD_6C_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6C_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6C: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6D(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6D_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6D: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6E(void)
{
    B588_CMD_6E_QUESTION *question = (B588_CMD_6E_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6E_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6E: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_6F(void)
{
    B588_CMD_6F_QUESTION *question = (B588_CMD_6F_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_6F_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_6F: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_71(void)
{
    B588_CMD_71_QUESTION *question = (B588_CMD_71_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_71_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_71: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_72(void)
{
    B588_CMD_72_QUESTION *question = (B588_CMD_72_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_72_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_72: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_73(void)
{
    B588_CMD_73_QUESTION *question = (B588_CMD_73_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_73_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_73: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_B588_test::check_question_75(void)
{
    B588_CMD_75_QUESTION *question = (B588_CMD_75_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(B588_CMD_75_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_75: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_01(void)
{
    if(!check_question_01()) return;

    B588_CMD_01_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x01;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_code = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_01_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_01_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_02(void)
{
    if(!check_question_02()) return;

    B588_CMD_02_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x02;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_U = current_U;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_02_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_02_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_03(void)
{
    if(!check_question_03()) return;

    B588_CMD_03_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x03;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.setting_U = setting_U;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_03_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_03_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_04(void)
{
    if(!check_question_04()) return;

    B588_CMD_04_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x04;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_code = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_04_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_04_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_05(void)
{
    if(!check_question_05()) return;

    B588_CMD_05_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x05;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_I = current_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_05_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_05_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_06(void)
{
    if(!check_question_06()) return;

    B588_CMD_06_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x06;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.setting_I = setting_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_06_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_06_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_07(void)
{
    if(!check_question_07()) return;

    B588_CMD_07_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x07;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.temperature = temperature;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_07_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_07_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_08(void)
{
    if(!check_question_08()) return;

    B588_CMD_08_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x08;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.version = 4;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_08_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_08_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_09(void)
{
    if(!check_question_09()) return;

    B588_CMD_09_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x09;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<11; n++)
    {
        answer.body.data.data_compile[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_09_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_09_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0A(void)
{
    if(!check_question_0A()) return;

    B588_CMD_0A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0A;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<8; n++)
    {
        answer.body.data.time_compile[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0B(void)
{
    if(!check_question_0B()) return;

    B588_CMD_0B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.serno = 1234;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0C(void)
{
    if(!check_question_0C()) return;

    B588_CMD_0C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.month = 1;
    answer.body.data.year  = 2010;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0D(void)
{
    if(!check_question_0D()) return;

    B588_CMD_0D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0D;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<9; n++)
    {
        answer.body.data.profiles[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0E(void)
{
    if(!check_question_0E()) return;

    B588_CMD_0E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.active_point_profil = 0;
    answer.body.data.active_profil = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_0F(void)
{
    if(!check_question_0F()) return;

    B588_CMD_0F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_0F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_0F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    B588_CMD_11_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x11;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_11_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_11_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    B588_CMD_12_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x12;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_12_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_12_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_13(void)
{
    if(!check_question_13()) return;

    B588_CMD_13_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x13;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_13_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_13_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_14(void)
{
    if(!check_question_14()) return;

    B588_CMD_14_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x14;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_14_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_14_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    B588_CMD_15_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x15;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_15_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_15_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    B588_CMD_16_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x16;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_16_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_16_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    B588_CMD_17_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x17;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_17_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_17_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_28(void)
{
    if(!check_question_28()) return;

    B588_CMD_28_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x28;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_28_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_28_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_29(void)
{
    if(!check_question_29()) return;

    B588_CMD_29_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x29;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_29_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_29_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2A(void)
{
    if(!check_question_2A()) return;

    B588_CMD_2A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2A;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2B(void)
{
    if(!check_question_2B()) return;

    B588_CMD_2B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2B;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2C(void)
{
    if(!check_question_2C()) return;

    B588_CMD_2C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2C;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2D(void)
{
    if(!check_question_2D()) return;

    B588_CMD_2D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2D;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2E(void)
{
    if(!check_question_2E()) return;

    B588_CMD_2E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2E;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_2F(void)
{
    if(!check_question_2F()) return;

    B588_CMD_2F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_2F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_2F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_31(void)
{
    if(!check_question_31()) return;

    B588_CMD_31_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x31;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_31_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_31_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_32(void)
{
    if(!check_question_32()) return;

    B588_CMD_32_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x32;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_32_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_32_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_33(void)
{
    if(!check_question_33()) return;

    B588_CMD_33_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x33;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_33_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_33_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_34(void)
{
    if(!check_question_34()) return;

    B588_CMD_34_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x34;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_34_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_34_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_35(void)
{
    if(!check_question_35()) return;

    B588_CMD_35_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x35;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_35_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_35_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_36(void)
{
    if(!check_question_36()) return;

    B588_CMD_36_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x36;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_36_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_36_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_37(void)
{
    if(!check_question_37()) return;

    B588_CMD_37_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x37;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_37_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_37_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_38(void)
{
    if(!check_question_38()) return;

    B588_CMD_38_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x46;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_46_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_46_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_39(void)
{
    if(!check_question_39()) return;

    B588_CMD_39_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x39;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_39_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_39_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3A(void)
{
    if(!check_question_3A()) return;

    B588_CMD_3A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3A;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3B(void)
{
    if(!check_question_3B()) return;

    B588_CMD_3B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3B;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3C(void)
{
    if(!check_question_3C()) return;

    B588_CMD_3C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3C;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3D(void)
{
    if(!check_question_3D()) return;

    B588_CMD_3D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3D;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3E(void)
{
    if(!check_question_3E()) return;

    B588_CMD_3E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3E;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_3F(void)
{
    if(!check_question_3F()) return;

    B588_CMD_3F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_3F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_3F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_41(void)
{
    if(!check_question_41()) return;

    B588_CMD_41_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x41;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_41_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_41_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_42(void)
{
    if(!check_question_42()) return;

    B588_CMD_42_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x42;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_42_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_42_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_43(void)
{
    if(!check_question_43()) return;

    B588_CMD_43_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x43;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_43_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_43_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_44(void)
{
    if(!check_question_44()) return;

    B588_CMD_44_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x44;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_44_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_44_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    B588_CMD_46_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x46;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.type = 3;
    answer.body.data.year = 1;
    answer.body.data.month = 1;
    answer.body.data.serno = 123;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_46_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_46_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    B588_CMD_47_ANSWER answer;

    calculate(setting_U,
              setting_I);
    Connection::set_value(current_U);

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x47;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.in_voltage  = setting_U;
    answer.body.data.in_current  = setting_I;
    answer.body.data.out_voltage = current_U;
    answer.body.data.out_current = current_I;
    answer.body.data.temperature = 25;
    answer.body.data.current_point = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_47_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_47_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    B588_CMD_48_QUESTION *question = (B588_CMD_48_QUESTION *)input.data();
    B588_CMD_48_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x48;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.new_address_modbus = question->body.data.new_address_modbus;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_48_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_48_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_49(void)
{
    if(!check_question_49()) return;

    B588_CMD_49_QUESTION *question = (B588_CMD_49_QUESTION *)input.data();
    B588_CMD_49_ANSWER answer;

    setting_U = question->body.data.voltage;
    setting_I = question->body.data.current;

    calculate(question->body.data.voltage,
              question->body.data.current);
    Connection::set_value(current_U);

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x49;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.voltage = current_U;
    answer.body.data.current = current_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_49_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_49_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_4A(void)
{
    if(!check_question_4A()) return;

    B588_CMD_4A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x4A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_data = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_4A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_4A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_54(void)
{
    if(!check_question_54()) return;

    B588_CMD_54_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x54;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.count_point = 0;
    answer.body.data.count_repeat_profile = 0;
    answer.body.data.error = 0;
    answer.body.data.profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_54_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_54_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_55(void)
{
    if(!check_question_55()) return;

    B588_CMD_55_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x55;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.mototime_min = 1;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_55_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_55_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_56(void)
{
    if(!check_question_56()) return;

    B588_CMD_56_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x56;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current = 0;
    answer.body.data.error = 0;
    answer.body.data.point = 0;
    answer.body.data.profile = 0;
    answer.body.data.time = 0;
    answer.body.data.voltage = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_56_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_56_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_57(void)
{
    if(!check_question_57()) return;

    B588_CMD_57_QUESTION *question = (B588_CMD_57_QUESTION *)input.data();
    B588_CMD_57_ANSWER answer;

    calculate(question->body.data.U,
              question->body.data.I);
    Connection::set_value(current_U);

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x57;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = current_U;
    answer.body.data.I = current_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_57_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_57_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_58(void)
{
    if(!check_question_58()) return;

    B588_CMD_58_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x58;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = current_U;
    answer.body.data.I = current_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_58_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_58_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_59(void)
{
    if(!check_question_59()) return;

    B588_CMD_59_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x59;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = current_U;
    answer.body.data.I = current_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_59_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_59_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5A(void)
{
    if(!check_question_5A()) return;

    B588_CMD_5A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.active_point = 0;
    answer.body.data.active_profile = 0;
    answer.body.data.current_ADC_I = 0;
    answer.body.data.current_ADC_U = 0;
    answer.body.data.setting_DAC_I = 0;
    answer.body.data.setting_DAC_U = 0;
    answer.body.data.temperature = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5B(void)
{
    if(!check_question_5B()) return;

    B588_CMD_5B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.ADC_I = 0;
    answer.body.data.ADC_U = 0;
    answer.body.data.ADC_U_input = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5C(void)
{
    if(!check_question_5C()) return;

    B588_CMD_5C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_Zero_ADC_U  = Value_Zero_ADC_U;
    answer.body.data.Value_delta_ADC_U = Value_delta_ADC_U;
    answer.body.data.Value_DAC_MIN     = Value_DAC_MIN_U;
    answer.body.data.Value_DAC_MAX     = Value_DAC_MAX_U;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5D(void)
{
    if(!check_question_5D()) return;

    B588_CMD_5D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_Zero_ADC_I  = Value_Zero_ADC_I;
    answer.body.data.Value_delta_ADC_I = Value_delta_ADC_I;
    answer.body.data.Value_DAC_MIN     = Value_DAC_MIN_I;
    answer.body.data.Value_DAC_MAX     = Value_DAC_MAX_I;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5E(void)
{
    if(!check_question_5E()) return;

    B588_CMD_5E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current = 0;
    answer.body.data.error = 0;
    answer.body.data.point = 0;
    answer.body.data.profile = 0;
    answer.body.data.time = 0;
    answer.body.data.voltage = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_5F(void)
{
    if(!check_question_5F()) return;

    B588_CMD_5F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.number_profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_5F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_5F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_61(void)
{
    if(!check_question_61()) return;

    B588_CMD_61_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x61;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_Zero_U = Value_Zero_ADC_U;
    answer.body.data.factor_K = factor_K;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_61_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_61_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_62(void)
{
    if(!check_question_62()) return;

    B588_CMD_62_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x62;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_Zero_I  = Value_Zero_ADC_I;
    answer.body.data.factor_K = factor_K;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_62_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_62_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_63(void)
{
    if(!check_question_63()) return;

    B588_CMD_63_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x63;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_point = 0;
    answer.body.data.number_point = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_63_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_63_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
/*
  TASK_CALIBRATION_ADC_U = 1,   // Запуск калибровки АЦП по напряжению
  TASK_CALIBRATION_ADC_I = 2,   // Запуск калибровки АЦП по току
  TASK_CALIBRATION_DAC_U = 3,   // Запуск калибровки ЦАП по напряжению
  TASK_CALIBRATION_DAC_I = 4,   // Запуск калибровки ЦАП по току
  ASK_SLP =  5,                 // Режим SLP
*/
void SerialPort_B588_test::prepare_answer_64(void)
{
    if(!check_question_64()) return;

    B588_CMD_64_QUESTION *question = (B588_CMD_64_QUESTION *)input.data();
    B588_CMD_64_ANSWER answer;

    dac_point = 0;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x64;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.code = question->body.data.code;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_64_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_64_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_65(void)
{
    if(!check_question_65()) return;

    B588_CMD_65_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x65;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.number_current_point = dac_point;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_65_ANSWER) - 2);

    dac_point++;

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_65_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_66(void)
{
    if(!check_question_66()) return;

    B588_CMD_66_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x66;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.points_crc16 = 0;
    answer.body.data.point_value  = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_66_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_66_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_67(void)
{
    if(!check_question_67()) return;

    B588_CMD_67_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x67;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.speed_cooler = 1600;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_67_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_67_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_68(void)
{
    if(!check_question_68()) return;

    B588_CMD_68_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x68;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_PWM = 1600;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_68_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_68_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_69(void)
{
    if(!check_question_69()) return;

    B588_CMD_69_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x69;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.OCR3A = 0;
    answer.body.data.OCR3B = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_69_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_69_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6A(void)
{
    if(!check_question_6A()) return;

    B588_CMD_6A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6A_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6A_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6B(void)
{
    if(!check_question_6B()) return;

    B588_CMD_6B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6B_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6B_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6C(void)
{
    if(!check_question_6C()) return;

    B588_CMD_6C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_compile_day = 0;
    answer.body.data.data_compile_month = 0;
    answer.body.data.data_compile_year = 0;
    answer.body.data.time_compile_hour = 0;
    answer.body.data.time_compile_min = 0;
    answer.body.data.time_compile_sec = 0;
    answer.body.data.version = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6C_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6C_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_GET_ID(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_GET_ID fram_id;
    fram_id.data.ID = 0x55;

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = fram_id.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_RESET(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_CLEAR_MOTOHOUR(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_CLEAR_FRAM(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_READ_FRAM_8BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_8BIT fram_8bit;
    unsigned int len = sizeof(DATA_FRAM_8BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_8bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_8(fram_8bit.data.address, &fram_8bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_8bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_READ_FRAM_16BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_16BIT fram_16bit;
    unsigned int len = sizeof(DATA_FRAM_16BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_16bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_16(fram_16bit.data.address, &fram_16bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_16bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_READ_FRAM_32BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_32BIT fram_32bit;
    unsigned int len = sizeof(DATA_FRAM_32BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_32bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_32(fram_32bit.data.address, &fram_32bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_32bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_READ_FRAM_MORE_BYTES(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_MORE_BYTES fram_more_bytes;
    unsigned int len = sizeof(DATA_FRAM_MORE_BYTES);
    for(unsigned int n=0; n<len; n++)
    {
        fram_more_bytes.buf[n] = question->body.data.secret_data[n];
    }

    uint16_t address = fram_more_bytes.data.address;
    unsigned int len_data = fram_more_bytes.data.data_len;
    uint8_t value = 0;
    for(unsigned int n=0; n<len_data; n++)
    {
        bool ok = fram->read_data_8(address+n, &value);
        if(ok)
        {
            fram_more_bytes.data.fram_data[n] = value;
        }
    }

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_more_bytes.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    //emit trace(QString("output %1").arg(output.toHex().data()));
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_SAVE_FRAM_8BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_8BIT save_fram_8bit;
    unsigned int len = sizeof(DATA_FRAM_8BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_8bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_8(save_fram_8bit.data.address, save_fram_8bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_8bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_SAVE_FRAM_16BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_16BIT save_fram_16bit;
    unsigned int len = sizeof(DATA_FRAM_16BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_16bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_16(save_fram_16bit.data.address, save_fram_16bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_16bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_SAVE_FRAM_32BIT(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_32BIT save_fram_32bit;
    unsigned int len = sizeof(DATA_FRAM_32BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_32bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_32(save_fram_32bit.data.address, save_fram_32bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_32bit.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_SAVE_FRAM_MORE_BYTES(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_MORE_BYTES save_fram_more_bytes;
    unsigned int len = sizeof(DATA_FRAM_MORE_BYTES);
    uint16_t address = save_fram_more_bytes.data.address;
    for(unsigned int n=0; n<len; n++)
    {
        fram->write_data_8(address+n, question->body.data.secret_data[n]);
    }

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_more_bytes.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_SAVE_DATE_AND_SERNO(void)
{
    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::secret_CMD_COPY_USER_TO_FACTORY(void)
{
    B588_CMD_6D_ANSWER answer;

    //---
    fram->copy_user_to_factory_calibration();
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6D_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6D_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6D(void)
{
    if(!check_question_6D()) return;

    B588_CMD_6D_QUESTION *question = (B588_CMD_6D_QUESTION *)input.data();

    //---
    switch(question->body.data.secret_cmd)
    {
    case SECRET_CMD_GET_ID:                 secret_CMD_GET_ID();                break;
    case SECRET_CMD_RESET:                  secret_CMD_RESET();                 break;
    case SECRET_CMD_CLEAR_MOTOHOUR:         secret_CMD_CLEAR_MOTOHOUR();        break;
    case SECRET_CMD_CLEAR_FRAM:             secret_CMD_CLEAR_FRAM();            break;
    case SECRET_CMD_READ_FRAM_8BIT:         secret_CMD_READ_FRAM_8BIT();        break;
    case SECRET_CMD_READ_FRAM_16BIT:        secret_CMD_READ_FRAM_16BIT();       break;
    case SECRET_CMD_READ_FRAM_32BIT:        secret_CMD_READ_FRAM_32BIT();       break;
    case SECRET_CMD_READ_FRAM_MORE_BYTES:   secret_CMD_READ_FRAM_MORE_BYTES();  break;
    case SECRET_CMD_SAVE_FRAM_8BIT:         secret_CMD_SAVE_FRAM_8BIT();        break;
    case SECRET_CMD_SAVE_FRAM_16BIT:        secret_CMD_SAVE_FRAM_16BIT();       break;
    case SECRET_CMD_SAVE_FRAM_32BIT:        secret_CMD_SAVE_FRAM_32BIT();       break;
    case SECRET_CMD_SAVE_FRAM_MORE_BYTES:   secret_CMD_SAVE_FRAM_MORE_BYTES();  break;
    case SECRET_CMD_SAVE_DATE_AND_SERNO:    secret_CMD_SAVE_DATE_AND_SERNO();   break;

    default:    break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6E(void)
{
    if(!check_question_6E()) return;

    B588_CMD_6E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.profile = 0;
    answer.body.data.repeat = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6E_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6E_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_6F(void)
{
    if(!check_question_6F()) return;

    B588_CMD_6F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_6F_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_6F_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_71(void)
{
    if(!check_question_71()) return;

    B588_CMD_71_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x71;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.state_ADC_I = 0;
    answer.body.data.state_ADC_U = 0;
    answer.body.data.state_ADC_Ui = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_71_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_71_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_72(void)
{
    if(!check_question_72()) return;

    B588_CMD_72_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x72;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.conf_ADC = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_72_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_72_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_73(void)
{
    if(!check_question_73()) return;

    B588_CMD_73_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x73;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.real_DAC_I = 0;
    answer.body.data.real_DAC_U = 0;
    answer.body.data.revers = 0;
    answer.body.data.temperature = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_73_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_73_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::prepare_answer_75(void)
{
    if(!check_question_75()) return;

    B588_CMD_75_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x75;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.count_Error_ADC = 0;
    answer.body.data.count_Error_Answer_ADC = 0;
    answer.body.data.count_ReStart_ADC_0 = 0;
    answer.body.data.count_ReStart_ADC_1 = 0;
    answer.body.data.count_ReStart_ADC_2 = 0;
    answer.body.data.MCUCSR = 0;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B588_CMD_75_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(B588_CMD_75_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_B588_test::calculate(uint16_t new_setting_U,
                                     uint16_t new_setting_I)
{
    // все значения в mV и mA
    setting_U = new_setting_U;
    setting_I = new_setting_I;

    double R = sb_R->value();
    double U = new_setting_U;
    double I = (double)new_setting_U / R;
    if(I > new_setting_I)
    {
        U = new_setting_I * R;
        I = new_setting_I;
    }

    current_U = U;
    current_I = I;
}
//--------------------------------------------------------------------------------
