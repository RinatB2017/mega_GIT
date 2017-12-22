#ifndef GRAPHICSACTOR_H
#define GRAPHICSACTOR_H

#include "animatedgraphicsitem.h"
#include "animations_enum.h"

class QGraphicsItemAnimation;

class GraphicsActor : public AnimatedGraphicsItem {
    Q_OBJECT
public:
    explicit GraphicsActor(QObject *parent = 0);
public slots:
    void processKey(ActorActions key);
    void setSprites(ActorActions sprites, bool force = false);

    void onAnimationFinished();
protected:
    const int AnimationPeriodMS = 1000;
    const int SpeedPx = 30;
    ActorActions m_currectAction;
    QGraphicsItemAnimation *m_moveAnimation;
};

#endif // GRAPHICSACTOR_H
