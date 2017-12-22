#include "animatedgraphicsitem.h"

AnimatedGraphicsItem::AnimatedGraphicsItem(QObject *parent)
    : QObject(parent) {
    connect(&m_timer, SIGNAL(timeout()), SLOT(on_timerTick()));
}

void AnimatedGraphicsItem::animation(ActorActions animationId, Mode mode,
                                     bool randomStartFrame, int framerate) {
    m_frames = &ACTOR_ANIMATION_POOL.get(animationId);
    m_mode = mode;
    m_timer.stop();
    m_nFrames = m_frames->size();

    if (randomStartFrame)
        m_curFrameIndex = qrand() % m_nFrames;
    else
        m_curFrameIndex = 0;

    setPixmap(*(*m_frames)[m_curFrameIndex]);

    if (framerate > 0) {
        m_timer.setInterval(1000/framerate);
        m_timer.start();
    }
}

void AnimatedGraphicsItem::on_timerTick() {
    ++m_curFrameIndex;
    if (m_mode == Loop) {
        if (m_curFrameIndex >= m_nFrames)
            m_curFrameIndex = 0;
        setPixmap(*(*m_frames)[m_curFrameIndex]);
    }
    else {
        if (m_curFrameIndex >= m_nFrames) {
            m_timer.stop();
        hide();
        emit animationFinished();
    }
    else
        setPixmap(*(*m_frames)[m_curFrameIndex]);
    }
}
