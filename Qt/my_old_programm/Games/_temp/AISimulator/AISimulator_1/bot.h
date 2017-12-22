#ifndef BOT_H
#define BOT_H

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
    Bot( int x, int y, int size = DEFAULT_SIZE, int speed = DEFAULT_SPEED );

    int getID() const;

    int getX() const;
    void setX( int x );

    int getY() const;
    void setY( int y );

    void move();
    void move( int distance );

    int getSize() const;

    Direction getDirection() const;
    void setDirection( Direction value );

    bool isMoving() const;
    void startMoving();
    void stopMoving();

    int getSpeed() const;

private:
    static int counter;

private:
    int m_id;

    int m_x;
    int m_y;

    int m_size;

    Direction m_direction;

    bool m_moving;

    int m_speed;
};

#endif // BOT_H
