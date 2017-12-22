#include "smartrandomai.h"

#include "aimodel.h"
#include "bot.h"

#include <cstdlib>

void SmartRandomAI::doStep( const AIModel& model, Bot* bot ) {
    auto directions = model.findValidDirections( *bot );
    bot->startMoving();

    if( !directions.empty() ) {
        bot->setDirection( directions[ rand() % directions.size() ] );
    }
}
