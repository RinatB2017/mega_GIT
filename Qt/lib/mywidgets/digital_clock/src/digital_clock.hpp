/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef DIGITAL_CLOCK_HPP
#define DIGITAL_CLOCK_HPP
//--------------------------------------------------------------------------------
#include <QLabel>
#include <QTimer>
#include <QTime>
//--------------------------------------------------------------------------------
class Digital_clock : public QLabel
{
    Q_OBJECT

public:
    explicit Digital_clock(QWidget *parent = nullptr);
    ~Digital_clock();

private:
    QTimer *timer = nullptr;
    QTime time;

    int hour = 0;
    int min  = 0;
    int sec  = 0;

    void init(void);
    void timeout(void);
};
//--------------------------------------------------------------------------------
#endif // DIGITAL_CLOCK_HPP
