#include <qstring.h>
#include <qpainter.h>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_scale_map.h>
#include <qwt_plot_price.h>

class QwtPlotPrice::PrivateData
{
public:
    PrivateData():
        curveStyle(NoCurve)
    {
    }

    ~PrivateData()
    {
    }

    QPen pen;
    QBrush upBrush;
    QBrush downBrush;
    QwtPlotPrice::CurveStyle curveStyle;
};

QwtPlotPrice::QwtPlotPrice(const QwtText &title):
    QwtPlotSeriesItem(title)
{
    init();
}

QwtPlotPrice::QwtPlotPrice(const QString &title):
    QwtPlotSeriesItem(title)
{
    init();
}

QwtPlotPrice::~QwtPlotPrice()
{
    delete d_data;
}

void QwtPlotPrice::init()
{
    d_data = new PrivateData();
    d_series = new QwtPriceMoveSeriesData();

    setUpBrush(QBrush(Qt::NoBrush));
    setDownBrush(QBrush(Qt::black));

    setItemAttribute(QwtPlotItem::AutoScale, true);
    setItemAttribute(QwtPlotItem::Legend, true);

    setZ(20.0);

}

void QwtPlotPrice::setStyle(CurveStyle style)
{
    if ( style != d_data->curveStyle )
    {
        d_data->curveStyle = style;
        itemChanged();
    }
}

QwtPlotPrice::CurveStyle QwtPlotPrice::style() const
{
    return d_data->curveStyle;
}

void QwtPlotPrice::setPen(const QPen &pen)
{
    if ( pen != d_data->pen )
    {
        d_data->pen = pen;
        itemChanged();
    }
}

const QPen &QwtPlotPrice::pen() const
{
    return d_data->pen;
}

void QwtPlotPrice::setUpBrush(const QBrush &brush)
{
    if ( brush != d_data->upBrush )
    {
        d_data->upBrush = brush;
        itemChanged();
    }
}

const QBrush &QwtPlotPrice::upBrush() const
{
    return d_data->upBrush;
}

void QwtPlotPrice::setDownBrush(const QBrush &brush)
{
    if ( brush != d_data->downBrush )
    {
        d_data->downBrush = brush;
        itemChanged();
    }
}

const QBrush &QwtPlotPrice::downBrush() const
{
    return d_data->downBrush;
}

int QwtPlotPrice::rtti() const
{
    return QwtPlotItem::Rtti_PlotPrice;
}

void QwtPlotPrice::setData(
        const QwtArray &data)
{
    QwtPlotSeriesItem::setData(
                QwtPriceMoveSeriesData(data));
}

void QwtPlotPrice::setData(
        const QwtSeriesData<QwtPriceMove> &data)
{
    QwtPlotSeriesItem<QwtPriceMove>::setData(data);
}

void QwtPlotPrice::draw(QPainter *painter, 
                        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                        const QRect &) const
{
    draw(painter, xMap, yMap, 0, -1);
}

void QwtPlotPrice::draw(QPainter *painter,
                        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                        int from, int to) const
{
    if ( !painter || dataSize() <= 0 )
        return;

    if (to < 0)
        to = dataSize() - 1;

    switch (d_data->curveStyle)
    {
    case OHLC:
        drawOHLCs(painter, xMap, yMap, from, to);
        break;
    case Candlestick:
        drawCandlesticks(painter, xMap, yMap, from, to);
        break;
    default:
        break;
    }
}
void QwtPlotPrice::drawOHLCs(QPainter *painter,
                             const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                             int from, int to) const
{
    painter->setPen(d_data->pen);

    for ( int i = from; i <= to; i++ )
    {
        const QwtPriceMove price = d_series->sample(i);
        if ( !price.interval.isNull() )
        {
            const int open = yMap.transform(price.open);
            const int close = yMap.transform(price.close);
            const int centerAxis = xMap.transform(
                        (price.interval.minValue() + price.interval.maxValue()) / 2);

            QwtPainter::drawLine(painter, centerAxis, yMap.transform(price.low),
                                 centerAxis, yMap.transform(price.high));

            QwtPainter::drawLine(painter,
                                 xMap.transform(price.interval.minValue()), open, centerAxis,
                                 open);

            QwtPainter::drawLine(painter, centerAxis, close,
                                 xMap.transform(price.interval.maxValue()), close);

        }
    }
}

void QwtPlotPrice::drawCandlesticks(QPainter *painter,
                                    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                                    int from, int to) const
{
    painter->setPen(d_data->pen);

    for ( int i = from; i <= to; i++ )
    {
        const QwtPriceMove price = d_series->sample(i);
        if ( !price.interval.isNull() )
        {
            const int open = yMap.transform(price.open);
            const int high = yMap.transform(price.high);
            const int low = yMap.transform(price.low);
            const int close = yMap.transform(price.close);

            const bool upCandle = (price.close > price.open);
            const int wickAxis = xMap.transform(
                        (price.interval.minValue() + price.interval.maxValue()) / 2);

            QwtPainter::drawLine(painter, wickAxis, (upCandle ? close : open),
                                 wickAxis, high);

            QwtPainter::drawLine(painter, wickAxis, (upCandle ? open : close),
                                 wickAxis, low);

            QRect body = QRect();
            body.setCoords(xMap.transform(price.interval.minValue()), open,
                           xMap.transform(price.interval.maxValue()), close);
#if QT_VERSION < 0x040000
            body = body.normalize();
#else
            body = body.normalized();
#endif

            QwtPainter::fillRect(painter, body,
                                 (upCandle ? upBrush() : downBrush()));
            QwtPainter::drawRect(painter, body);
        }
    }
}


void QwtPlotPrice::updateLegend(QwtLegend *) const
{
#if 0
#ifdef __GNUC__
#warning TODO
#endif
#endif
}
