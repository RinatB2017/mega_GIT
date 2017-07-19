#include "figures/rhombfigure.h"
#include <QPainter>

RhombFigure::RhombFigure() { }

void RhombFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                        QWidget*) {
  painter->setPen(Qt::black);
  painter->setBrush(Qt::white);

  QPoint
    p0(0, -m_height / 2), p1(m_width / 2, 0),
    p2(0, m_height / 2), p3(-m_width / 2, 0);
  painter->drawLine(p0, p1);
  painter->drawLine(p1, p2);
  painter->drawLine(p2, p3);
  painter->drawLine(p3, p0);
}
