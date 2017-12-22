#include "collisionresolver.h"

#include "aimodel.h"
#include "bot.h"

CollisionResolver::~CollisionResolver() {
}

void CollisionResolver::resolve( const AIModel&, Bot* b1, Bot* b2 ) {
    if( b1->getType() != b2->getType() ) {
        if( b1->getType() == 3 ) {
            b1->die();
        } else {
            b2->die();
        }
    }
}
