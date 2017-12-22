#include "smartsinglememrandomai.h"

#include "aimodel.h"
#include "bot.h"

#include <cstdlib>
#include <algorithm>

#include <QVector>

void SmartSingleMemRandomAI::doStep( const AIModel& model, Bot* bot ) {
    bot->startMoving();

    auto directions = model.findValidDirections( *bot );

    QVector< Bot::Direction > openedDirections;
    for( Bot::Direction d : directions ) {
        if( !m_memory[ bot->getID() ].contains( d ) ) {
            openedDirections << d;
        }
    }

    if( std::find( directions.begin(), directions.end(), bot->getDirection() ) != directions.end() ) {
        openedDirections << bot->getDirection();
    } else {
        openedDirections.clear();
        for( Bot::Direction d : directions ) {
            openedDirections << d;
        }
    }

    bot->setDirection( openedDirections[ rand() % openedDirections.size() ] );
    m_memory[ bot->getID() ].clear();
    m_memory[ bot->getID() ] << Bot::reverseDirection( bot->getDirection() );
    for( Bot::Direction d : directions ) {
        m_memory[ bot->getID() ] << d;
    }
}

void SmartSingleMemRandomAI::reset() {
    m_memory.clear();
}
