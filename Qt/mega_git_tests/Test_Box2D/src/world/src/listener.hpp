//--------------------------------------------------------------------------------
#ifndef LISTENER_HPP
#define LISTENER_HPP
//--------------------------------------------------------------------------------
#include <QVector>
#include <Box2D/Box2D.h>
//--------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------
class MyContactListener : public b2ContactListener
{
public:
    vector<b2ContactListener> _contacts;

    MyContactListener(b2World *world);
    virtual ~MyContactListener();

  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:
    b2World *world;
};
//--------------------------------------------------------------------------------
#endif
