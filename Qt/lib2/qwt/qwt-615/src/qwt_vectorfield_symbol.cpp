/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_vectorfield_symbol.h"

#include <qpainter.h>
#include <qpainterpath.h>

QwtVectorFieldSymbol::QwtVectorFieldSymbol()
{
}

QwtVectorFieldSymbol::~QwtVectorFieldSymbol()
{
}

class QwtVectorFieldArrow::PrivateData
{
public:
    PrivateData( double headW, double tailW ):
        headWidth( headW ),
        tailWidth( tailW ),
        length( headW + 4.0 )
    {
        /*
            Arrow is drawn horizontally, pointing into positive x direction
            with tip at 0,0.
        */

        path.lineTo( -headWidth, headWidth );
        path.lineTo( -headWidth, tailWidth );
        path.lineTo( -length, tailWidth );
        path.lineTo( -length, -tailWidth );
        path.lineTo( -headWidth, -tailWidth );
        path.lineTo( -headWidth, -headWidth );

        path.closeSubpath();
    }
        
    void setTailLength( qreal length )
    {
        length = headWidth + length;

        path.setElementPositionAt( 3, -length, tailWidth );
        path.setElementPositionAt( 4, -length, -tailWidth );
    }

    const double headWidth;
    const double tailWidth;
    double length;

    QPainterPath path;

};

QwtVectorFieldArrow::QwtVectorFieldArrow( double headWidth, double tailWidth )
{
    d_data = new PrivateData( headWidth, tailWidth );
}

QwtVectorFieldArrow::~QwtVectorFieldArrow()
{
    delete d_data;
}

void QwtVectorFieldArrow::setLength( qreal length )
{
    if ( length < d_data->headWidth )
        d_data->setTailLength( 0 ); // only the triangle will be drawn
    else
        d_data->setTailLength( length - d_data->headWidth );
}

double QwtVectorFieldArrow::length() const
{
    return d_data->length;
}

void QwtVectorFieldArrow::paint( QPainter *painter ) const
{
    painter->drawPath( d_data->path );
}

class QwtVectorFieldThinArrow::PrivateData
{
public:
    PrivateData( double headW ):
        headWidth( headW ),
        length( headW + 4.0 )
    {
        path.lineTo( -headWidth, headWidth * 0.6 );
        path.moveTo( 0, 0 );
        path.lineTo( -headWidth, -headWidth * 0.6 );
        path.moveTo( 0, 0 );
        path.lineTo( -length, 0 );
    }

    const double headWidth;
    double length;

    QPainterPath path;
};

QwtVectorFieldThinArrow::QwtVectorFieldThinArrow( double headWidth )
{
    d_data = new PrivateData( headWidth );
}

QwtVectorFieldThinArrow::~QwtVectorFieldThinArrow()
{
    delete d_data;
}

void QwtVectorFieldThinArrow::setLength( qreal length )
{
    d_data->length = length;

    const qreal headWidth = qMin( d_data->headWidth, length / 3.0 );

    QPainterPath& path = d_data->path;

    path.setElementPositionAt( 1, -headWidth, headWidth * 0.6 );
    path.setElementPositionAt( 3, -headWidth, -headWidth * 0.6 );
    path.setElementPositionAt( 5, -length, 0 );
}

double QwtVectorFieldThinArrow::length() const
{
    return d_data->length;
}

void QwtVectorFieldThinArrow::paint(QPainter * p) const
{
    p->drawPath( d_data->path );
}
