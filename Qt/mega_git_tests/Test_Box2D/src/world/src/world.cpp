//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "world.hpp"
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
                       QColor color,
                       qreal angle,
                       b2BodyType type)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = type;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = qDegreesToRadians(angle);
//    bd.angle = angle;
    obj.body = _world->CreateBody(&bd);

    // shape
    b2PolygonShape shape;
    shape.SetAsBox(w / 2.0f, h / 2.0f);

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = WallObject;

    obj.color = color;

    _objects.append(obj);
}
//--------------------------------------------------------------------------------
void World::createPolygon(float x,
                          float y,
                          QColor color,
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

    obj.color = color;

    _objects.append(obj);
}
//--------------------------------------------------------------------------------
Object World::createBall(const b2Vec2& pos,
                         float radius,
                         QColor color,
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

    obj.color = color;

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

    painter->save();
    painter->setBrush(QBrush(obj.color));
    painter->drawEllipse(QPointF(pt_to_pixel(x),
                                 HEIGHT - pt_to_pixel(y)),
                         pt_to_pixel(r),
                         pt_to_pixel(r));
    painter->restore();
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
qreal World::pt_to_pixel(qreal value)
{
    return value * k_pt_to_pixel;
}
//--------------------------------------------------------------------------------
void World::drawWall(QPainter *painter,
                     const Object& obj)
{
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());

#if 0
    QRectF r(x-hx,
             y-hy,
             2*hx,
             2*hy);
    painter->save();
    painter->rotate(angle);
    painter->drawRect(r);
    painter->restore();
#endif

#if 0
    float x = obj.body->GetPosition().x;
    float y = obj.body->GetPosition().y;
    float angle = obj.body->GetAngle();
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;
//    QRectF r(x-hx, y-hy, 2*hx, 2*hy);
    QRectF r(pt_to_pixel(x-hx),
             HEIGHT - pt_to_pixel(y+hy),
             pt_to_pixel(2*hx),
             pt_to_pixel(2*hy));
    painter->save();
    painter->setBrush(QBrush(obj.color));
    painter->rotate(angle);
    painter->drawRect(r);
    painter->restore();
#endif

#if 1
    //FIXME нужно рисовать полигоном, похоже

    int pointCount = shape->GetVertexCount();
//    emit info(QString("GetVertexCount: %1").arg(shape->GetVertexCount()));

    float angle = obj.body->GetAngle();
    float pos_x = obj.body->GetPosition().x;
    float pos_y = obj.body->GetPosition().y;

    QPointF points[pointCount];

    for(int n=0; n<pointCount; n++)
    {
        float x = pt_to_pixel(shape->GetVertex(n).x);
        float y = pt_to_pixel(shape->GetVertex(n).y);
        points[n].setX(x);
        points[n].setY(y);
    }

    painter->save();
    painter->setBrush(QBrush(obj.color));
    painter->translate(pt_to_pixel(pos_x),
                       HEIGHT - pt_to_pixel(pos_y));
    painter->rotate(qRadiansToDegrees(angle));
    painter->drawPolygon(points, pointCount);
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
    painter->setBrush(QBrush(obj.color));
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
#if 1
    // низ
    createWall(0.0f,
               0.0f,
               20.0f,
               0.1f,
               Qt::red,
               0);
#endif
#if 1
    // верх
    createWall(0.0f,
               10.0f,
               20.0f,
               0.1f,
               Qt::red,
               0);
#endif
#if 1
    // левая стена
    createWall(0.0f,
               0.0f,
               0.1f,
               10.0f,
               Qt::blue,
               0.0f);
#endif
#if 1
    // правая стена
    createWall(20.0f,
               0.0f,
               0.1f,
               10.0f,
               Qt::green,
               0.0f);
#endif
#if 0
    //кирпич
    createWall(15.0f,
               7.0f,
               3.0f,
               1.0f,
               Qt::magenta,
               0);
#endif
#if 0
    //палка
    createWall(10.0f,
               5.0f,
               1.0f,
               0.1f,
               Qt::cyan,
               15,
               b2_dynamicBody);
#endif
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

        createPolygon(5,
                      5,
                      Qt::green,
                      vertices,
                      4,
                      0,
                      b2_dynamicBody);

        Object ball = createBall(b2Vec2(1.0f, 3.0f),
                                 0.15,
                                 Qt::red);
        ball.body->SetGravityScale(-1);
        ball.fixture->SetRestitution(1.1f);

        createWall(10.0f,
                   3.0f,
                   0.2f,
                   3.0f,
                   Qt::blue,
                   45.0f,
                   b2_dynamicBody);

        createWall(15.0f,
                   2.0f,
                   0.2f,
                   5.0f,
                   Qt::blue,
                   -45.0f,
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
                                 0.1,
                                 Qt::red);

        // линейная скорость
        //ball.body->SetLinearVelocity(b2Vec2(1.0f, 1.0f));
        // линейный импульс
        ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f),
                                      b2Vec2(1.0f, 1.0f),
                                      true);

        createWall(15,
                   4,
                   0.2,
                   2.0,
                   90.0,
                   b2_dynamicBody);

        int xx = 2;
        for(int n=0; n<10; n++)
        {
            createWall(xx,
                       0,
                       0.05f,
                       0.7f,
                       Qt::blue,
                       0,
                       b2_dynamicBody);
            xx = xx + 2;
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
        int pos_x = 5;
        int pos_y = 5;
        for(int y=0; y<200; y+=10)
        {
            for(int x=0; x<200; x+=5)
            {
                createBall(b2Vec2(pos_x+1.05f+x/25.0f,
                                  pos_y+1.05f+y/50.0f),
                           0.1f,
                           Qt::red);
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
            Object ball = createBall(b2Vec2(10.0, 5),
                                     0.05,
                                     Qt::red);

            qreal angle = n;
            qreal x = qSin(qDegreesToRadians(angle));
            qreal y = qCos(qDegreesToRadians(angle));
            ball.body->SetLinearVelocity(b2Vec2(x, y));
            ball.body->ApplyLinearImpulse(b2Vec2(x, y),
                                          b2Vec2(x, y),
                                          true);
        }

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
               0.1,
               Qt::red);

#if 0
    createWall(0,
               0.1,
               20,
               0.1,
               Qt::red,
               0);
#endif
}
//--------------------------------------------------------------------------------
void World::add_wall(qreal x,
                     qreal y,
                     qreal w,
                     qreal h,
                     qreal a)
{
    createWall(x,
               y,
               w,
               h,
               a,
               0);
}
//--------------------------------------------------------------------------------
void World::add_ball(qreal x,
                     qreal y,
                     qreal r)
{
    createBall(b2Vec2(x, y),
               r,
               Qt::red);
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
    Object ball = createBall(b2Vec2(x, y),
                             r,
                             Qt::red);
    // линейная скорость
    ball.body->SetLinearVelocity(b2Vec2(linear_velocity_x, linear_velocity_y));
    // линейный импульс
    ball.body->ApplyLinearImpulse(b2Vec2(impulse_x, impulse_y),
                                  b2Vec2(point_x, point_y),
                                  true);
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
