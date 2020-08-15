//--------------------------------------------------------------------------------
#include "plot.h"
#include "griditem.h"
#include "quotefactory.h"

#include <qwt_plot_tradingcurve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_date_scale_engine.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_magnifier.h>
#include <qwt_date.h>
//--------------------------------------------------------------------------------
class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( QWidget *canvas ):
        QwtPlotZoomer( canvas )
    {
        setRubberBandPen( QColor( Qt::darkGreen ) );
        setTrackerMode( QwtPlotPicker::AlwaysOn );

        // panning with the left mouse button
        new QwtPlotPanner(canvas);

        // zoom in/out with the wheel
        new QwtPlotMagnifier(canvas);

    }

protected:
    QwtText trackerTextF( const QPointF &pos ) const
    {
        const QDateTime dt = QwtDate::toDateTime( pos.x() );

        QString s;
        s += QwtDate::toString( QwtDate::toDateTime( pos.x() ),
                                "MMM dd hh:mm ", QwtDate::FirstThursday );

        QwtText text( s );
        text.setColor( Qt::white );

        QColor c = rubberBandPen().color();
        text.setBorderPen( QPen( c ) );
        text.setBorderRadius( 6 );
        c.setAlpha( 170 );
        text.setBackgroundBrush( c );

        return text;
    }
};
//--------------------------------------------------------------------------------
class DateScaleDraw: public QwtDateScaleDraw
{
public:
    DateScaleDraw( Qt::TimeSpec timeSpec ):
        QwtDateScaleDraw( timeSpec )
    {
        // as we have dates from 2010 only we use
        // format strings without the year

        setDateFormat( QwtDate::Millisecond, "hh:mm:ss:zzz\nddd dd MMM" );
        setDateFormat( QwtDate::Second, "hh:mm:ss\nddd dd MMM" );
        setDateFormat( QwtDate::Minute, "hh:mm\nddd dd MMM" );
        setDateFormat( QwtDate::Hour, "hh:mm\nddd dd MMM" );
        setDateFormat( QwtDate::Day, "ddd dd MMM" );
        setDateFormat( QwtDate::Week, "Www" );
        setDateFormat( QwtDate::Month, "MMM" );
    }
};
//--------------------------------------------------------------------------------
class ZoneItem: public QwtPlotZoneItem
{
public:
    ZoneItem( const QString &title )
    {
        setTitle( title );
        setZ( 11 ); // on top the the grid
        setOrientation( Qt::Vertical );
        setItemAttribute( QwtPlotItem::Legend, true );
    }

    void setColor( const QColor &color )
    {
        QColor c = color;

        c.setAlpha( 100 );
        setPen( c );

        c.setAlpha( 20 );
        setBrush( c );
    }

    void setInterval( const QDate &date1, const QDate &date2 )
    {
        const QDateTime dt1( date1, QTime(), Qt::UTC );
        const QDateTime dt2( date2, QTime(), Qt::UTC );

        QwtPlotZoneItem::setInterval( QwtDate::toDouble( dt1 ),
                                      QwtDate::toDouble( dt2 ) );
    }
};
//--------------------------------------------------------------------------------
bool Plot::append(int time,
                  float open,
                  float close,
                  float low,
                  float high)
{
    QwtOHLCSample temp;

    temp.time  = time;
    temp.high  = high;
    temp.close = close;
    temp.open  = open;
    temp.low   = low;
    tickets.append(temp);

    return true;
}
//--------------------------------------------------------------------------------
void Plot::update_ticket(void)
{
    curve->setSamples(tickets);
}
//--------------------------------------------------------------------------------
Plot::Plot(const QString &ticket_name, QWidget *parent):
    QwtPlot(parent),
    ticket_name(ticket_name)
{
    setWindowTitle(ticket_name);
    setTitle(ticket_name);

    QwtDateScaleDraw *scaleDraw = new DateScaleDraw(Qt::UTC);
    QwtDateScaleEngine *scaleEngine = new QwtDateScaleEngine(Qt::UTC);

    setAxisTitle(QwtPlot::xBottom, QString("2018"));
    setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    setAxisLabelRotation(QwtPlot::xBottom, -50.0);
    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    setAxisTitle(QwtPlot::yLeft, QString("Price"));

    curve = new QwtPlotTradingCurve();
    curve->setTitle(ticket_name);
    curve->setOrientation(Qt::Vertical);
    curve->attach(this);

    // curve->setSymbolExtent(12*3600*1000.0);
    // curve->setMinSymbolWidth(3);
    // curve->setMaxSymbolWidth(15);

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    Zoomer* zoomer = new Zoomer(canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                            Qt::RightButton,
                            Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
                            Qt::RightButton);

    QwtPlotPanner *panner = new QwtPlotPanner(canvas());
    panner->setMouseButton(Qt::MidButton);
}
//--------------------------------------------------------------------------------
void Plot::setMode(int style)
{
    QwtPlotTradingCurve::SymbolStyle symbolStyle =
            static_cast<QwtPlotTradingCurve::SymbolStyle>( style );

    QwtPlotItemList curves = itemList( QwtPlotItem::Rtti_PlotTradingCurve );
    for(int i=0; i<curves.size(); i++)
    {
        QwtPlotTradingCurve *curve = static_cast<QwtPlotTradingCurve *>(curves[i]);
        curve->setSymbolStyle(symbolStyle);
    }

    replot();
}
//--------------------------------------------------------------------------------
void Plot::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    replot();
}
//--------------------------------------------------------------------------------
void Plot::exportPlot(void)
{
    QwtPlotRenderer renderer;
    renderer.exportTo(this, "stockchart.pdf");
}
//--------------------------------------------------------------------------------
QString Plot::get_ticket_name(void)
{
    return ticket_name;
}
//--------------------------------------------------------------------------------
void Plot::test(void)
{
    showItem(curve, true);
}
//--------------------------------------------------------------------------------
