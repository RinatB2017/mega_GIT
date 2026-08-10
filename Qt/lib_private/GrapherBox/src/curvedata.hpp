//--------------------------------------------------------------------------------
#ifndef CURVE_DATA_HPP
#define CURVE_DATA_HPP
//--------------------------------------------------------------------------------
#include "qwt_series_data.h"
#include <QPointF>
//--------------------------------------------------------------------------------
class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        if ( cachedBoundingRect.width() < 0.0 )
            cachedBoundingRect = qwtBoundingRect( *this );

        return cachedBoundingRect;
    }

    inline void append( const QPointF &point )
    {
        m_samples += point;
    }

    inline void append( const QList<QPointF> &l_point )
    {
        foreach (QPointF point, l_point)
        {
            m_samples += point;
        }
    }

    void clear()
    {
        m_samples.clear();
        m_samples.squeeze();
        cachedBoundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }

    void clearStaleVal(double limit)
    {
        const QVector<QPointF> values = m_samples;
        m_samples.clear();

        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }

        if ( index > 0 )
            m_samples += (values[index++]);

        while (index < values.size() - 1)
            m_samples += (values[index++]);

        cachedBoundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};
//--------------------------------------------------------------------------------
#endif
