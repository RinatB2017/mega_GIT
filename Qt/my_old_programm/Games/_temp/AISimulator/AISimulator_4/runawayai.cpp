#include "runawayai.h"

#include "aimodel.h"

#include <algorithm>

int getDanger( Bot::Direction dir, const Bot& b, const AIModel& model ) {
    const int halfBotSize = b.getSize() / 2;

    Bot bCopy = b;
    bCopy.setDirection( dir );
    bCopy = model.doMove( bCopy );
    int x = bCopy.getX();
    int y = bCopy.getY();
    switch( dir ) {
    case Bot::LEFT:
        x -= halfBotSize;
        break;
    case Bot::RIGHT:
        x += halfBotSize;
        if( x % AIModel::BLOCK_SIZE == 0 ) {
            x -= AIModel::BLOCK_SIZE;
        }
        break;
    case Bot::UP:
        y -= halfBotSize;
        break;
    case Bot::DOWN:
        y += halfBotSize;
        if( y % AIModel::BLOCK_SIZE == 0 ) {
            y -= AIModel::BLOCK_SIZE;
        }
        break;
    default:
        break;
    }

    return model.getDangerMap()[ y / AIModel::BLOCK_SIZE ][ x / AIModel::BLOCK_SIZE ];
}

void RunAwayAI::doStep( const AIModel& model, Bot* bot ) {
    bot->startMoving();

    auto directions = model.findValidDirections( *bot );

    if( !directions.empty() ) {
        auto it = directions.begin();
        int minDanger = getDanger( *it, *bot, model );
        ++it;
        for( ; it != directions.end(); ++it ) {
            int danger = getDanger( *it, *bot, model );
            if( danger < minDanger ) {
                minDanger = danger;
            }
        }

        it = directions.begin();
        while( it != directions.end() ) {
            if( minDanger < getDanger( *it, *bot, model ) ) {
                it = directions.erase( it );
            } else {
                ++it;
            }
        }
    }

    if( !directions.empty() && std::find( directions.begin(), directions.end(), bot->getDirection() ) == directions.end() ) {
        bot->setDirection( directions[ rand() % directions.size() ] );
    }
}
