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
    QColor color;
    b2Body *body;
    b2Fixture *fixture;
};

struct UserData
{
    QString name;
    int index;
};

struct Wall_param
{
    qreal x;
    qreal y;
    qreal w;
    qreal h;
    QColor color;
    qreal angle;
    b2BodyType type;
    int index;
};

struct Polygon_param
{
    float x;
    float y;
    QColor color;
    float angle;
    b2BodyType type;
    int index;
};

struct Ball_param
{
    b2Vec2 pos;
    float radius;
    QColor color;
    b2BodyType type;
    int index;
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

    void createWall(Wall_param param);  //b2_staticBody

    void createPolygon(Polygon_param param,
                       b2Vec2 vertices[],
                       int count);

    Object createBall(Ball_param param);

    void drawWall(QPainter *painter,
                  const Object& obj);
    void drawEllipse(QPainter *painter,
                     const Object& obj);
    void drawPolygon(QPainter *painter,
                     const Object& obj);

    void start(void);
    void stop(void);
    void timerEvent(QTimerEvent *event);

    void delete_objects(void);
    void test(void);

public slots:
    void create_borders(void);

    void create_scene_0(void);
    void create_scene_1(void);
    void create_scene_2(void);
    void create_scene_3(void);
    void create_scene_4(void);

    void add_wall(Wall_param param);
    void add_ball(Ball_param param);
    void add_bullet(Ball_param param,
                    qreal linear_velocity_x,
                    qreal linear_velocity_y,
                    qreal impulse_x,
                    qreal impulse_y,
                    qreal point_x,
                    qreal point_y);

    qreal pt_to_pixel_get(void);
    void pt_to_pixel_set(qreal value);

    void w_clear(void);

private:
    b2World *_world = nullptr;
    int _timerId = 0;
    QTransform _transform;
    QVector<Object> _objects;

    bool is_busy = false;

    int  cnt = 0;
    float timeStep = 1.0f;

    int32 velocityIterations = 6;   //8
    int32 positionIterations = 2;   //3

    // габариты сцены
    float scene_w = 20.0f;
    float scene_h = 10.0f;
    float wall_h = 0.01f;

    qreal k_pt_to_pixel = 59.9;

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
