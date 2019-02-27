#ifndef CLOUD_H
#define CLOUD_H
#include "block.h"

//! активный (проходимый) блок
class Cloud: public Block {
  Q_OBJECT
public:
  explicit Cloud(QWidget *parent = 0);
protected slots:
  void bang();
protected:
  virtual void mousePressEvent(QMouseEvent *);
signals:
  void clicked();
};
#endif // CLOUD_H
