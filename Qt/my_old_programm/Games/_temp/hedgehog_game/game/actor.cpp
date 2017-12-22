#include "actor.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

Actor::Actor(QWidget *parent): Block(parent),
    m_player(new QMediaPlayer(this)),
    m_playlist(new QMediaPlaylist(m_player)) {

    m_player->setPlaylist(m_playlist);
    m_playlist->addMedia(QUrl("qrc:/game/audio/hardStep.wav"));
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    state(Idle);
}

void Actor::state(Direction direction) {
    switch (direction) {
    case Direction::Idle:
        animation(":/game/pics/actor_stay.gif");
        m_player->stop();
        break;
    case Direction::Up:
        animation(":/game/pics/actor_up.gif");
        m_player->play();
        break;
    case Direction::Down:
        animation(":/game/pics/actor_down.gif");
        m_player->play();
        break;
    case Direction::Right:
        animation(":/game/pics/actor_right.gif");
        m_player->play();
        break;
    case Direction::Left:
        animation(":/game/pics/actor_left.gif");
        m_player->play();
        break;
    }
}
