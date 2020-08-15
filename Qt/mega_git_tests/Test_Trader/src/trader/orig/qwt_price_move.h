#ifndef QWT_PRICE_MOVE_H
#define QWT_PRICE_MOVE_H

#include "qwt_global.h"
#include "qwt_double_interval.h"

/*!
  The QwtPriceMove class describes the price movements of an equity over time.
*/
class QWT_EXPORT QwtPriceMove
{
public:

    inline QwtPriceMove():
        open(0.0), high(0.0), low(0.0), close(0.0)
    {
    }

    inline QwtPriceMove(const QwtDoubleInterval &intv, double o, double h,
        double l, double c):
        open(o), high(h), low(l), close(c), interval(intv)
    {
    }

    bool operator==(const QwtPriceMove &other) const
    {
        return interval == other.interval && open == other.open
            && high == other.high && low == other.low && close == other.close;
    }

    bool operator!=(const QwtPriceMove &other) const
    {
        return !operator==(other);
    }

    double open, high, low, close;
    QwtDoubleInterval interval;
};

#endif
