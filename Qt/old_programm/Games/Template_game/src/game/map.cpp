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
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QFile>
//--------------------------------------------------------------------------------
#include "map.hpp"
//--------------------------------------------------------------------------------
Map::Map(void)
{
    init();
}
//--------------------------------------------------------------------------------
void Map::init(void)
{
    int x,y;
    for(y=0; y<MAX_Y; y++)
    {
        for(x=0; x<MAX_X; x++)
        {
            map[x][y] = 0;
        }
    }
    width  = MAX_X;
    height = MAX_Y;
}
//--------------------------------------------------------------------------------
int Map::load(const QString &filename)
{
    QString line;
    int x,y;
    bool ok;

    QFile file(filename);
    y = 0;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        do {
            line = in.readLine();
            if(!line.isNull())
            {
                if(line.length() > MAX_X) return ERROR_WIDTH;
                for(x=0; x<line.length(); x++)
                    map[x][y] = (char)line.mid(x, 1).toInt(&ok);
                width = line.length();
                y++;
            }
        } while (!line.isNull());
        if(y > MAX_Y) return ERROR_HEIGHT;
        height = y;
    }
    else
    {
        qDebug() << QString("file %1 not found").arg(filename);
    }
    return NO_ERROR_ADV;
}
//--------------------------------------------------------------------------------
void Map::set_max_x(int value)
{
    width = value;
}
//--------------------------------------------------------------------------------
void Map::set_max_y(int value)
{
    height = value;
}
//--------------------------------------------------------------------------------
int Map::get_max_x(void)
{
    return width;
}
//--------------------------------------------------------------------------------
int Map::get_max_y(void)
{
    return height;
}
//--------------------------------------------------------------------------------
int Map::set(int x, int y, char value)
{
    if(x > MAX_X) return ERROR_WIDTH;
    if(y > MAX_Y) return ERROR_HEIGHT;

    map[x][y] = value;

    return NO_ERROR_ADV;
}
//--------------------------------------------------------------------------------
char Map::get(int x, int y)
{
    return map[x][y];
}
//--------------------------------------------------------------------------------
