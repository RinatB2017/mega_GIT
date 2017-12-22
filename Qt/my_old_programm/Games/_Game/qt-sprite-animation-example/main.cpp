#include <QApplication>
#include "view.h"
#include "animationpool.h"
#include "animations_enum.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  ACTOR_ANIMATION_POOL.load(ActorActions::Down, ":/resources/hedgehog_down.png", 11, 246, 280);
  ACTOR_ANIMATION_POOL.load(ActorActions::Up, ":/resources/hedgehog_up.png", 11, 246, 280);
  ACTOR_ANIMATION_POOL.load(ActorActions::Left, ":/resources/hedgehog_left.png", 11, 246, 280);
  ACTOR_ANIMATION_POOL.load(ActorActions::Right, ":/resources/hedgehog_right.png", 11, 246, 280);
  ACTOR_ANIMATION_POOL.load(ActorActions::Stay, ":/resources/hedgehog_stay.png", 31, 246, 280);

  View view;
  view.show();

  return a.exec();
}
