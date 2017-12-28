//--------------------------------------------------------------------------------
#include <QPainter>
#include <QWidget>
#include <QObject>
#include <QString>
#include <QVector>
//#include <QtMath>
//--------------------------------------------------------------------------------
#include "listener.hpp"
#include "defines.hpp"
#include "world.hpp"
//--------------------------------------------------------------------------------
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
//--------------------------------------------------------------------------------
World::World(QWidget *parent) :
    MyWidget(parent),
    _timerId(0)
{
    is_busy = false;

    flag_block_insert_objects = false;
    cnt = 0;

    //b2Vec2 gravity(0.0f, -10.0f);
    b2Vec2 gravity(0.0f, 0.0f);
    _world = new b2World(gravity);

    _transform.scale(10.0f, -10.0f);
    _transform.translate(0.0f, -pixel_to_pt(HEIGHT));
}
//--------------------------------------------------------------------------------
World::~World()
{
    emit info("~World()");
}
//--------------------------------------------------------------------------------
float World::pixel_to_pt(float value)
{
    return (value / 10.0f);
}
//--------------------------------------------------------------------------------
Object World::createWall(float32 x,
                         float32 y,
                         float32 w,
                         float32 h,
                         float32 angle,
                         b2BodyType type)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = type;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = angle;
    obj.body = _world->CreateBody(&bd);

    // shape
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = WallObject;

    UserData *data = new UserData;
    data->name="wall";
    data->is_dead=false;
    obj.body->SetUserData(data);

    return obj;
}
//--------------------------------------------------------------------------------
Object World::createPolygon(float32 x,
                            float32 y,
                            b2Vec2  vertices[],
                            int32   count,
                            float32 angle,
                            b2BodyType type)
{
    Object obj;
    //const int32 count = 4;
    //b2Vec2 vertices[count];

    // вершины идут ПРОТИВ часовой стрелки
    // y растет вверх
    // 0 точка должна быть в начале координат
    //vertices[0].Set(0.0f,   0.0f);
    //vertices[1].Set(10.0f,  0.0f);
    //vertices[2].Set(8.0f,   5.0f);
    //vertices[3].Set(2.0f,   5.0f);

    // body
    b2BodyDef bd;
    bd.type = type;
    bd.position = b2Vec2(x, y);
    bd.angle = angle * b2_pi;
    obj.body = _world->CreateBody(&bd);

    b2PolygonShape polygon;
    polygon.Set(vertices, count);

    // fixture
    b2FixtureDef fd;
    fd.shape = &polygon;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = PolygonObject;

    UserData *data = new UserData;
    data->name="polygon";
    data->is_dead=false;
    obj.body->SetUserData(data);

    return obj;
}
//--------------------------------------------------------------------------------
Object World::createBall(const b2Vec2& pos,
                         float32 radius,
                         int index,
                         b2BodyType type)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = type;
    bd.position = pos;
    obj.body = _world->CreateBody(&bd);

    // shape
    b2CircleShape shape;
    shape.m_radius = radius;

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = BallObject;

    UserData *data = new UserData;
    data->name=QString("ball%1").arg(index);
    data->is_dead=false;
    obj.body->SetUserData(data);

    return obj;
}
//--------------------------------------------------------------------------------
void World::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setTransform(_transform);

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#else
    QPen pen;
    pen.setWidthF(0.07f);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(pen);
#endif

    foreach(const Object& obj, _objects)
    {
        switch(obj.type)
        {
        case BallObject:
            drawEllipse(&painter, obj);
            break;

        case WallObject:
            drawWall(&painter, obj);
            break;

        case PolygonObject:
            drawPolygon(&painter, obj);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void World::drawEllipse(QPainter *painter,
                        const Object& obj)
{
    float32 x = obj.body->GetPosition().x;
    float32 y = obj.body->GetPosition().y;
    float32 r = obj.fixture->GetShape()->m_radius;

    painter->drawEllipse(QPointF(x, y), r, r);
}
//--------------------------------------------------------------------------------
void World::drawEllipse_B(QPainter *painter,
                          b2Body *body)
{
    float32 x = body->GetPosition().x;
    float32 y = body->GetPosition().y;
    float32 r = 1.0f; //body->GetShape()->m_radius;

    painter->drawEllipse(QPointF(x, y), r, r);
}
//--------------------------------------------------------------------------------
void World::drawWall(QPainter *painter,
                     const Object& obj)
{
    float32 x = obj.body->GetPosition().x;
    float32 y = obj.body->GetPosition().y;
    float32 angle = obj.body->GetAngle();

    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;
    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    painter->save();
    painter->translate(r.center());
    painter->rotate(angle*180/b2_pi);
    painter->translate(-r.center());
    painter->drawRect(r);
    painter->restore();
}
//--------------------------------------------------------------------------------
void World::drawPolygon(QPainter *painter,
                        const Object& obj)
{
    float32 x = obj.body->GetPosition().x;
    float32 y = obj.body->GetPosition().y;
    float32 angle = obj.body->GetAngle();

    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());
    int pointCount = shape->GetVertexCount();
    QPointF points[pointCount];

    float32 hx = 0.0f;
    float32 hy = 0.0f;
    for(int n=0; n<pointCount; ++n)
    {
        float32 lx = shape->GetVertex(n).x;
        float32 ly = shape->GetVertex(n).y;
        if(x>hx) hx=lx;
        if(y>hy) hy=ly;
        points[n].setX(lx);
        points[n].setY(ly);
    }

    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    painter->save();
    painter->translate(r.center());
    painter->rotate(angle*180.0f/b2_pi);
    painter->drawPolygon(points, pointCount);
    painter->translate(-r.center());
    painter->restore();
}
//--------------------------------------------------------------------------------
void World::start(void)
{
    if(!_timerId)
    {
        _timerId = startTimer(1000/60); // 60fps
    }
}
//--------------------------------------------------------------------------------
void World::stop(void)
{
    if(_timerId)
    {
        killTimer(_timerId);
        _timerId = 0;
    }
}
//--------------------------------------------------------------------------------
void World::delete_objects(void)
{
    while(_objects.count() > 0)
    {
        _objects.remove(0);
    }
}
//--------------------------------------------------------------------------------
void World::test(void)
{
    emit info(QString("cnt obj %1").arg(_objects.count()));
    //_world->Step(1.0f/60.0f, 8, 3);
    //update();
}
//--------------------------------------------------------------------------------
void World::block_insert_objects(bool state)
{
    flag_block_insert_objects = state;
}
//--------------------------------------------------------------------------------
void World::insert_objects(void)
{
#if 1
    Object ball = createBall(b2Vec2(pixel_to_pt(10), pixel_to_pt(HEIGHT - 20)), pixel_to_pt(3.0f), 0);
    ball.body->SetLinearVelocity(b2Vec2(25.0f, 0.0f));
    ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f), b2Vec2(1.0f, 1.0f), true);  //TODO

    _objects.append(ball);

    emit cnt_objects(_objects.count());
#else
    cnt++;
    if(cnt > 5)
    {
        Object ball = createBall(b2Vec2(pixel_to_pt(50), pixel_to_pt(550)), pixel_to_pt(3.0f), 0);
        ball.body->SetLinearVelocity(b2Vec2(25.0f, 0.0f));
        ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f), b2Vec2(1.0f, 1.0f));

        _objects.append(ball);

        emit cnt_objects(_objects.count());

        cnt=0;
    }
#endif
}
//--------------------------------------------------------------------------------
void World::create_borders(void)
{
    _objects.append(createWall(0.0f, 0.0f, pixel_to_pt(WIDTH), pixel_to_pt(1.0f)));                // низ
    _objects.append(createWall(0.0f, pixel_to_pt(HEIGHT), pixel_to_pt(WIDTH), pixel_to_pt(1.0f))); // верх
    _objects.append(createWall(0.0f, 0.0f, pixel_to_pt(1.0f),   pixel_to_pt(HEIGHT)));             // левая стена
    _objects.append(createWall(pixel_to_pt(WIDTH), 0.0f, pixel_to_pt(1.0f), pixel_to_pt(HEIGHT))); // правая стена
}
//--------------------------------------------------------------------------------
void World::create_scene_0(void)
{
    if(is_busy == false)
    {
        is_busy = true;
        b2Vec2 vertices[4];

        // вершины идут ПРОТИВ часовой стрелки
        // y растет вверх
        // 0 точка должна быть в начале координат
        vertices[0].Set(0.0f,   0.0f);
        vertices[1].Set(10.0f,  0.0f);
        vertices[2].Set(8.0f,   5.0f);
        vertices[3].Set(2.0f,   5.0f);
        //vertices[4].Set(2.0f,   13.0f);
        //vertices[5].Set(1.0f,   13.0f);

        Object polygon = createPolygon(pixel_to_pt(400), pixel_to_pt(400), vertices, 4, 0*DEGTORAD, b2_dynamicBody);
        _objects.append(polygon);

        //b2MassData mass;
        //mass.mass = 50;
        //mass.I = 10.0f;

        Object ball = createBall(b2Vec2(pixel_to_pt(30), pixel_to_pt(500)), pixel_to_pt(5), 0);
        ball.body->SetGravityScale(-1);
        ball.fixture->SetRestitution(1.1f);
        _objects.append(ball);

        Object wall = createWall(pixel_to_pt(20), pixel_to_pt(HEIGHT/2 - 50/2), pixel_to_pt(50), pixel_to_pt(50), 50*DEGTORAD, b2_dynamicBody);
        //wall.body->SetGravityScale(-1);
        _objects.append(wall);

        Object wall2 = createWall(pixel_to_pt(400), pixel_to_pt(HEIGHT/2 - 50/2), pixel_to_pt(50), pixel_to_pt(50), 50*M_PI/180, b2_dynamicBody);
        //wall2.body->SetMassData(&mass);
        _objects.append(wall2);

        create_borders();
        emit cnt_objects(_objects.count());
        is_busy = false;
    }
}
//--------------------------------------------------------------------------------
void World::create_scene_1(void)
{
    if(is_busy == false)
    {
        is_busy = true;
        Object ball = createBall(b2Vec2(pixel_to_pt(10.0f), pixel_to_pt(10.0f)), pixel_to_pt(10.0f), 0);
        _objects.append(ball);

        ball.body->SetLinearVelocity(b2Vec2(25.0f, 0.0f));
        ball.body->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), b2Vec2(1.0f, 1.0f), true);   //TODO

        _objects.append(createWall(50, 20, 1.0f, 12.0f, 90.0f*M_PI/180.0f, b2_dynamicBody));

        for(int n=250; n<WIDTH; n+=50)
        {
            _objects.append(createWall(pixel_to_pt(n), 0, 1.0f, 12.0f, 0, b2_dynamicBody));
        }

        create_borders();
        emit cnt_objects(_objects.count());
        is_busy = false;
    }
}
//--------------------------------------------------------------------------------
void World::create_scene_2(void)
{
    if(is_busy == false)
    {
        is_busy = true;
        for(int y=0; y<200; y+=10)
        {
            for(int x=0; x<200; x+=10)
            {
                Object ball = createBall(b2Vec2(pixel_to_pt(120.0f+x), pixel_to_pt(100.0f+y)), pixel_to_pt(3.0f), 0);
                _objects.append(ball);
            }
        }

        create_borders();
        emit cnt_objects(_objects.count());
        is_busy = false;
    }
}
//--------------------------------------------------------------------------------
void World::create_scene_3(void)
{
    if(is_busy == false)
    {
        is_busy = true;
        for(int n=0; n<360; n++)
        {
            Object ball = createBall(b2Vec2(pixel_to_pt(WIDTH / 2), pixel_to_pt(HEIGHT / 2)), pixel_to_pt(3.0f), 0);

            float angle = n;
            float x = qSin(angle * DEGTORAD) * 1000.0f;
            float y = qCos(angle * DEGTORAD) * 1000.0f;
            ball.body->SetLinearVelocity(b2Vec2(x, y));
            //ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f), b2Vec2(1.0f, 1.0f));

            _objects.append(ball);
        }

        create_borders();
        emit cnt_objects(_objects.count());
        is_busy = false;
    }
}
//--------------------------------------------------------------------------------
void World::clear(void)
{
    emit info(QString("delete %1 objects").arg(_objects.count()));
    delete_objects();
}
//--------------------------------------------------------------------------------
void World::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == _timerId)
    {
        if(!flag_block_insert_objects)
        {
            //insert_objects();
        }
        _world->Step(1.0f/60.0f, 8, 3);
        update();
    }
}
//--------------------------------------------------------------------------------
void World::updateText(void)
{

}
//--------------------------------------------------------------------------------
