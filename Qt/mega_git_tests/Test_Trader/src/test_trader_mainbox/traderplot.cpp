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
#include <QBrush>

#include <qwt_picker_machine.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "curvedata.hpp"
//--------------------------------------------------------------------------------
#include "traderplot.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class RangeMarker: public QwtPlotItem
{
public:
    RangeMarker()
    {

    }

    void setInterval( QwtInterval& interval )
    {
        m_interval = interval;
    }

    virtual void draw( QPainter *painter,
                       const QwtScaleMap &xMap, const QwtScaleMap &,
                       const QRectF &canvasRect ) const
    {
        int x1 = qRound( xMap.transform( m_interval.minValue() ));
        int x2 = qRound( xMap.transform( m_interval.maxValue() ));

        painter->fillRect( QRect(x1, canvasRect.top(), x2-x1, canvasRect.height()), QBrush(Qt::red) );
    }

private:
    QwtInterval m_interval;
};

class TraderCurve: public QwtPlotCurve
{
public:
    TraderCurve()
    {

    }

#if 0
    virtual void drawDots( QPainter *p,
                           const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                           const QRectF &canvasRect, int from, int to ) const
    {
        qDebug() << "drawDots";
    }
#endif

#if 0
    virtual void drawSteps( QPainter *p,
                            const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                            const QRectF &canvasRect, int from, int to ) const
    {
        qDebug() << "drawSteps";
    }
#endif

#if 0
    virtual void drawSeries( QPainter *,
                             const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                             const QRectF &canvasRect, int from, int to ) const
    {
        qDebug() << "drawSeries";
    }
#endif

#if 0
    virtual void drawCurve( QPainter *p, int style,
                            const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                            const QRectF &canvasRect, int from, int to ) const
    {
        qDebug() << "drawCurve";
    }
#endif

};
//--------------------------------------------------------------------------------
TraderPlot::TraderPlot()
{
    qDebug() << "start";

    d_picker = new QwtPlotPicker(QwtPlot::xBottom,
                                 QwtPlot::yLeft,
                                 QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::AlwaysOn,
                                 canvas());
    d_picker->setStateMachine(new QwtPickerDragPointMachine());
    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::white));

    // panning with the left mouse button
    new QwtPlotPanner(canvas());

    // zoom in/out with the wheel
    new QwtPlotMagnifier(canvas());

    data_curve = new CurveData;

    curve = new TraderCurve();

    //curve->setStyle(QwtPlotCurve::UserCurve);
    curve->setStyle(QwtPlotCurve::Dots);
    curve->setData(data_curve);
    curve->attach(this);
}
//--------------------------------------------------------------------------------
void TraderPlot::append_point(float a, float b, float c, float d)
{
    Q_UNUSED(a)
    Q_UNUSED(b)
    Q_UNUSED(c)
    Q_UNUSED(d)
}
//--------------------------------------------------------------------------------
void TraderPlot::append_point(const QPointF &point)
{
    Q_ASSERT(data_curve);
    //qDebug() << point.x() << point.y();
    data_curve->append(point);
    //replot();
}
//--------------------------------------------------------------------------------
