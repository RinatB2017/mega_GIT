//--------------------------------------------------------------------------------
#ifndef CURVE_DATA_HPP
#define CURVE_DATA_HPP
//--------------------------------------------------------------------------------
#include "qwt_series_data.h"
#include <QPointF>

//TODO https://github.com/eiimage/qwt/blob/master/examples/realtime/incrementalplot.cpp
// возможно, что взято отсюда
//--------------------------------------------------------------------------------
class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }

    void clearStaleVal(double limit)
    {
        const QVector<QPointF> values = d_samples;
        d_samples.clear();

        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }

        if ( index > 0 )
            d_samples += (values[index++]);

        while (index < values.size() - 1)
            d_samples += (values[index++]);

        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }

private:
    QRectF d_boundingRect;        //TODO этого не было
    QVector<QPointF> d_samples;   //TODO этого не было
};
//--------------------------------------------------------------------------------
#endif
