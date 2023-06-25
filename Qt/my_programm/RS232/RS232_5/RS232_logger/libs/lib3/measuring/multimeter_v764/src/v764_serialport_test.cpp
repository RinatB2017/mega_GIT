/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "v764_serialport_test.hpp"
#include "v764_multimeter.hpp"
#include "v764_packets.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
SerialPort_V764_test::SerialPort_V764_test(QObject *parent) :
    QObject(parent)
{
    ba_input.clear();
    ba_output.clear();

    is_open = false;
    measuring_value = 0;

    code_button = BUTTON_NUMBER_UDC;
    mode_measuring = MODE_UDC;

    //QTimer::singleShot(1000, this, SLOT(create_fake_toolbar()));
}
//--------------------------------------------------------------------------------
SerialPort_V764_test::~SerialPort_V764_test()
{
#ifdef QT_DEBUG
    qDebug() << "~SerialPort_V764_test()";
#endif
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::create_fake_toolbar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(QApplication::activeWindow());
    Q_ASSERT(mw);

    sb_value = new QSpinBox();
    sb_value->setMinimum(-INT_MAX);
    sb_value->setMaximum(INT_MAX);

    cb_hand_value = new QCheckBox();
    cb_hand_value->setText("hand");
    connect(cb_hand_value,  &QCheckBox::toggled,
            this,           &SerialPort_V764_test::hand_data);
    connect(sb_value,       &QSpinBox::editingFinished,
            this,           &SerialPort_V764_test::regenerate_value);

    QToolBar *fake_toolbar = new QToolBar(tr("fake_toolbar"));
    fake_toolbar->setObjectName("fake_toolbar");
    fake_toolbar->addWidget(new QLabel(tr("Value")));
    fake_toolbar->addWidget(sb_value);
    fake_toolbar->addWidget(cb_hand_value);

    hand_data(false);
    //mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, fake_toolbar);
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::regenerate_value(void)
{
    measuring_value = sb_value->value();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::hand_data(bool state)
{
    sb_value->setEnabled(state);
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_V764_test::readAll(void)
{
    return ba_output;
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::input(const QByteArray &data)
{
    write(data);
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::get_test_value(void)
{
    //TODO подготавливаем точку синусоиды
    measuring_value = 5.0 * qSin(qreal(index_measuring_value)*qreal(M_PI)/qreal(180.0));
    index_measuring_value++;

    if(measuring_value < 0)
    {
        emit debug("measuring_value < 0");
    }

    ba_output.clear();

    //ba_output.append("+0.130469 V");
    QString temp = QString("%1 V").arg(measuring_value, 0, 'f', 6);
    emit debug(temp);

    ba_output.append(temp);

    ba_output.append((char)0x0D);
    ba_output.append((char)0x0A);

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::serial_open(void)
{
    return open(1);
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::serial_close(void)
{
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::setPortName(QString)
{

}
//--------------------------------------------------------------------------------
QStringList SerialPort_V764_test::get_port_names(void)
{
    QStringList sl;
    sl << "TEST";

    return sl;
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::write(const QByteArray &data)
{
    if(data.isEmpty()) return;

    ba_input.clear();
    ba_input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case V764_CMD_0x45: prepare_answer_45(); break;
    case V764_CMD_0x46: prepare_answer_46(); break;
    case V764_CMD_0x47: prepare_answer_47(); break;
    case V764_CMD_0x48: prepare_answer_48(); break;
    case V764_CMD_0x53: prepare_answer_53(); break;
    default:    break;
    }
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::write(const char *data, int len)
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
void SerialPort_V764_test::print_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86/2: ");
    temp.append(text);

    emit info(temp);
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::print_err_log(const QString &text)
{
    QString temp;
    temp.append("### Б7-86/2: ");
    temp.append(text);

    emit error(temp);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::check_question_45(void)
{
    CMD_V764_0x45_QUESTION *question = (CMD_V764_0x45_QUESTION *)ba_input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)question,
                                                sizeof(*question) - 2);
    if(crc != question->crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_45: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::check_question_46(void)
{
    CMD_V764_0x46_QUESTION *question = (CMD_V764_0x46_QUESTION *)ba_input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)question,
                                                sizeof(*question) - 2);
    if(crc != question->crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_46: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::check_question_47(void)
{
    CMD_V764_0x47_QUESTION *question = (CMD_V764_0x47_QUESTION *)ba_input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)question,
                                                sizeof(*question) - 2);
    if(crc != question->crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_47: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::check_question_48(void)
{
    CMD_V764_0x48_QUESTION *question = (CMD_V764_0x48_QUESTION *)ba_input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)question,
                                                sizeof(*question) - 2);
    if(crc != question->crc16)
    {
#ifdef QT_DEBUG
        qDebug() << "check_question_48: bad crc16";
#endif
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_V764_test::check_question_53(void)
{
    CMD_V764_0x53_QUESTION *question = (CMD_V764_0x53_QUESTION *)ba_input.data();

    unsigned short crc = CRC::powersupply_crc16((unsigned char *)question,
                                                sizeof(*question) - 2);
    if(crc != question->crc16)
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
void SerialPort_V764_test::prepare_answer_45(void)
{
    if(!check_question_45()) return;

    //---
    CMD_V764_0x45_QUESTION *question = reinterpret_cast<CMD_V764_0x45_QUESTION *>(ba_input.data());
    Q_ASSERT(question);

    print_log("Пришел пакет");
    print_log(QString("   address: %1").arg(question->header.address));
    print_log(QString("   cmd: %1").arg(question->header.cmd));
    print_log(QString("   cnt: %1").arg(question->header.count_data));
    print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
    //---

    CMD_V764_0x45_ANSWER answer;

    answer.header.address = 0;
    answer.header.cmd = V764_CMD_0x45;
    answer.header.count_data = 0;
    answer.crc16 = CRC::powersupply_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(CMD_V764_0x45_ANSWER));

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    //---
    CMD_V764_0x46_QUESTION *question = reinterpret_cast<CMD_V764_0x46_QUESTION *>(ba_input.data());
    Q_ASSERT(question);

    print_log("Пришел пакет");
    print_log(QString("   address: %1").arg(question->header.address));
    print_log(QString("   cmd: %1").arg(question->header.cmd));
    print_log(QString("   cnt: %1").arg(question->header.count_data));
    print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
    //---

    CMD_V764_0x46_ANSWER answer;

    answer.header.address = 0;
    answer.header.cmd = V764_CMD_0x46;
    answer.header.count_data = sizeof(answer) - sizeof(answer.header) - sizeof(answer.crc16);
    answer.type = 0;
    answer.year = 0;
    answer.month = 1;
    answer.serno_lo = 0x12;
    answer.serno_hi = 0x34;
    answer.baud = 1;
    answer.time_waiting = 1;
    answer.address_station = 1;
    answer.byte_test = 0;
    answer.crc16 = CRC::powersupply_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    //---
    CMD_V764_0x47_QUESTION *question = reinterpret_cast<CMD_V764_0x47_QUESTION *>(ba_input.data());
    Q_ASSERT(question);

    print_log("Пришел пакет");
    print_log(QString("   address: %1").arg(question->header.address));
    print_log(QString("   cmd: %1").arg(question->header.cmd));
    print_log(QString("   cnt: %1").arg(question->header.count_data));
    print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
    //---

    CMD_V764_0x47_ANSWER answer;

    if(!cb_hand_value->isChecked())
    {
        measuring_value = Connection::get_uint16_value();
    }

    answer.header.address = 0;
    answer.header.cmd = V764_CMD_0x47;
    answer.header.count_data = sizeof(answer) - sizeof(answer.header) - sizeof(answer.crc16);
    answer.state = mode_measuring;
    answer.error = 0;

    answer.value = measuring_value;

    answer.crc16 = CRC::powersupply_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(CMD_V764_0x47_ANSWER));

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    //---
    CMD_V764_0x48_QUESTION *question = reinterpret_cast<CMD_V764_0x48_QUESTION *>(ba_input.data());
    Q_ASSERT(question);

    print_log("Пришел пакет");
    print_log(QString("   address: %1").arg(question->header.address));
    print_log(QString("   cmd: %1").arg(question->header.cmd));
    print_log(QString("   cnt: %1").arg(question->header.count_data));
    print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
    //---

    CMD_V764_0x48_ANSWER answer;

    answer.header.address = 0;
    answer.header.cmd = V764_CMD_0x48;
    answer.header.count_data = sizeof(answer) - sizeof(answer.header) - sizeof(answer.crc16);
    answer.crc16 = CRC::powersupply_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::prepare_answer_53(void)
{
    if(!check_question_53()) return;

    CMD_V764_0x53_QUESTION *question = (CMD_V764_0x53_QUESTION *)ba_input.data();

    //---
    Q_ASSERT(question);

    print_log("Пришел пакет");
    print_log(QString("   address: %1").arg(question->header.address));
    print_log(QString("   cmd: %1").arg(question->header.cmd));
    print_log(QString("   cnt: %1").arg(question->header.count_data));
    print_log(QString("   crc16: %1").arg(question->crc16, 2, 16, QChar('0')));
    //---

    code_button = question->data_0;

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

    CMD_V764_0x53_ANSWER answer;

    answer.header.address = 0;
    answer.header.cmd = V764_CMD_0x53;
    answer.header.count_data = sizeof(answer) - sizeof(answer.header) - sizeof(answer.crc16);
    answer.data_0 = code_button;
    answer.crc16 = CRC::powersupply_crc16((unsigned char *)&answer, sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append((const char *)&answer, sizeof(answer));

    emit output(ba_output);
    emit readyRead();
}
//--------------------------------------------------------------------------------
void SerialPort_V764_test::receive(const QVariant &data)
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
