#include "triangle.h"
#include <QPainter>

Triangle::Triangle(QPointF point, QObject *parent) :
    Figure(point,parent)
{
    Q_UNUSED(point)
}

Triangle::~Triangle()
{

}

// Реализуем метод отрисовки
void Triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 2));

    QPolygonF polygon;

    polygon << QPointF(startPoint().x() + (endPoint().x() > startPoint().x() ? + 1 : - 1)*
                           abs((endPoint().x() - startPoint().x())/2), startPoint().y())
                << QPointF((endPoint().x() > startPoint().x()) ? endPoint().x() : startPoint().x(),
                           endPoint().y())
                << QPointF((endPoint().x() > startPoint().x()) ? startPoint().x() : endPoint().x(),
                           endPoint().y());

    painter->drawPolygon(polygon);

    Q_UNUSED(option)
    Q_UNUSED(widget)
}
