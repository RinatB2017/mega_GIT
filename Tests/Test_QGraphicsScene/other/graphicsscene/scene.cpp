#include "scene.h"
#include "figures/rectfigure.h"
#include "figures/rhombfigure.h"

#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent) : QGraphicsScene(parent), m_figureType(None) { }

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (None == m_figureType) return;

  QGraphicsItem *item;

  switch (m_figureType) {
  case Rect:
    item = new RectFigure();
    break;
  case Rhomb:
    item = new RhombFigure();
    break;
  }

  item->setPos(event->scenePos());
  addItem(item);
}

void Scene::on_tool_changed(FigureType type) { m_figureType = type; }
