#ifndef ANIMATEDGRAPHICSITEM_H
#define ANIMATEDGRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>

#include "animationpool.h"
#include "animations_enum.h"

class AnimatedGraphicsItem : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
public:
  enum Mode { Once, Loop };
  AnimatedGraphicsItem(QObject *parent);
signals:
  void animationFinished();
protected:
  void animation(ActorActions animationId, Mode mode, bool randomStartFrame = false, int framerate = DefaultFramerate);
private slots:
  void on_timerTick();
private:
  QTimer m_timer;

  QVector<QPixmap*> *m_frames;
  int m_nFrames;
  int m_curFrameIndex;

  Mode m_mode;

  const static int DefaultFramerate = 11;
};

#endif // ANIMATEDGRAPHICSITEM_H
