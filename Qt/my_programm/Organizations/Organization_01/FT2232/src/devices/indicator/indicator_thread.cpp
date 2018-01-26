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
//--------------------------------------------------------------------------------
#include "indicator_thread.hpp"
#include "ft2232h.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Indicator_thread::Indicator_thread(I2C_Freq freq, QWidget *parent) :
    QThread(parent),
    parent(parent),
    is_worked(false)
{
    connect_log();

    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
Indicator_thread::~Indicator_thread()
{
    ft2232h->close();
    ft2232h->deleteLater();
}
//--------------------------------------------------------------------------------
void Indicator_thread::connect_log(void)
{
    if(parent)
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parent, SIGNAL(trace(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void Indicator_thread::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#endif
}
//--------------------------------------------------------------------------------
bool Indicator_thread::read(void)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumBytesRead = 0;
    DWORD dwNumInputBuffer = 0;
    BYTE buf[1024];

    ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("getQueueStatus", ftStatus);
        return false;
    }

    if(dwNumInputBuffer <= 0)
    {
        emit error("no data!");
        return false;
    }

    ftStatus = ft2232h->read(&buf[0], 1024, &dwNumBytesRead);
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("read", ftStatus);
        return false;
    }

    emit debug(QString("read %1 bytes").arg(dwNumBytesRead));
    return true;
}
//--------------------------------------------------------------------------------
void Indicator_thread::run(void)
{
    FT_STATUS ftStatus = FT_OK;

    ftStatus = ft2232h->open(1);
    if(ftStatus != FT_OK)
        return;

    is_worked = true;
    while(is_worked)
    {
        emit info("read");
        if(!read())
        {
            Sleeper::sleep(1);
        }
    }
}
//--------------------------------------------------------------------------------
void Indicator_thread::stop(void)
{
    is_worked = false;
}
//--------------------------------------------------------------------------------
