#include "singlememrandomai.h"

#include "aimodel.h"
#include "bot.h"

#include <cstdlib>
#include <algorithm>

void SingleMemRandomAI::doStep( const AIModel& model, Bot* bot ) {
    bot->startMoving();
    auto directions = model.findValidDirections( *bot );
    if( std::find( directions.begin(), directions.end(), bot->getDirection() ) == directions.end() ) {
        bot->setDirection( directions[ rand() % directions.size() ] );
    }
}
