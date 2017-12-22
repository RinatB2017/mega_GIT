#include "bot.h"

Bot::Bot( int x, int y , int size, int speed ) :
    m_id( counter++ ), m_x( x ), m_y( y ), m_size( size ), m_direction( RIGHT ), m_moving( false ), m_speed( speed ) {
}

int Bot::getID() const {
    return m_id;
}

int Bot::getX() const {
    return m_x;
}

void Bot::setX( int x ) {
    m_x = x;
}

int Bot::getY() const {
    return m_y;
}

void Bot::setY( int y ) {
    m_y = y;
}

void Bot::move( Direction direction, int distance ) {
    switch( direction ) {
    case Bot::LEFT:
        m_x -= distance;
        break;
    case Bot::RIGHT:
        m_x += distance;
        break;
    case Bot::UP:
        m_y -= distance;
        break;
    case Bot::DOWN:
        m_y += distance;
        break;
    default:
        break;
    }
}

int Bot::getSize() const {
    return m_size;
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


int Bot::counter = 0;
