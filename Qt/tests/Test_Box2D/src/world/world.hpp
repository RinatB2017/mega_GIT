//--------------------------------------------------------------------------------
#ifndef WORLD_HPP
#define WORLD_HPP
//--------------------------------------------------------------------------------
#include <QTimerEvent>
#include <QtOpenGL>
#include <QObject>
#include <QString>
//--------------------------------------------------------------------------------
#include <Box2D/Box2D.h>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum
{
    BallObject,
    WallObject,
    PolygonObject
};

struct Object
{
    int type;
    b2Body *body;
    b2Fixture *fixture;
};

struct UserData
{
    QString name;
    bool is_dead;
};
//--------------------------------------------------------------------------------
class World : public MyWidget
{
    Q_OBJECT

public:
    World(QWidget *parent = 0);
    ~World();

    Object createWall(float32 x,
                      float32 y,
                      float32 w,
                      float32 h,
                      float32 angle = 0,
                      b2BodyType type = b2_staticBody);

    Object createPolygon(float32 x,
                         float32 y,
                         b2Vec2 vertices[],
                         int32 count,
                         float32 angle,
                         b2BodyType = b2_staticBody);

    Object createBall(const b2Vec2& pos,
                      float32 radius,
                      int index,
                      b2BodyType type = b2_dynamicBody);

    void drawWall(QPainter *painter,
                  const Object& obj);
    void drawEllipse(QPainter *painter,
                     const Object& obj);
    void drawPolygon(QPainter *painter,
                     const Object& obj);

    void drawEllipse_B(QPainter *painter,
                       b2Body *body);


    void start(void);
    void stop(void);
    void timerEvent(QTimerEvent *event);

    void delete_objects(void);
    void test(void);

signals:
    void cnt_objects(int);

public slots:
    void block_insert_objects(bool state);

    void create_scene_0(void);
    void create_scene_1(void);
    void create_scene_2(void);
    void create_scene_3(void);

    void clear(void);

private:
    b2World *_world = 0;
    int _timerId;
    QTransform _transform;
    QVector<Object> _objects;
    bool flag_block_insert_objects;

    bool is_busy;

    int  cnt;
    void insert_objects(void);

    void create_borders(void);

    float pixel_to_pt(float value);

    void updateText(void);

protected:
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // WORLD_HPP
