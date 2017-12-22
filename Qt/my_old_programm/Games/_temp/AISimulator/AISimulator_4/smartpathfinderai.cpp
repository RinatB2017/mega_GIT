#include "smartpathfinderai.h"

#include "aimodel.h"

void SmartPathFinderAI::doStep( const AIModel& model, Bot* bot ) {
    auto targets = model.getBots( 3 );
    if( targets.isEmpty() ) {
        bot->stopMoving();
        return;
    }

    auto target = *std::min_element(
                      targets.begin(),
                      targets.end(),
                      [ &bot ]( const std::shared_ptr< Bot >& b1, const std::shared_ptr< Bot >& b2 ) {
                          return bot->squareDistanceTo( *b1 ) < bot->squareDistanceTo( *b2 );
                      }
                  );
    auto path = model.findPath( *bot, target->getX(), target->getY() );
    if( path.empty() ) {
        bot->stopMoving();
        return;
    }

    bot->startMoving();
    bot->setDirection( path.front() );
}
