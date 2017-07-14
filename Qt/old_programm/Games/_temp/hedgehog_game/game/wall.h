#ifndef WALL_H
#define WALL_H
#include "block.h"

//! неактивный (непроходимый) блок
class Wall: public Block {
  Q_OBJECT
public:
  explicit Wall(QWidget *parent = 0);
};
#endif // WALL_H
