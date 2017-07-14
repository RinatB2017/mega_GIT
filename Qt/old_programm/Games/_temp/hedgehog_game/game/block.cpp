#include "block.h"
#include <QMovie>
#include <QLabel>

Block::Block(QWidget *parent) : QWidget(parent),
    m_animation(new QMovie(this)), m_label(new QLabel(this)) {

    setAttribute(Qt::WA_NoSystemBackground);

    m_label->setMovie(m_animation);
    m_animation->setCacheMode(QMovie::CacheMode::CacheAll);

    resize(BlockParam::BlockSize, BlockParam::BlockSize);
    m_animation->setScaledSize(size());
}

void Block::animation(QString texturename, bool randomStartFrame) {
    m_animation->stop();
    m_animation->setFileName(texturename);

    if (randomStartFrame)
        m_animation->jumpToFrame(qrand() % m_animation->frameCount());

    m_animation->start();
}
