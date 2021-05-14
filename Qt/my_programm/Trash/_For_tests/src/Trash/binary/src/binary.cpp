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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "binary.hpp"
//--------------------------------------------------------------------------------
Binary::Binary(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
Binary::~Binary()
{

}
//--------------------------------------------------------------------------------
void Binary::file_to_buf(const QString &i_filename,
                         const QString &o_filename)
{
    if(i_filename.isEmpty())
    {
        return;
    }
    if(o_filename.isEmpty())
    {
        return;
    }

    QFile i_file(i_filename);
    if (!i_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QFile o_file(o_filename);
    if (!o_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    while (!i_file.atEnd()) {
        QByteArray line = i_file.readLine();
        //process_line(i_line);
    }

    //static const unsigned char qt_resource_data[] = {
}
//--------------------------------------------------------------------------------
void Binary::buf_to_file(const QString &i_filename,
                         const QString &o_filename)
{
    if(i_filename.isEmpty())
    {
        return;
    }
    if(o_filename.isEmpty())
    {
        return;
    }
}
//--------------------------------------------------------------------------------
