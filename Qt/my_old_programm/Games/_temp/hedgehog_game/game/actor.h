#ifndef ACTOR_H
#define ACTOR_H
#include "block.h"

enum Direction {
  Idle, Left, Right, Up, Down
};

class QMediaPlayer;
class QMediaPlaylist;

//! игровой персонаж
class Actor: public Block {
  Q_OBJECT
public:
  explicit Actor(QWidget *parent = 0);
  void state(Direction);
protected:
  QMediaPlayer *m_player;
  QMediaPlaylist *m_playlist;
};
#endif // ACTOR_H
