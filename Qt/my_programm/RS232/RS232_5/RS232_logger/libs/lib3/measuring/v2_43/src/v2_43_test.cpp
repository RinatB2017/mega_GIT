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
#include <QSerialPort>
#include <QVariant>
#include <QtMath>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "v2_43_test.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
V243_test::V243_test(QObject *parent) :
    QObject(parent)
{
    output.clear();

    is_open = false;
}
//--------------------------------------------------------------------------------
bool V243_test::isOpen(void)
{
    return is_open;
}
//--------------------------------------------------------------------------------
bool V243_test::open(int mode)
{
#ifdef QT_DEBUG
    qDebug() << "open" << mode;
#endif
    Q_UNUSED(mode)
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void V243_test::close(void)
{
#ifdef QT_DEBUG
    qDebug() << "close";
#endif
    is_open = false;
}
//--------------------------------------------------------------------------------
void V243_test::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool V243_test::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray V243_test::readAll(void)
{
    return output;
}
//--------------------------------------------------------------------------------
void V243_test::write(QByteArray data)
{
    if(data.isEmpty()) return;

}
//--------------------------------------------------------------------------------
void V243_test::write(const char *data, int len)
{
    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
void V243_test::flush(void)
{

}
//--------------------------------------------------------------------------------
bool V243_test::setFlowControl(QSerialPort::FlowControl flow)
{
#ifdef QT_DEBUG
    qDebug() << "setFlowControl" << flow;
#else
    Q_UNUSED(flow);
#endif
    return true;
}
//--------------------------------------------------------------------------------
