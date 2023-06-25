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
#include <QSerialPort>
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#include "H4_X_serialport_test.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SerialPort_H4_X_test::SerialPort_H4_X_test(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
SerialPort_H4_X_test::~SerialPort_H4_X_test()
{
    if(test_tb)
        mw->removeToolBar(test_tb);

    if(tp)
        delete tp;

    if(converter)
        delete converter;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::init(void)
{
    add_toolbar();

    ba_input.clear();
    ba_output.clear();

    is_open = false;

    converter = new Converter_UIR();
    connect(converter,   &Converter_UIR::info,   this,   &SerialPort_H4_X_test::s_info);
    connect(converter,   &Converter_UIR::debug,  this,   &SerialPort_H4_X_test::s_debug);
    connect(converter,   &Converter_UIR::error,  this,   &SerialPort_H4_X_test::s_error);
    connect(converter,   &Converter_UIR::trace,  this,   &SerialPort_H4_X_test::s_trace);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::add_toolbar(void)
{
    QWidgetList widgets = qApp->allWidgets();
    foreach (QWidget *widget, widgets)
    {
        mw = dynamic_cast<MainWindow *>(widget);
        if(mw)
        {
#if 0
            test_tb = new QToolBar();
            test_tb->setObjectName("test_toolbar");

            QPushButton *btn = new QPushButton("set param");
            connect(btn,    &QPushButton::clicked,
                    this,   &SerialPort_V786_2_test::set_param);

            test_tb->addWidget(btn);

            mw->addToolBar(test_tb);
#else
            tp = new SerialPort_H4_X_test_param();
            connect(this,   &SerialPort_H4_X_test::s_info,
                    tp,     &SerialPort_H4_X_test_param::f_info);
            connect(this,   &SerialPort_H4_X_test::s_debug,
                    tp,     &SerialPort_H4_X_test_param::f_debug);
            connect(this,   &SerialPort_H4_X_test::s_error,
                    tp,     &SerialPort_H4_X_test_param::f_error);
            connect(this,   &SerialPort_H4_X_test::s_trace,
                    tp,     &SerialPort_H4_X_test_param::f_trace);
            mw->add_dock_widget("test_param", "test_param", Qt::RightDockWidgetArea, tp);
#endif
            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::open(int mode)
{
    emit s_debug(QString("### H4-6: open: %1").arg(mode));
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::close(void)
{
    emit s_debug("### H4-6: сlose");
    is_open = false;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::setPort(QSerialPortInfo sinfo)
{
    emit s_debug(QString("### H4-6: setPort: %1").arg(sinfo.portName()));
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setBaudRate(int speed)
{
    emit s_debug(QString("### H4-6: setBaudRate: %1").arg(speed));
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setDataBits(QSerialPort::DataBits dataBits)
{
    emit s_debug(QString("### H4-6: setDataBits: %1").arg(dataBits));
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setParity(QSerialPort::Parity parity)
{
    emit s_debug(QString("### H4-6: setParity: %1").arg(parity));
    return true;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setStopBits(QSerialPort::StopBits stopBits)
{
    emit s_debug(QString("### H4-6: setStopBits: %1").arg(stopBits));
    return true;
}
//--------------------------------------------------------------------------------
QByteArray SerialPort_H4_X_test::readAll(void)
{
    return ba_output;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::input(const QByteArray &data)
{
    QByteArray temp = ba_output;
    emit s_debug(QString("### H4-6: отправлены данные: [%1]")
                 .arg(temp.replace("\r\n", "").data()));
    write(data);
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::check_manual(const QByteArray &ba)
{
    // массив приходит чистый, без \r\n

    QString array_U;
    QString array_I;
    QString array_R;
    QString array_F;

    float k = 1.0f;

    enum {
        STATUS_IDLE = 0,
        STATUS_U,
        STATUS_I,
        STATUS_R,
        STATUS_F
    };
    int status = STATUS_IDLE;

    if(ba.isEmpty() || (ba.length() < 2))
    {
        emit s_error(QString("Size ba: %1").arg(ba.length()));
        return false;
    }
    if(ba.length() % 2)
    {
        emit s_error("ba.length() % 2");
        emit s_error(ba.data());
        return false;
    }

    int len = ba.length();
    for(int pos=0; pos<len; pos+=2)
    {
        QByteArray s_ba = ba.mid(pos, 2);
        switch (status)
        {
        case STATUS_IDLE:
            if(s_ba == "X1") status = STATUS_U;
            if(s_ba == "X2") status = STATUS_I;
            if(s_ba == "X0") status = STATUS_R;
            if(s_ba == "X3") status = STATUS_F;
            if(s_ba == "X4") status = STATUS_F;
            break;

        case STATUS_U:
            if(s_ba == "Y7") k *= 10.0f;
            if(s_ba == "Y6") k *= 0.1f;
            if(s_ba == "X0") array_U.append("0");
            if(s_ba == "X1") array_U.append("1");
            if(s_ba == "X2") array_U.append("2");
            if(s_ba == "X3") array_U.append("3");
            if(s_ba == "X4") array_U.append("4");
            if(s_ba == "X5") array_U.append("5");
            if(s_ba == "X6") array_U.append("6");
            if(s_ba == "X7") array_U.append("7");
            if(s_ba == "X8") array_U.append("8");
            if(s_ba == "X9") array_U.append("9");
            break;

        case STATUS_I:
            if(s_ba == "Y7") k *= 10.0f;
            if(s_ba == "Y6") k *= 0.1f;
            if(s_ba == "X0") array_I.append("0");
            if(s_ba == "X1") array_I.append("1");
            if(s_ba == "X2") array_I.append("2");
            if(s_ba == "X3") array_I.append("3");
            if(s_ba == "X4") array_I.append("4");
            if(s_ba == "X5") array_I.append("5");
            if(s_ba == "X6") array_I.append("6");
            if(s_ba == "X7") array_I.append("7");
            if(s_ba == "X8") array_I.append("8");
            if(s_ba == "X9") array_I.append("9");
            break;

        case STATUS_R:
            if(s_ba == "Y7") k *= 10.0f;
            if(s_ba == "Y6") k *= 0.1f;
            if(s_ba == "X0") array_R.append("0");
            if(s_ba == "X1") array_R.append("1");
            if(s_ba == "X2") array_R.append("2");
            if(s_ba == "X3") array_R.append("3");
            if(s_ba == "X4") array_R.append("4");
            if(s_ba == "X5") array_R.append("5");
            if(s_ba == "X6") array_R.append("6");
            if(s_ba == "X7") array_R.append("7");
            if(s_ba == "X8") array_R.append("8");
            if(s_ba == "X9") array_R.append("9");
            break;

        case STATUS_F:
            if(s_ba == "Y7") k *= 10.0f;
            if(s_ba == "Y6") k *= 0.1f;
            if(s_ba == "X0") array_R.append("0");
            if(s_ba == "X1") array_R.append("1");
            if(s_ba == "X2") array_R.append("2");
            if(s_ba == "X3") array_R.append("3");
            if(s_ba == "X4") array_R.append("4");
            if(s_ba == "X5") array_R.append("5");
            if(s_ba == "X6") array_R.append("6");
            if(s_ba == "X7") array_R.append("7");
            if(s_ba == "X8") array_R.append("8");
            if(s_ba == "X9") array_R.append("9");
            break;
        }
    }

    if(!array_U.isEmpty())
    {
        bool ok;
        int val = array_U.toInt(&ok);
        if(ok)
        {
            Connection::set_value((float)val * k);
            emit s_error(QString(">>>>> [0] %1").arg((float)val * k));
            emit output_qvariant((float)val * k);
            return true;
        }
        else
        {
            emit s_error("Bad U");
        }
    }
    if(!array_I.isEmpty())
    {
        bool ok;
        int val = array_I.toInt(&ok);
        if(ok)
        {
            Connection::set_value((float)val * k);
            emit s_error(QString(">>>>> [1] %1").arg((float)val * k));
            emit output_qvariant((float)val * k);
            return true;
        }
        else
        {
            emit s_error("Bad I");
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::check_auto(const QByteArray &ba)
{
    // массив приходит чистый, без \r\n
    if(ba.isEmpty())    return false;

    if(ba.contains("T0"))
    {
        ba_output.clear();
        ba_output.append("00\r\n");

        QByteArray temp = ba_output;
        emit s_debug(QString("### H4-6: отправлено [%1]")
                     .arg(temp.replace("\r\n", "").data()));
        delay();
        emit output(ba_output);
        emit readyRead();
        return true;
    }

    if(ba.contains("UD"))
    {
        bool ok = false;
        QString temp = ba.data();
        temp.remove("UD");
        float x = temp.toFloat(&ok);

        if(ok)
        {
            delay();
            emit s_debug(QString("### H4-6: UD: set value %1").arg(x));
            Connection::set_value(x);
            emit s_error(QString(">>>>> [2] %1").arg(x));
            emit output_qvariant(x);
            return true;
        }
        else
        {
            emit s_debug("### H4-6: bad UD");
            return false;
        }
    }

    if(ba.contains("UA"))
    {
        bool ok = false;
        QString temp = ba.data();
        temp.remove("UA");
        int pos = temp.indexOf('F');
        if(pos) temp.resize(pos);
        float x = temp.toFloat(&ok);

        if(ok)
        {
            delay();
            emit s_debug(QString("### H4-6: UA: set value %1").arg(x));
            Connection::set_value(x);
            emit s_error(QString(">>>>> [3] %1").arg(x));
            emit output_qvariant(x);
            return true;
        }
        else
        {
            emit s_debug("### H4-6: bad UA");
            return false;
        }
    }

    if(ba.contains("ID"))
    {
        bool ok = false;
        QString temp = ba.data();
        temp.remove("ID");
        float x = temp.toFloat(&ok);

        if(ok)
        {
            delay();
            emit s_debug(QString("### H4-6: ID: set value %1").arg(x));
            Connection::set_value(x);  //TODO connection
            emit s_error(QString(">>>>> [4] %1").arg(x));
            emit output_qvariant(x);
            return true;
        }
        else
        {
            emit s_debug("### H4-6: bad ID");
            return false;
        }
    }

    if(ba.contains("IA"))
    {
        bool ok = false;
        QString temp = ba.data();
        temp.remove("IA");
        int pos = temp.indexOf('F');
        if(pos) temp.resize(pos);
        float x = temp.toFloat(&ok);

        if(ok)
        {
            delay();
            emit s_debug(QString("### H4-6: IA: set value %1").arg(x));
            Connection::set_value(x);  //TODO connection
            emit s_error(QString(">>>>> [5] %1").arg(x));
            emit output_qvariant(x);
            return true;
        }
        else
        {
            emit s_debug("### H4-6: bad IA");
            return false;
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::check_command(const QByteArray &ba)
{
    QString filename = QDir::currentPath() + "/calibrator_values.txt";

    QString temp_str;
    temp_str.append(ba.data());
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        bool found = false;
        while(!in.atEnd())
        {
            QString temp = in.readLine();
            if(!temp.isEmpty())
            {
                QStringList sl = temp.split("|");
                if(sl.count() == 3)
                {
                    if(temp_str == sl.at(0))
                    {
                        Connection::set_value(sl.at(1).toInt());  //TODO connection
                        emit s_error(QString(">>>>> [6] %1").arg(sl.at(1).toInt()));
                        emit output_qvariant(sl.at(1));
                        emit s_trace(QString("emit %1").arg(sl.at(2)));
                        found = true;
                    }
                }
            }
        }
        if(!found)
        {
            emit s_error(QString("Command %1 not found!")
                         .arg(ba.toHex().toUpper().data()));
        }
    }
    else
    {
        emit s_error(QString("File %1 not open!").arg(filename));
        return;
    }

    if(ba.contains("T0"))
    {
        ba_output.clear();
        ba_output.append("00\r\n");

        QByteArray temp = ba_output;
        emit s_debug(QString("### H4-6: отправлено [%1]")
                     .arg(temp.replace("\r\n", "").data()));
        delay();
        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::write(const QByteArray &data)
{
    if(data.isEmpty()) return;

#ifndef FAST_TEST
    wait(100);
#endif

    QByteArray temp = data;

    ba_input.clear();
    ba_input.append(temp.replace("\r","").replace("\n","").toUpper());
    emit s_debug(QString("### H4-6: %1").arg(ba_input.data()));

    converter->input(ba_input);

    if(ba_input.contains("T0"))
    {
        ba_output.clear();
        ba_output.append("00\r\n"); //TODO send 00

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::delay(void)
{
    if(answer_delay > 0)
    {
        wait(answer_delay);
    }
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::write(const char *data, int len)
{
    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::flush(void)
{

}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::wait(int timeout_msec)
{
#ifdef TEST
    timeout_msec = TEST_WAIT_MS;
#endif

    QElapsedTimer time;

    if(timeout_msec < 1)
        return;

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
bool SerialPort_H4_X_test::setFlowControl(QSerialPort::FlowControl flow)
{
    emit s_debug(QString("### H4-6: setFlowControl: %1").arg(flow));
    return true;
}
//--------------------------------------------------------------------------------
void SerialPort_H4_X_test::set_answer_delay(int delay_ms)
{
    answer_delay = delay_ms;
}
//--------------------------------------------------------------------------------
