#include "gamehelpwidget.h"
#include "block.h"
#include "actor.h"
#include "cloudhelp.h"

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

GameHelpWidget::GameHelpWidget(QWidget *parent) :
    GameWidget(parent) {
}

void GameHelpWidget::loadSolution(QList<QPoint> solution) {
    m_solution = solution;

    QPoint pt = m_solution.front();
    m_solution.pop_front();

    m_items[pt.y()][pt.x()]->hide();
    itemAdd(pt.y(), pt.x(), CloudHelpType);
}

void GameHelpWidget::blockClickedHandler(Block *block) {
    if (nullptr == dynamic_cast<CloudHelp*>(block))
        return;

    if (GameWidgetState::Animate == m_state)
        return;

    if (GameWidgetState::Start != m_state &&
            (m_actor->pos() - ActorShift) == block->pos())
        return;

    GameWidget::blockClickedHandler(block);

    if (m_solution.isEmpty())
        return;

    QPoint pt = m_solution.front();
    m_solution.pop_front();

    m_items[pt.y()][pt.x()]->hide();
    itemAdd(pt.y(), pt.x(), CloudHelpType);
}

void GameHelpWidget::itemAdd(int i, int j, char type) {
    if (type == 'h') {
        m_items[i][j] = new CloudHelp();
        m_scene->addWidget(m_items[i][j])->setZValue(CloudLayer);
        connect(m_items[i][j], SIGNAL(clicked()), SLOT(onBlockClicked()));
        m_items[i][j]->move(j * BlockParam::BlockSize, i * BlockParam::BlockSize);
    }
    else
        GameWidget::itemAdd(i, j, type);
}
