/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef TRADERPLOT_HPP
#define TRADERPLOT_HPP
//--------------------------------------------------------------------------------
#include <QPointF>

#include <qwt_plot.h>
//--------------------------------------------------------------------------------
class QwtPlotCurve;
class QwtPlotPicker;
class CurveData;
//--------------------------------------------------------------------------------
class TraderPlot : public QwtPlot
{
public:
    TraderPlot();

    void append_point(const QPointF &point);
    void append_point(float a, float b, float c, float d);

private:
    CurveData *data_curve = 0;
    QwtPlotCurve *curve = 0;
    QwtPlotPicker *d_picker = 0;
};
//--------------------------------------------------------------------------------
#endif // TRADERPLOT_HPP
