/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QIODevice>
#include <QDateTime>
#include <QFile>
//--------------------------------------------------------------------------------
#include "control_log.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Control_LOG::Control_LOG(QString f_name,
                         QObject *parent) :
    QObject(parent)
{
    filename = f_name;
}
//--------------------------------------------------------------------------------
void Control_LOG::info(const QString text)
{
    QString temp;
    temp.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    temp.append(" INFO ");
    temp.append(text);
    temp.append("\n");

    save(temp);
}
//--------------------------------------------------------------------------------
void Control_LOG::debug(const QString text)
{
    QString temp;
    temp.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    temp.append(" DEBUG ");
    temp.append(text);
    temp.append("\n");

    save(temp);
}
//--------------------------------------------------------------------------------
void Control_LOG::error(const QString text)
{
    QString temp;
    temp.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    temp.append(" ERROR ");
    temp.append(text);
    temp.append("\n");

    save(temp);
}
//--------------------------------------------------------------------------------
void Control_LOG::trace(const QString text)
{
    QString temp;
    temp.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    temp.append(" TRACE ");
    temp.append(text);
    temp.append("\n");

    save(temp);
}
//--------------------------------------------------------------------------------
void Control_LOG::save(QString text)
{
    QFile file(filename);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        file.write(text.toLocal8Bit());
        file.close();
    }
}
//--------------------------------------------------------------------------------
