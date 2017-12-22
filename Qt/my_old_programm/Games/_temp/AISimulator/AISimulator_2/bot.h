#ifndef BOT_H
#define BOT_H

#include <QRect>

class Bot {
public:
    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN,

        DIRECTION_COUNT
    };

    static Direction reverseDirection( Direction direction ) {
        switch( direction ) {
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
        case UP: return DOWN;
        case DOWN: return UP;
        default: return LEFT;
        }
    }

    static const int DEFAULT_SPEED = 2;
    static const int DEFAULT_SIZE = 12;

public:
    Bot( int x, int y, int type, int size = DEFAULT_SIZE, int speed = DEFAULT_SPEED );

    int getID() const;

    int getType() const;

    int getX() const;

    int getY() const;

    void move();
    void move( int distance );

    int getSize() const;

    int squareDistanceTo( const Bot& bot ) const;

    Direction getDirection() const;
    void setDirection( Direction value );

    bool isMoving() const;
    void startMoving();
    void stopMoving();

    int getSpeed() const;

    bool hasCollisions( const Bot& other ) const;

    void die();
    bool isDead() const;

private:
    void setX( int x );
    void setY( int y );

private:
    static int counter;

private:
    int m_id;

    int m_type;

    QRect m_rect;

    Direction m_direction;

    bool m_moving;

    int m_speed;

    bool m_dead;
};

#endif // BOT_H
