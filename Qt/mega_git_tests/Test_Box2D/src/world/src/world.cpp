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
void World::createWall(Wall_param param)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = param.type;
    bd.position = b2Vec2(param.x + param.w/2.0f,
                         param.y + param.h/2.0f);
    bd.angle = qDegreesToRadians(param.angle);
//    bd.angle = angle;
    obj.body = _world->CreateBody(&bd);

    // shape
    b2PolygonShape shape;
    shape.SetAsBox(param.w / 2.0f,
                   param.h / 2.0f);

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = WallObject;

    UserData *data = new UserData;
    data->name="wall";
    data->index=0;
    obj.body->SetUserData(data);

    obj.color = param.color;
    _objects.append(obj);
}
//--------------------------------------------------------------------------------
void World::createPolygon(Polygon_param param,
                          b2Vec2 vertices[],
                          int count)
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
    bd.type = param.type;
    bd.position = b2Vec2(param.x,
                         param.y);
    bd.angle = param.angle * b2_pi;
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
    data->index=0;
    obj.body->SetUserData(data);

    obj.color = param.color;
    _objects.append(obj);
}
//--------------------------------------------------------------------------------
Object World::createBall(Ball_param param)
{
    Object obj;

    // body
    b2BodyDef bd;
    bd.type = param.type;
    bd.position = param.pos;
    obj.body = _world->CreateBody(&bd);

    // shape
    b2CircleShape shape;
    shape.m_radius = param.radius;

    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    obj.fixture = obj.body->CreateFixture(&fd);
    obj.type = BallObject;

    UserData *data = new UserData;
    data->name="ball";
    data->index=param.index;
    obj.body->SetUserData(data);

    obj.color = param.color;
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
qreal World::pt_to_pixel(qreal value)
{
    return value * k_pt_to_pixel;
}
//--------------------------------------------------------------------------------
void World::drawWall(QPainter *painter,
                     const Object& obj)
{
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(obj.fixture->GetShape());

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
    Wall_param param;

    // низ
    param.x = 0.0f;
    param.y = 0.0f,
    param.w = scene_w;
    param.h = wall_h;
    param.color = Qt::red;
    param.angle = 0;
    param.index = 0;
    param.type = b2_staticBody;

    createWall(param);

    // верх
    param.x = 0.0f;
    param.y = scene_h,
    param.w = scene_w;
    param.h = wall_h;
    param.color = Qt::red;
    param.angle = 0;
    param.index = 0;
    param.type = b2_staticBody;

    createWall(param);

    // левая стена
    param.x = 0.0f;
    param.y = 0.0f,
    param.w = wall_h;
    param.h = scene_h;
    param.color = Qt::blue;
    param.angle = 0;
    param.index = 0;
    param.type = b2_staticBody;

    createWall(param);

    // правая стена
    param.x = scene_w;
    param.y = 0.0f,
    param.w = wall_h;
    param.h = scene_h;
    param.color = Qt::red;
    param.angle = 0;
    param.index = 0;
    param.type = b2_staticBody;

    createWall(param);

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

        Polygon_param p_param;

        p_param.x = 5.0f;
        p_param.y = 5.0f;
        p_param.color = Qt::green;
        p_param.type = b2_dynamicBody;
        p_param.angle = 0;
        p_param.index = 0;

        int cnt = 4;

        createPolygon(p_param, vertices, cnt);

        Ball_param b_param;

        b_param.pos = b2Vec2(1.0f, 3.0f);
        b_param.radius = 0.15f;
        b_param.color = Qt::red;
        b_param.type = b2_dynamicBody;
        b_param.index = 0;

        Object ball = createBall(b_param);
        ball.body->SetGravityScale(-1);
        ball.fixture->SetRestitution(1.1f);

        Wall_param param;

        param.x = 10.0f;
        param.y = 3.0f,
        param.w = 0.2f;
        param.h = 3.0f;
        param.color = Qt::blue;
        param.angle = 45.0f;
        param.index = 0;
        param.type = b2_dynamicBody;

        createWall(param);

        param.x = 15.0f;
        param.y = 2.0f,
        param.w = 0.2f;
        param.h = 5.0f;
        param.color = Qt::blue;
        param.angle = -45.0f;
        param.index = 0;
        param.type = b2_dynamicBody;

        createWall(param);

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

        Ball_param b_param;

        b_param.pos = b2Vec2(1.0f, 1.0f);
        b_param.radius = 0.1f;
        b_param.color = Qt::red;
        b_param.type = b2_dynamicBody;
        b_param.index = 0;

        Object ball = createBall(b_param);

        // линейная скорость
        //ball.body->SetLinearVelocity(b2Vec2(1.0f, 1.0f));
        // линейный импульс
        ball.body->ApplyLinearImpulse(b2Vec2(1.0f, 1.0f),
                                      b2Vec2(1.0f, 1.0f),
                                      true);

        Wall_param param;

        param.x = 15.0f;
        param.y = 4.0f,
        param.w = 0.2f;
        param.h = 2.0f;
        param.color = Qt::red;
        param.angle = 90.0f;
        param.index = 0;
        param.type = b2_dynamicBody;

        createWall(param);

        int xx = 2;

        param.y = 0.0f,
        param.w = 0.05f;
        param.h = 0.7f;
        param.color = Qt::blue;
        param.angle = 0.0f;
        param.index = 0;
        param.type = b2_dynamicBody;

        for(int n=0; n<10; n++)
        {
            param.x = xx;
            createWall(param);
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
        Ball_param b_param;
        for(int y=0; y<200; y+=10)
        {
            for(int x=0; x<200; x+=5)
            {
                b_param.pos = b2Vec2(pos_x+1.05f+x/25.0f,
                                     pos_y+1.05f+y/50.0f);
                b_param.radius = 0.1f;
                b_param.color = Qt::red;
                b_param.type = b2_dynamicBody;
                b_param.index = 0;

                createBall(b_param);
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
        Ball_param b_param;
        for(int n=0; n<360; n++)
        {
            b_param.pos = b2Vec2(10.0f,
                                 5.0f);
            b_param.radius = 0.05f;
            b_param.color = Qt::red;
            b_param.type = b2_dynamicBody;
            b_param.index = 0;

            Object ball = createBall(b_param);

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

    Ball_param b_param;

    b_param.pos = b2Vec2(1.0f + qrand() % 10,
                         5.0f);
    b_param.radius = 0.1f;
    b_param.color = Qt::red;
    b_param.type = b2_dynamicBody;
    b_param.index = 0;

    createBall(b_param);

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
void World::add_wall(Wall_param param)
{
    createWall(param);
}
//--------------------------------------------------------------------------------
void World::add_ball(Ball_param param)
{
    Ball_param b_param;

    b_param.pos = param.pos;
    b_param.radius = param.radius;
    b_param.color = param.color;
    b_param.type = param.type;
    b_param.index = param.index;

    createBall(b_param);
}
//--------------------------------------------------------------------------------
void World::add_bullet(Ball_param param,
                       qreal linear_velocity_x,
                       qreal linear_velocity_y,
                       qreal impulse_x,
                       qreal impulse_y,
                       qreal point_x,
                       qreal point_y)
{
    Ball_param b_param;

    b_param.pos = param.pos;
    b_param.radius = param.radius;
    b_param.color = param.color;
    b_param.type = param.type;
    b_param.index = param.index;

    Object ball = createBall(b_param);
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
