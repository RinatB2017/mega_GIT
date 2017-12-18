/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QByteArray>
#include <QVariant>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "fake_oscilloscope_GDS_840C.hpp"
//--------------------------------------------------------------------------------
Fake_oscilloscope_GDS_840C::Fake_oscilloscope_GDS_840C(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::init(void)
{

}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::isOpen(void)
{
#ifdef QT_DEBUG
    qDebug() << "isOpen" << is_open;
#endif
    return is_open;
}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray Fake_oscilloscope_GDS_840C::readAll(void)
{
    return output;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::write(QByteArray data)
{
    if(data.isEmpty()) return;

#ifdef QT_DEBUG
    qDebug() << "data" << data;
#endif

    input.clear();
    input.append(data);

#if 0
    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case V764_2_CMD_0x45: prepare_answer_45(); break;
    case V764_2_CMD_0x46: prepare_answer_46(); break;
    case V764_2_CMD_0x47: prepare_answer_47(); break;
    case V764_2_CMD_0x48: prepare_answer_48(); break;
    case V764_2_CMD_0x53: prepare_answer_53(); break;
    default:    break;
    }
#endif
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::write(const char *data, int len)
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
void Fake_oscilloscope_GDS_840C::receive(const QVariant &data)
{
    Q_UNUSED(data); //???
}
//--------------------------------------------------------------------------------
