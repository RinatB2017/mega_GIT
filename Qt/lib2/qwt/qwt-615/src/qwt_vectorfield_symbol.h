/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_VECTOR_FIELD_SYMBOL_H
#define QWT_VECTOR_FIELD_SYMBOL_H

#include "qwt_global.h"

class QPainter;
class QPainterPath;

/*!
    Defines abstract interface for arrow drawing routines.

    Arrow needs to be drawn horizontally with arrow tip at coordinate 0,0.
    arrowLength() shall return the entire length of the arrow (needed
    to translate the arrow for tail/centered alignment).
    setArrowLength() defines arror length in pixels (screen coordinates). It
    can be implemented to adjust other geometric properties such as
    the head size and width of the arrow. It is _always_ called before
    paint().

    A new arrow implementation can be set with QwtPlotVectorField::setArrowSymbol(), whereby
    ownership is transferred to the plot field.
*/
class QWT_EXPORT QwtVectorFieldSymbol
{
public:
    QwtVectorFieldSymbol();
    virtual ~QwtVectorFieldSymbol();

    virtual void setLength( qreal length ) = 0;
    virtual double length() const = 0;

    virtual void paint( QPainter * ) const = 0;

private:
    Q_DISABLE_COPY(QwtVectorFieldSymbol)
};

/*!
    Arrow implementation that draws a filled arrow with outline, using
    a triangular head of constant width.
 */
class QWT_EXPORT QwtVectorFieldArrow : public QwtVectorFieldSymbol
{
public:
    QwtVectorFieldArrow( double headWidth = 6.0, double tailWidth = 1.0 );
    virtual ~QwtVectorFieldArrow() QWT_OVERRIDE;

    virtual void setLength( qreal length ) QWT_OVERRIDE;
    virtual double length() const QWT_OVERRIDE;
    virtual void paint( QPainter * ) const QWT_OVERRIDE;

private:
    class PrivateData;
    PrivateData *d_data;
};

/*!
    Arrow implementation that only used lines, with optionally a filled arrow or only
    lines.
 */
class QWT_EXPORT QwtVectorFieldThinArrow : public QwtVectorFieldSymbol
{
public:
    QwtVectorFieldThinArrow( double headWidth = 6.0 );
    virtual ~QwtVectorFieldThinArrow() QWT_OVERRIDE;

    virtual void setLength( qreal length ) QWT_OVERRIDE;
    virtual double length() const QWT_OVERRIDE;
    virtual void paint( QPainter * ) const QWT_OVERRIDE;

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
