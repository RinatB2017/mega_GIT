#ifndef COLLISIONRESOLVER_H
#define COLLISIONRESOLVER_H

class AIModel;
class Bot;

class CollisionResolver {
public:
    virtual ~CollisionResolver();

    virtual void resolve( const AIModel& model, Bot* b1, Bot* b2 );
};

#endif // COLLISIONRESOLVER_H
