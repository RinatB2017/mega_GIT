//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "world.hpp"
//--------------------------------------------------------------------------------
//#define NEW_CALC    1
//--------------------------------------------------------------------------------
World::World(QWidget *parent) :
    MyWidget(parent)
{
    is_busy = false;

    cnt = 0;

    timeStep = 1.0f / 60.0f;

    b2Vec2 gravity(0.0f, -9.8f);
    _world = new b2World(gravity);

#if 0
    //TODO компенсация координат при отрисовке
    _transform.scale(10.0, -10.0);
    _transform.translate(0.0, -pixel_to_pt(HEIGHT));
#endif
}
//--------------------------------------------------------------------------------
World::~World()
{
    if(_world)
    {
        delete _world;
    }
    emit debug("~World()");
}
//--------------------------------------------------------------------------------
void World::createWall(qreal x,
                       qreal y,
                       qreal w,
                       qreal h,
                       qreal angle,
                       b2BodyType type)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = type;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = qDegreesToRadians(angle);
    obj.body = _world->CreateBody(&bd);

    // shape
    b2PolygonShape shape;
    shape.SetAsBox(w / 2.0, h / 4.0);   //TODO надо разобраться

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = WallObject;

    _objects.append(obj);
}
//--------------------------------------------------------------------------------
void World::createPolygon(float x,
                          float y,
                          b2Vec2  vertices[],
                          int   count,
                          float angle,
                          b2BodyType type)
{
    Object obj;
    //const int count = 4;
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

    _objects.append(obj);
}
//--------------------------------------------------------------------------------
Object World::createBall(const b2Vec2& pos,
                         float radius,
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

    _objects.append(obj);

    return obj;
}
//--------------------------------------------------------------------------------
void World::drawEllipse(QPainter *painter,
                        const Object& obj)
{
    float x = obj.body->GetPosition().x;
    float y = obj.body->GetPosition().y;
    float r = obj.fixture->GetShape()->m_radius;

    painter->drawEllipse(QPointF(pt_to_pixel(x),
                                 HEIGHT - pt_to_pixel(y)),
                         pt_to_pixel(r),
                         pt_to_pixel(r));
}
//--------------------------------------------------------------------------------
void World::drawEllipse_B(QPainter *painter,
                          b2Body *body)
{
    float x = body->GetPosition().x;
    float y = body->GetPosition().y;
    float r = 1.0f; //body->GetShape()->m_radius;

    painter->drawEllipse(QPointF(pt_to_pixel(x),
                                 HEIGHT - pt_to_pixel(y)),
                         r,
                         r);
}
//--------------------------------------------------------------------------------
void World::drawWall(QPainter *painter,
                     const Object& obj)
{
    float x = obj.body->GetPosition().x;
    float y = obj.body->GetPosition().y;
    float angle = obj.body->GetAngle();

    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());
    float hx = shape->GetVertex(1).x;
    float hy = shape->GetVertex(2).y;

#ifdef NEW_CALC
    QRectF r(x-hx,
             y-hy,
             2*hx,
             2*hy);
    painter->save();
    painter->rotate(angle);
    painter->drawRect(r);
    painter->restore();
#else
    QRectF r(pt_to_pixel(x-hx),
             HEIGHT - pt_to_pixel(y-hy),
             pt_to_pixel(2*hx),
             pt_to_pixel(2*hy));
    painter->save();
    painter->rotate(angle);
    painter->drawRect(r);
    painter->restore();
#endif
}
//--------------------------------------------------------------------------------
void World::drawPolygon(QPainter *painter,
                        const Object& obj)
{
    float x = obj.body->GetPosition().x;
    float y = obj.body->GetPosition().y;
    float angle = obj.body->GetAngle();

    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());
    int pointCount = shape->GetVertexCount();
    QPointF points[pointCount];

    float hx = 0.0f;
    float hy = 0.0f;
    for(int n=0; n<pointCount; ++n)
    {
        float lx = shape->GetVertex(n).x;
        float ly = shape->GetVertex(n).y;
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
        Object obj = _objects.takeAt(0);
        _world->DestroyBody(obj.body);
    }
}
//--------------------------------------------------------------------------------
void World::test(void)
{
    emit info(QString("cnt obj %1").arg(_objects.count()));

    b2Body *bl = _world->GetBodyList();
    while(bl != NULL)
    {
        emit info(QString("pos: %1 %2")
                  .arg(bl->GetPosition().x)
                  .arg(bl->GetPosition().y));
        bl = bl->GetNext();
    }
}
//--------------------------------------------------------------------------------
void World::create_borders(void)
{
    // низ
    createWall(0.0f,
               0.0f,
               20.0f,
               0.1f);
    // верх
    createWall(0.0f,
               10.0f,
               20.0f,
               0.1f);
    // левая стена
    createWall(0.0f,
               0.0f,
               0.1f,
               40.0f);
    // правая стена
    createWall(20.0f,
               0.0f,
               0.1f,
               40.0f);
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

        createPolygon(10,
                      10,
                      vertices,
                      4,
                      0,
                      b2_dynamicBody);

        Object ball = createBall(b2Vec2(pixel_to_pt(30), pixel_to_pt(500)),
                                 0.15);
        ball.body->SetGravityScale(-1);
        ball.fixture->SetRestitution(1.1f);

        createWall(20,
                   20,
                   1,
                   3,
                   45,
                   b2_dynamicBody);

        createWall(40,
                   20,
                   1,
                   5,
                   50,
                   b2_dynamicBody);

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

        Object ball = createBall(b2Vec2(1.0, 1.0),
                                 0.1);

        // линейная скорость
        //ball.body->SetLinearVelocity(b2Vec2(1.0f, 1.0f));
        // линейный импульс
        ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f),
                                      b2Vec2(1.0f, 1.0f),
                                      true);

        createWall(15,
                   5,
                   0.2,
                   2.0,
                   90.0,
                   b2_dynamicBody);

        /*
        for(int n=250; n<WIDTH; n+=50)
        {
            createWall(n / 10,
                       0,
                       1.0f,
                       12.0f,
                       0,
                       b2_dynamicBody);
        }
*/

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
                createBall(b2Vec2(pixel_to_pt(120.0f+x), pixel_to_pt(100.0f+y)),
                           pixel_to_pt(3.0f));
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
#if 1
        for(int n=0; n<360; n++)
        {
            Object ball = createBall(b2Vec2(10.0, 5),
                                     0.05);

            qreal angle = n;
            qreal x = qSin(qDegreesToRadians(angle));
            qreal y = qCos(qDegreesToRadians(angle));
            ball.body->SetLinearVelocity(b2Vec2(x, y));
            ball.body->ApplyLinearImpulse(b2Vec2(x, y),
                                          b2Vec2(x, y),
                                          true);
        }
#endif

        create_borders();
        emit cnt_objects(_objects.count());
        is_busy = false;
    }
}
//--------------------------------------------------------------------------------
void World::create_scene_4(void)
{
    /*
    экран 1200 * 600
    объект на высоте 5 метров и вправо на 10 метров
    размер объекта 10 сантиметров
    ширина земли 20 метров
    */

    createBall(b2Vec2(1 + qrand() % 10, 5),
               0.1);

    createWall(0, 0.1, 20, 0.1);
}
//--------------------------------------------------------------------------------
void World::add_wall(qreal x,
                     qreal y,
                     qreal w,
                     qreal h,
                     qreal a)
{
    createWall(x, y, w, h, a);
}
//--------------------------------------------------------------------------------
void World::add_ball(qreal x,
                     qreal y,
                     qreal r)
{
    createBall(b2Vec2(x, y), r);
}
//--------------------------------------------------------------------------------
void World::add_bullet(qreal x,
                       qreal y,
                       qreal r,
                       qreal linear_velocity_x,
                       qreal linear_velocity_y,
                       qreal impulse_x,
                       qreal impulse_y,
                       qreal point_x,
                       qreal point_y)
{
    Object ball = createBall(b2Vec2(x, y), r);
    // линейная скорость
    ball.body->SetLinearVelocity(b2Vec2(linear_velocity_x, linear_velocity_y));
    // линейный импульс
    ball.body->ApplyLinearImpulse(b2Vec2(impulse_x, impulse_y),
                                  b2Vec2(point_x, point_y),
                                  true);
}
//--------------------------------------------------------------------------------
qreal World::pixel_to_pt_get(void)
{
    return k_pixel_to_pt;
}
//--------------------------------------------------------------------------------
void World::pixel_to_pt_set(qreal value)
{
    k_pixel_to_pt = value;
}
//--------------------------------------------------------------------------------
qreal World::pt_to_pixel_get(void)
{
    return k_pt_to_pixel;
}
//--------------------------------------------------------------------------------
void World::pt_to_pixel_set(qreal value)
{
    k_pt_to_pixel = value;
}
//--------------------------------------------------------------------------------
qreal World::pixel_to_pt(qreal value)
{
    return (value / k_pixel_to_pt);
}
//--------------------------------------------------------------------------------
qreal World::pt_to_pixel(qreal value)
{
    return value * k_pt_to_pixel;
}
//--------------------------------------------------------------------------------
void World::w_clear(void)
{
    emit info(QString("delete %1 objects").arg(_objects.count()));
    delete_objects();
}
//--------------------------------------------------------------------------------
void World::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == _timerId)
    {
        Q_ASSERT(_world);
        _world->Step(timeStep, velocityIterations, positionIterations);
        update();
    }
}
//--------------------------------------------------------------------------------
void World::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
#if 0
    painter.setTransform(_transform);
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#   error (Need Qt5)
#else
    QPen pen;
    pen.setWidthF(0.07);
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
void World::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool World::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void World::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void World::save_setting(void)
{

}
//--------------------------------------------------------------------------------
