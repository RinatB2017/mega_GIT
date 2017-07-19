#include "figure.h"

Figure::Figure() : m_height(20), m_width(40) { }

QRectF Figure::boundingRect() const {
  return QRectF(-m_width / 2, -m_height / 2, m_width, m_height);
}
