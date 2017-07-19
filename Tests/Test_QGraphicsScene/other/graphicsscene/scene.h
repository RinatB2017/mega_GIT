#ifndef SCENE_H
# define SCENE_H
# include "figures/figure.h"
# include "figures/figuretype.h"
# include <QGraphicsScene>

class Scene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit Scene(QObject *parent = 0);
signals:
public slots:
  void on_tool_changed(FigureType);
protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

  FigureType m_figureType;
};

#endif // SCENE_H
