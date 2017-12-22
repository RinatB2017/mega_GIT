#include "smartrandomai.h"

#include "aimodel.h"
#include "bot.h"

#include <cstdlib>

void SmartRandomAI::doStep( const AIModel& model, Bot* bot ) {
    auto directions = model.findValidDirections( *bot );
    bot->startMoving();
    bot->setDirection( directions[ rand() % directions.size() ] );
}
