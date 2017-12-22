#include "accumulatingrandomai.h"

#include "aimodel.h"
#include "bot.h"

#include <cstdlib>

static const int MAX_CHANCE = 100;
static const int MIN_CHANCE = 5;

void AccumulatingRandomAI::doStep( const AIModel& model, Bot* bot ) {
    bot->startMoving();

    if( !m_memory.contains( bot->getID() ) ) {
        m_memory[ bot->getID() ].resize( Bot::DIRECTION_COUNT, MAX_CHANCE / Bot::DIRECTION_COUNT );
    }

    auto directions = model.findValidDirections( *bot );
    if( directions.empty() ) {
        return;
    }

    std::vector< double > chances( Bot::DIRECTION_COUNT, 0 );
    for( auto d : directions ) {
        chances[ d ] = m_memory[ bot->getID() ][ d ];
    }

    const int p = rand() % std::accumulate( chances.begin(), chances.end(), 0 );
    double sum = 0;
    size_t i = 0;
    for( ; i < chances.size(); ++i ) {
        sum += chances[ i ];
        if( p < sum ) {
            break;
        }
    }
    const Bot::Direction direction = Bot::Direction( i );
    bot->setDirection( direction );

    for( int i = 0; i < Bot::DIRECTION_COUNT; ++i ) {
        if( i == direction ) {
            continue;
        }
        if( MIN_CHANCE < m_memory[ bot->getID() ][ i ] ) {
            m_memory[ bot->getID() ][ i ]--;
            m_memory[ bot->getID() ][ direction ]++;
        }
    }
}

void AccumulatingRandomAI::reset() {
    m_memory.clear();
}
