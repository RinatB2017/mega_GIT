//--------------------------------------------------------------------------------
#ifndef LISTENER_HPP
#define LISTENER_HPP
//--------------------------------------------------------------------------------
#include <Box2D/Box2D.h>
//--------------------------------------------------------------------------------
class MyContactListener : public b2ContactListener
{
public:
    std::vector<b2ContactListener> _contacts;

    MyContactListener(b2World *world);
    ~MyContactListener();

  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:
    b2World *world;
};
//--------------------------------------------------------------------------------
#endif
