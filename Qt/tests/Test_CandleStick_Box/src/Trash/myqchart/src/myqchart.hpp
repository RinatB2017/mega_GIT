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
#ifndef MYQCHART_HPP
#define MYQCHART_HPP
//--------------------------------------------------------------------------------
#include <QChartView>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE
//--------------------------------------------------------------------------------
class MyQChart : public QChart
{
    Q_OBJECT

public:
    MyQChart();
    ~MyQChart();

private:

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MYQCHART_HPP
