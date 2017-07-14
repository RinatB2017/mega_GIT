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
#include <QApplication>
#include <QPushButton>
#include <QCheckBox>
#include <QToolBar>
#include <QSpinBox>
#include <QVariant>
#include <QLabel>
#include <QTimer>
#include <limits.h>
//--------------------------------------------------------------------------------
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v780_serialport_test.hpp"
#include "v780_multimeter.hpp"
#include "v780_packets.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_V780_test::SerialPort_V780_test(QObject *parent) :
    QObject(parent)
{
    input.clear();
    output.clear();

    is_open = false;
    measuring_value = 0;

    code_button = BUTTON_NUMBER_UDC;
    mode_measuring = MODE_UDC;

    QTimer::singleShot(1000, this, SLOT(create_fake_toolbar()));
}
//--------------------------------------------------------------------------------
SerialPort_V780_test::~SerialPort_V780_test()
{
#ifdef QT_DEBUG
    qDebug() << "~SerialPort_V780_test()";
#endif
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::create_fake_toolbar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(QApplication::activeWindow());
    if(!mw) return;

    sb_value = new QSpinBox();
    sb_value->setMinimum(-INT_MAX);
    sb_value->setMaximum(INT_MAX);

    cb_hand_value = new QCheckBox();
    cb_hand_value->setText("hand");
    connect(cb_hand_value, SIGNAL(toggled(bool)), this, SLOT(hand_data(bool)));
    connect(sb_value, SIGNAL(editingFinished()), this, SLOT(regenerate_value()));

    QToolBar *toolBar = new QToolBar(tr("fake_toolbar"));
    toolBar->setObjectName("fake_toolbar");
    toolBar->addWidget(new QLabel(tr("Value")));
    toolBar->addWidget(sb_value);
    toolBar->addWidget(cb_hand_value);

    hand_data(false);
    //mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, toolBar);
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::regenerate_value(void)
{
    measuring_value = sb_value->value();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::hand_data(bool state)
{
    sb_value->setEnabled(state);
}
//--------------------------------------------------------------------------------
bool SerialPort_V780_test::isOpen(void)
{
#ifdef QT_DEBUG
    qDebug() << "isOpen" << is_open;
#endif
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_V780_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_V780_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V780_test::readAll(void)
{
    return output;
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::write(QByteArray data)
{
    if(data.isEmpty()) return;

    input.clear();
    input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case V780_CMD_0x45: prepare_answer_45(); break;
    case V780_CMD_0x46: prepare_answer_46(); break;
    case V780_CMD_0x47: prepare_answer_47(); break;
    case V780_CMD_0x48: prepare_answer_48(); break;
    case V780_CMD_0x53: prepare_answer_53(); break;
    default:    break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::write(const char *data, int len)
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
bool SerialPort_V780_test::check_question_45(void)
{
    CMD_V780_0x45_QUESTION *question = (CMD_V780_0x45_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(CMD_V780_0x45_QUESTION) - 2);
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
bool SerialPort_V780_test::check_question_46(void)
{
    CMD_V780_0x46_QUESTION *question = (CMD_V780_0x46_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(CMD_V780_0x46_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_46: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V780_test::check_question_47(void)
{
    CMD_V780_0x47_QUESTION *question = (CMD_V780_0x47_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(CMD_V780_0x47_QUESTION) - 2);
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
bool SerialPort_V780_test::check_question_48(void)
{
    CMD_V780_0x48_QUESTION *question = (CMD_V780_0x48_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(CMD_V780_0x48_QUESTION) - 2);
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
bool SerialPort_V780_test::check_question_53(void)
{
    CMD_V780_0x53_QUESTION *question = (CMD_V780_0x53_QUESTION *)input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)&question->buf, sizeof(CMD_V780_0x53_QUESTION) - 2);
    if(crc != question->body.crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_53: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void SerialPort_V780_test::prepare_answer_45(void)
{
    if(!check_question_45()) return;

    CMD_V780_0x45_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = V780_CMD_0x45;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_V780_0x45_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(CMD_V780_0x45_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    CMD_V780_0x46_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = V780_CMD_0x46;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.data.type = 0;
    answer.body.data.year = 0;
    answer.body.data.month = 1;
    answer.body.data.serno_lo = 0x12;
    answer.body.data.serno_hi = 0x34;
    answer.body.data.baud = 1;
    answer.body.data.time_waiting = 1;
    answer.body.data.address_station = 1;
    answer.body.data.byte_test = 0;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_V780_0x46_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(CMD_V780_0x46_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    CMD_V780_0x47_ANSWER answer;

    if(!cb_hand_value->isChecked())
    {
        measuring_value = Connection::get_uint16_value();
    }

    answer.body.header.address = 0;
    answer.body.header.cmd = V780_CMD_0x47;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.data.state = mode_measuring;
    answer.body.data.error = 0;

    //qDebug() << QString("SerialPort_V780_test::prepare_answer_47 measuring_value %1").arg(measuring_value);
    answer.body.data.value = measuring_value * 1000;

    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_V780_0x47_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(CMD_V780_0x47_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    CMD_V780_0x48_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = V780_CMD_0x48;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_V780_0x48_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(CMD_V780_0x48_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::prepare_answer_53(void)
{
    if(!check_question_53()) return;

    CMD_V780_0x53_QUESTION *question = (CMD_V780_0x53_QUESTION *)input.data();
    code_button = question->body.data.data_0;

    switch (code_button)
    {
    case BUTTON_CODE_UDC: mode_measuring = MODE_UDC;  break;
    case BUTTON_CODE_IDC: mode_measuring = MODE_IDC;  break;
    case BUTTON_CODE_R:   mode_measuring = MODE_R;    break;
    case BUTTON_CODE_UAC: mode_measuring = MODE_UAC;  break;
    case BUTTON_CODE_IAC: mode_measuring = MODE_IAC;  break;
    case BUTTON_CODE_HZ:  mode_measuring = MODE_HZ;   break;
    case BUTTON_CODE_MHZ: mode_measuring = MODE_MHZ;  break;

    default:
        mode_measuring = MODE_UDC;
        break;
    }

    emit measurement_mode(mode_measuring);

    CMD_V780_0x53_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = V780_CMD_0x53;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.data.data_0 = code_button;
    answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(CMD_V780_0x53_ANSWER) - 2);

    output.clear();
    output.append((const char *)answer.buf, sizeof(CMD_V780_0x53_ANSWER));

    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V780_test::receive(const QVariant &data)
{
    bool ok = false;
    float temp = data.toFloat(&ok);
    if(ok)
    {
        measuring_value = temp * 1000.0f;
#ifdef QT_DEBUG
        qDebug() << "measuring_value" << measuring_value;
#endif
    }
}
//--------------------------------------------------------------------------------
