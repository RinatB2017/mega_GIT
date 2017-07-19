#ifndef FIGURE_H
# define FIGURE_H
# include <QGraphicsItem>

class Figure : public QGraphicsItem {
public:
  Figure();
protected:
  virtual QRectF boundingRect() const;

  float m_height, m_width;
};

#endif // FIGURE_H
