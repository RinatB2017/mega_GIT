#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "graphicsactor.h"

class View : public QGraphicsView {
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);
protected slots:
    void keyPressEvent(QKeyEvent *event);
private:
    QGraphicsScene m_scene;
    GraphicsActor m_actor;
};

#endif // VIEW_H
