//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
#include "listener.hpp"
#include "world.hpp"
//--------------------------------------------------------------------------------
MyContactListener::MyContactListener(b2World *world) : _contacts()
{
    this->world = world;
}
//--------------------------------------------------------------------------------
MyContactListener::~MyContactListener()
{

}
//--------------------------------------------------------------------------------
void MyContactListener::BeginContact(b2Contact* contact)
{
    UserData *actor1 = (UserData *)contact->GetFixtureA()->GetBody()->GetUserData();
    UserData *actor2 = (UserData *)contact->GetFixtureB()->GetBody()->GetUserData();

    if(actor1) qDebug() << actor1->name;
    if(actor2) qDebug() << actor2->name;
    qDebug() << "-----";

    //qDebug() << "BeginContact" << contact << actor1 << actor2;
#if 0
    if(actor1->name.compare("ball1") == 0)
        qDebug() << "BeginContact: ball 1 actor1";
    if(actor2->name.compare(QString("static ball")) == 0)
        qDebug() << "BeginContact: static ball actor2";
#endif
}
//--------------------------------------------------------------------------------
void MyContactListener::EndContact(b2Contact* contact)
{
#if 0
    void *actor1 = contact->GetFixtureA()->GetBody()->GetUserData();
    void *actor2 = contact->GetFixtureB()->GetBody()->GetUserData();

    //qDebug() << "EndContact" << contact;
    if(actor1 == "ball") qDebug() << "EndContact: actor1";
    if(actor2 == "ball") qDebug() << "EndContact: actor2";
#else
    Q_UNUSED(contact)
#endif
}
//--------------------------------------------------------------------------------
void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //qDebug() << "PreSolve" << contact << oldManifold;
    Q_UNUSED(contact)
    Q_UNUSED(oldManifold)
}
//--------------------------------------------------------------------------------
void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
#if 0
    b2Body *body;

    if(contact->GetFixtureA())
        body = contact->GetFixtureA()->GetBody();
    if(contact->GetFixtureB())
        body = contact->GetFixtureB()->GetBody();

    if(body)
    {
        body->SetActive(false);
        world->DestroyBody(body);
        body = 0;
    }
#else
    Q_UNUSED(contact)
    Q_UNUSED(impulse)
#endif
    //qDebug() << "PostSolve" << contact << impulse;
}
//--------------------------------------------------------------------------------
