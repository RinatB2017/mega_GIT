#include "easyrandomai.h"
#include "bot.h"

#include <cstdlib>

void EasyRandomAI::doStep( const AIModel&, Bot* bot ) {
    bot->startMoving();
    bot->setDirection( Bot::Direction( rand() % Bot::DIRECTION_COUNT ) );
}
