//--------------------------------------------------------------------------------
#ifndef WORLD_HPP
#define WORLD_HPP
//--------------------------------------------------------------------------------
#include <QTimerEvent>
#include <QPolygonF>
#include <QtMath>
#include <Box2D/Box2D.h>
//--------------------------------------------------------------------------------
//#ifdef BOX2D_241
//#   define float float
//#endif
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

signals:
    void cnt_objects(int);

public:
    explicit World(QWidget *parent = nullptr);
    virtual ~World();

    Object createWall(qreal x,
                      qreal y,
                      qreal w,
                      qreal h,
                      qreal angle = 0,
                      b2BodyType type = b2_staticBody);

    Object createPolygon(float x,
                         float y,
                         b2Vec2 vertices[],
                         int count,
                         float angle,
                         b2BodyType = b2_staticBody);

    Object createBall(const b2Vec2& pos,
                      float radius,
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

public slots:
    void block_insert_objects(bool state);

    void create_scene_0(void);
    void create_scene_1(void);
    void create_scene_2(void);
    void create_scene_3(void);
    void create_scene_4(void);

    void add_wall(qreal x,
                  qreal y,
                  qreal w,
                  qreal h,
                  qreal a);
    void add_ball(qreal x,
                  qreal y,
                  qreal r);

    qreal pixel_to_pt_get(void);
    void pixel_to_pt_set(qreal value);
    qreal pt_to_pixel_get(void);
    void pt_to_pixel_set(qreal value);

    void w_clear(void);

private:
    b2World *_world = nullptr;
    int _timerId = 0;
    QTransform _transform;
    QVector<Object> _objects;
    bool flag_block_insert_objects;

    bool is_busy = false;

    int  cnt = 0;
    float timeStep = 1.0f;

    int32 velocityIterations = 6;   //8
    int32 positionIterations = 2;   //3

    qreal k_pixel_to_pt = 10;
    qreal k_pt_to_pixel = WIDTH / 20;

    void insert_objects(void);

    void create_borders(void);

    qreal pixel_to_pt(qreal value);
    qreal pt_to_pixel(qreal value);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // WORLD_HPP
