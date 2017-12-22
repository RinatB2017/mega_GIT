#include "bot.h"

Bot::Bot( int x, int y, int type, int size, int speed ) :
    m_id( counter++ ), m_type( type ), m_rect( x - size / 2 + 1, y - size / 2 + 1, size, size ), m_direction( RIGHT ), m_moving( false ), m_speed( speed ), m_dead( false ) {
}

int Bot::getID() const {
    return m_id;
}

int Bot::getType() const {
    return m_type;
}

int Bot::getX() const {
    return m_rect.center().x();
}

void Bot::setX( int x ) {
    m_rect.moveCenter( QPoint( x, getY() ) );
}

int Bot::getY() const {
    return m_rect.center().y();
}

void Bot::setY( int y ) {
    m_rect.moveCenter( QPoint( getX(), y ) );
}

void Bot::move() {
    move( getSpeed() );
}

void Bot::move( int distance ) {
    if( !isMoving() ) {
        return;
    }

    switch( m_direction ) {
    case Bot::LEFT:
        setX( getX() - distance );
        break;
    case Bot::RIGHT:
        setX( getX() + distance );
        break;
    case Bot::UP:
        setY( getY() - distance );
        break;
    case Bot::DOWN:
        setY( getY() + distance );
        break;
    default:
        break;
    }
}

int Bot::getSize() const {
    return m_rect.size().width();
}

int Bot::squareDistanceTo( const Bot& bot ) const {
    int dx = bot.getX() - getX();
    int dy = bot.getY() - getY();
    return dx * dx + dy * dy;
}

Bot::Direction Bot::getDirection() const {
    return m_direction;
}

void Bot::setDirection( Direction value ) {
    m_direction = value;
}

bool Bot::isMoving() const {
    return m_moving;
}

void Bot::startMoving() {
    m_moving = true;
}

void Bot::stopMoving() {
    m_moving = false;
}

int Bot::getSpeed() const {
    return m_speed;
}

bool Bot::hasCollisions( const Bot& other ) const {
    return m_rect.intersects( other.m_rect );
}

void Bot::die() {
    m_dead = true;
}

bool Bot::isDead() const {
    return m_dead;
}


int Bot::counter = 0;
