#ifndef QWT_PLOT_PRICE_H
#define QWT_PLOT_PRICE_H

#include <qglobal.h>
#include <qcolor.h>

#include "qwt_plot_seriesitem.h" 
#include "qwt_price_move.h"

class QWT_EXPORT QwtPlotPrice: public QwtPlotSeriesItem //<QwtPriceMove>
{
public:
    enum CurveStyle
    {
        NoCurve,
        OHLC,               // Open-High-Low-Close
        Candlestick,        // Japanese candlesticks
        UserCurve = 100
    };

    explicit QwtPlotPrice(const QString &title = QString::null);
    explicit QwtPlotPrice(const QwtText &title);
    virtual ~QwtPlotPrice();

    virtual int rtti() const;

    void setPen(const QPen &);
    const QPen &pen() const;

    void setUpBrush(const QBrush &);
    const QBrush &upBrush() const;

    void setDownBrush(const QBrush &);
    const QBrush &downBrush() const;

    void setData(const QwtArray<QwtPriceMove> &data);
    void setData(const QwtSeriesData<QwtPriceMove> &data);

    void setStyle(CurveStyle style);
    CurveStyle style() const;

    virtual void draw(QPainter *, const QwtScaleMap &xMap,
                      const QwtScaleMap &yMap, const QRect &) const;

    virtual void draw(QPainter *p,
                      const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                      int from, int to) const;

    virtual void updateLegend(QwtLegend *) const;

protected:
    void drawCandlesticks(QPainter *,
                          const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                          int from, int to) const;
    void drawOHLCs(QPainter *,
                   const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                   int from, int to) const;

private:
    void init();
    class PrivateData;
    PrivateData *d_data;
};

#endif
