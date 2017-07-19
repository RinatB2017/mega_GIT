#include "figures/rectfigure.h"
#include <QPainter>

RectFigure::RectFigure() { }

void RectFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                        QWidget*) {
  painter->setPen(Qt::black);
  painter->setBrush(Qt::white);

  QPoint
    p0(-m_width / 2, -m_height / 2), p1(m_width / 2, -m_height / 2),
    p2(m_width / 2, m_height / 2), p3(-m_width / 2, m_height / 2);
  painter->drawLine(p0, p1);
  painter->drawLine(p1, p2);
  painter->drawLine(p2, p3);
  painter->drawLine(p3, p0);
}
