#ifndef RECTFIGURE_H
# define RECTFIGURE_H
# include "figures/figure.h"

class RectFigure : public Figure {
public:
  RectFigure();
protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);
};

#endif // RECTFIGURE_H
