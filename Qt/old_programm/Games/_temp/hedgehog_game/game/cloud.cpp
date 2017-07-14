#include "cloud.h"
#include "actor.h"
#include <QMovie>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>

Cloud::Cloud(QWidget *parent): Block(parent) {
    animation(":/game/pics/cell.gif", true);
}

/*virtual*/ void Cloud::mousePressEvent(QMouseEvent *event) {
    const QGraphicsProxyWidget *proxy = graphicsProxyWidget();
    if (proxy->scene()->itemAt(mapToParent(event->pos()), QTransform()) == proxy)
        emit clicked();
}

void Cloud::bang() {
    Block::animation(":/game/pics/cellBang.gif");
}
