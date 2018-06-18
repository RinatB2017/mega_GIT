/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
* Qwt Widget Library
* Copyright (C) 1997   Josef Wilgen
* Copyright (C) 2002   Uwe Rathmann
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the Qwt License, Version 1.0
*****************************************************************************/

#include <qglobal.h>
#if QT_VERSION >= 0x040000
#include <qalgorithms.h>
#else
#include <qtl.h>
#endif

#include "qwt_math.h"
#include "qwt_double_interval.h"

QwtDoubleInterval QwtDoubleInterval::normalized() const
{
    if ( !isValid() )
    {
        return QwtDoubleInterval(d_maxValue, d_minValue);
    }
    else
        return *this;
}

QwtDoubleInterval QwtDoubleInterval::inverted() const
{
    return QwtDoubleInterval(d_maxValue, d_minValue);
}

bool QwtDoubleInterval::contains(double value) const
{
    if ( !isValid() )
        return false;

    return (value >= d_minValue) && (value <= d_maxValue);
}

QwtDoubleInterval QwtDoubleInterval::unite(
        const QwtDoubleInterval &interval) const
{
    if ( !isValid() )
    {
        if ( !interval.isValid() )
            return QwtDoubleInterval();
        else
            return interval;
    }
    if ( !interval.isValid() )
        return *this;

    const double minValue = qMin(d_minValue, interval.minValue());
    const double maxValue = qMax(d_maxValue, interval.maxValue());

    return QwtDoubleInterval(minValue, maxValue);
}

QwtDoubleInterval QwtDoubleInterval::intersect(
        const QwtDoubleInterval &interval) const
{
    if ( !interval.isValid() || !isValid() )
        return QwtDoubleInterval();

    QwtDoubleInterval i1 = *this;
    QwtDoubleInterval i2 = interval;

    if ( i1.minValue() > i2.minValue() )
        qSwap(i1, i2);

    if ( i1.maxValue() < i2.minValue() )
        return QwtDoubleInterval();

    return QwtDoubleInterval(i2.minValue(),
                             qMin(i1.maxValue(), i2.maxValue()));
}

QwtDoubleInterval& QwtDoubleInterval::operator|=(
        const QwtDoubleInterval &interval)
{
    *this = *this | interval;
    return *this;
}

QwtDoubleInterval& QwtDoubleInterval::operator&=(
        const QwtDoubleInterval &interval)
{
    *this = *this & interval;
    return *this;
}

bool QwtDoubleInterval::intersects(const QwtDoubleInterval &interval) const
{
    if ( !isValid() || !interval.isValid() )
        return false;

    QwtDoubleInterval i1 = *this;
    QwtDoubleInterval i2 = interval;

    if ( i1.minValue() > i2.minValue() )
        qSwap(i1, i2);

    return i1.maxValue() >= i2.minValue();
}

QwtDoubleInterval QwtDoubleInterval::symmetrize(double value) const
{
    if ( !isValid() )
        return *this;

    const double delta =
            qMax(qAbs(value - d_maxValue), qAbs(value - d_minValue));

    return QwtDoubleInterval(value - delta, value + delta);
}

QwtDoubleInterval QwtDoubleInterval::limited(
        double lBound, double hBound) const
{
    if ( !isValid() || lBound > hBound )
        return QwtDoubleInterval();

    double minValue = qMax(d_minValue, lBound);
    minValue = qMin(minValue, hBound);

    double maxValue = qMax(d_maxValue, lBound);
    maxValue = qMin(maxValue, hBound);

    return QwtDoubleInterval(minValue, maxValue);
}

QwtDoubleInterval QwtDoubleInterval::extend(double value) const
{
    if ( !isValid() )
        return *this;

    return QwtDoubleInterval(
                qMin(value, d_minValue), qMax(value, d_maxValue) );
}

QwtDoubleInterval& QwtDoubleInterval::operator|=(double value)
{
    *this = *this | value;
    return *this;
}
