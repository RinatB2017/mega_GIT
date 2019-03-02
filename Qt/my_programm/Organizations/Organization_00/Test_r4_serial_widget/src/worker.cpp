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
#include <QCoreApplication>
#include <QTimer>
#include <unistd.h>
//--------------------------------------------------------------------------------
#include "r4_serial.h"
//--------------------------------------------------------------------------------
#include "worker.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Worker::Worker()
{
    descriptor = -1;

    init_device();
    init_timer();
}
//--------------------------------------------------------------------------------
Worker::~Worker()
{

}
//--------------------------------------------------------------------------------
void Worker::init_timer(void)
{
    QTimer *writer = new QTimer(this);
    QTimer *reader = new QTimer(this);

    connect(writer, SIGNAL(timeout()), this, SLOT(write_data()));
    connect(reader, SIGNAL(timeout()), this, SLOT(read_data()));

    reader->start(1000);
    writer->start(100);
}
//--------------------------------------------------------------------------------
void Worker::write_data(void)
{
    QByteArray write_buf("Hello");

    if(descriptor > 0)
    {
        ssize_t s = write(descriptor,
                          static_cast<char *>(write_buf.data()),
                          static_cast<size_t>(write_buf.length()));
        emit debug(QString("write %1 bytes").arg(s));
    }
}
//--------------------------------------------------------------------------------
void Worker::read_data(void)
{
    memset(buf, 0, sizeof(buf));
    len = read(descriptor, buf, sizeof(buf));
    if(len > 0)
    {
        emit output(buf);
    }
    else
    {
        serial->close_device();
        descriptor = serial->setSettings(&s, true);
    }
}
//--------------------------------------------------------------------------------
void Worker::input(const QString &data)
{
    if(descriptor > 0)
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        ssize_t s = write(descriptor, data.toAscii(), strlen(data.toAscii()));
#else
        ssize_t s = write(descriptor, data.toLocal8Bit(), strlen(data.toLocal8Bit()));
#endif
        Q_UNUSED(s);
    }
}
//--------------------------------------------------------------------------------
void Worker::info_device(void)
{
    serial->getSettings(&s);
    emit debug(QString("device %1").arg(s.device));
    emit debug(QString("speed %1").arg(s.speed));
    emit debug(QString("data %1").arg(s.data));
    emit debug(QString("stop %1").arg(s.stop));
    emit debug(QString("stream %1").arg(s.stream));
    emit debug(QString("parity %1").arg(s.parity));
}
//--------------------------------------------------------------------------------
void Worker::init_device(void)
{
    emit info("init serial");
    serial   = new R4Serial();
    s.device = "/dev/ttyUSB0";
    s.speed  = 9600;
    s.data   = 8;
    s.stop   = 1;
    s.stream = Stream_Off;
    s.parity = Parity_Off;
    s.read   = Read_On;
}
//--------------------------------------------------------------------------------
