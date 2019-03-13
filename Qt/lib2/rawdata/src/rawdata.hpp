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
//--------------------------------------------------------------------------------
#ifndef RAWDATA_HPP
#define RAWDATA_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QObject>
#include <QQueue>
//--------------------------------------------------------------------------------
class RawData : public QObject
{
    Q_OBJECT
public:
    explicit RawData(QObject *parent = nullptr,
                     unsigned char begin_simvol = '$',
                     unsigned char end_simvol = '\n',
                     bool append_end_simvol = true);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void clear(const QByteArray &);
    
public slots:
    void raw(const QByteArray &data);

private:
    QQueue<unsigned char> queue;
    QByteArray clear_data;

    unsigned char begin_simvol;
    unsigned char end_simvol;
    bool append_end_simvol;
};
//--------------------------------------------------------------------------------
#endif // RAWDATA_HPP
