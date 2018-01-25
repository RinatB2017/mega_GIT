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
#ifndef DAT3_HPP
#define DAT3_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QFileInfo>
#include <QFile>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
class QTableWidget;
//--------------------------------------------------------------------------------
class DAT3 : public QObject
{
    Q_OBJECT
public:
    explicit DAT3(QObject *parent = 0);

    bool read_file(const QString filename);
    quint64 get_cnt_read_events(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send_event(pet_event);

private:
    QFile file;
    quint64 cnt_read_events = 0;
};
//--------------------------------------------------------------------------------
#endif
