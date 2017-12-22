#include "gamewidget.h"
#include "cloud.h"
#include "wall.h"
#include "actor.h"

#include <QGridLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QPropertyAnimation>

GameWidget::GameWidget(QWidget *parent): QWidget(parent),
    Delay(250),
    ActorShift(QPoint(0, -15)),
    m_state(GameWidgetState::Start),
    m_scene(new QGraphicsScene(this)),
    m_view(new QGraphicsView(m_scene, this)),
    m_actor(nullptr) {

    QGridLayout *layout = new QGridLayout(this);

    layout->setMargin(0);
    layout->addWidget(m_view, 0, 0, 1, 1);
}

void GameWidget::resizeEvent(QResizeEvent *) {
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void GameWidget::load(int n, int m, QString data) {
    if (Animate == m_state)
        return;

    m_scene->clearFocus();

    m_scene->clear();

    m_state = GameWidgetState::Start;

    m_items.resize(n);

    m_scene->setSceneRect(0.f, 0.f, n * BlockParam::BlockSize, m * BlockParam::BlockSize);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);

    for (int i = 0; i < n; ++i) {
        m_items[i].resize(m);
        for (int j = 0; j < m; ++j)
            itemAdd(i, j, data[i * m + j].toLatin1());
    }
}

void GameWidget::onBlockClicked() {
    Block* block = static_cast<Block*>(sender());
    blockClickedHandler(block);
}

void GameWidget::blockClickedHandler(Block *block) {
    int delay, // задержка уничтожения блоков с облаками
            di, dj, // смещение нового выделенного блока относительно актора
            i, j;

    QPoint
            actorPos, // позиция актора в относительных координатах
            pos; // позиция клетки, по которой кликнули

    pos = block->pos() / BlockParam::BlockSize;

    switch (m_state) {
    case GameWidgetState::Animate:
        break;
    case GameWidgetState::Start:
        m_state = GameWidgetState::Animate;

        itemAdd(pos.y(), pos.x(), ActorType);

        m_state = GameWidgetState::BlockSelected;
        break;
    case GameWidgetState::BlockSelected:
        actorPos = (m_actor->pos() - ActorShift) / BlockParam::BlockSize;


        di = pos.y() - actorPos.y();
        dj = pos.x() - actorPos.x();

        di = di < 0 ? -1 : (bool) di;
        dj = dj < 0 ? -1 : (bool) dj;

        delay = 0;

        m_state = GameWidgetState::Animate;

        if ((di && dj) || (!di && !dj)) {
            // выбран блок не на линии с ежом или блок под ежом
            m_state = GameWidgetState::BlockSelected;
            return;
        }

        if (di > 0) m_actor->state(Direction::Down);
        else if (di < 0) m_actor->state(Direction::Up);
        else if (dj > 0) m_actor->state(Direction::Right);
        else m_actor->state(Direction::Left);

        // удаляю клетки в направлении щелчка от актора
        for (i = actorPos.y(), j = actorPos.x(); !isBorder(i + di, j + dj); i += di, j += dj) {
            QTimer::singleShot(delay, m_items[i][j], SLOT(bang()));
            delay += Delay;
            m_items[i][j] = nullptr;
        }

        QPropertyAnimation *animation = new QPropertyAnimation(m_actor, "pos");
        animation->setDuration(delay);
        animation->setStartValue(m_actor->pos());
        animation->setEndValue(QPoint(BlockParam::BlockSize * j, BlockParam::BlockSize * i) + ActorShift);

        connect(animation, SIGNAL(finished()), this, SLOT(onMoveFinished()));

        animation->start();
        break;
    }
}

void GameWidget::onMoveFinished() {
    delete sender();

    m_state = GameWidgetState::BlockSelected;
    m_actor->state(Direction::Idle);

    int n = 0;
    foreach (auto t, m_items)
        n += count_if (t.begin(), t.end(), [](Block* tt) {return dynamic_cast<Cloud*>(tt);});

    if (n == 1) {
        emit gameFinished(true);
        return;
    }

    QPoint pos = (m_actor->pos() - ActorShift) / BlockParam::BlockSize;

    if (isBorder(pos.y() + 1, pos.x()) && isBorder(pos.y(), pos.x() + 1)
            && isBorder(pos.y() - 1, pos.x()) && isBorder(pos.y(), pos.x() - 1))
        emit gameFinished(false);
}

void GameWidget::itemAdd(int i, int j, char type) {
    switch (type) {
    case WallType:
        m_items[i][j] = new Wall();
        m_scene->addWidget(m_items[i][j])->setZValue(WallLayer);
        m_items[i][j]->move(j * BlockParam::BlockSize, i * BlockParam::BlockSize);
        break;
    case CloudType:
        m_items[i][j] = new Cloud();
        m_scene->addWidget(m_items[i][j])->setZValue(CloudLayer);
        connect(m_items[i][j], SIGNAL(clicked()), SLOT(onBlockClicked()));
        m_items[i][j]->move(j * BlockParam::BlockSize, i * BlockParam::BlockSize);
        break;
    case ActorType:
        m_actor = new Actor();
        m_scene->addWidget(m_actor)->setZValue(ActorLayer);
        m_actor->move(QPoint(j * BlockParam::BlockSize, i * BlockParam::BlockSize) + ActorShift);
        break;
    default:
        throw QString("GameWidget::load : bad block type");
    }
}

bool GameWidget::isBorder(int i, int j) {
    return i < 0 || j < 0 ||
            i >= (int)m_items.size() || j >= (int)m_items[0].size()
            || nullptr == dynamic_cast<Cloud*>(m_items[i][j]);
}
