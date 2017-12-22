#include "pathfinderai.h"

#include "aimodel.h"
#include "bot.h"

void PathFinderAI::doStep( const AIModel& model, Bot* bot ) {
    auto targets = model.getBots( 3 );
    if( targets.isEmpty() ) {
        bot->stopMoving();
        return;
    }

    auto target = targets.first();
    auto path = model.findPath( *bot, target->getX(), target->getY() );
    if( path.empty() ) {
        bot->stopMoving();
        return;
    }

    bot->startMoving();
    bot->setDirection( path.front() );
}
