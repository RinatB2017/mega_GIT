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
#ifndef ASCII_DATA_HPP
#define ASCII_DATA_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QByteArray>
//--------------------------------------------------------------------------------
class Ascii_data : public QObject
{
    Q_OBJECT

public:
    Ascii_data(void)
    {
        dirty_data.clear();
    }

signals:
    void result(QByteArray);

public slots:
    void append(QByteArray data)
    {
        if(data.isEmpty())  return;
        for(int n=0; n<data.length(); n++)
        {
            char s = data.at(n);
            switch(s)
            {
            case ':':
                dirty_data.clear();
                break;

            case '\r':
            case '\n':
                analize();
                break;

            default:
                dirty_data.append(s);
                break;
            }
        }
    }

private:
    QByteArray dirty_data;

    void analize(void)
    {
        QByteArray temp = QByteArray::fromHex(dirty_data);
        if(temp.isEmpty() == false)
        {
            emit result(temp);
        }
        dirty_data.clear();
    }
};
//--------------------------------------------------------------------------------
#endif // ASCII_DATA_HPP
