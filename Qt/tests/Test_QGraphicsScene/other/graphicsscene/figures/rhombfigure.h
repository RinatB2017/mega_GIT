#ifndef RHOMBFIGURE_H
# define RHOMBFIGURE_H
# include "figures/figure.h"

class RhombFigure : public Figure {
public:
  RhombFigure();
protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);
};

#endif // RHOMBFIGURE_H
