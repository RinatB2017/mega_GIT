#include "aimodel.h"

#include <QDebug>

AIModel::AIModel() {
    srand( time( 0 ) );
    reset();
}

int AIModel::getWidth() const {
    return m_field[ 0 ].size();
}

int AIModel::getHeight() const {
    return m_field.size();
}

void AIModel::setAI( BotAI* ai ) {
    m_ai.reset( ai );
}

void AIModel::reset() {
    m_field = {
        { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 1, 0, 1 },
        { 0, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 0, 1, 0, 0 },
        { 1, 0, 1, 0, 1, 1, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
    };

    m_bots.clear();
    m_bots.insert( makeBot( 10, 10 ) );
    m_bots.insert( makeBot( 10, 140 ) );
    m_bots.insert( makeBot( 110, 10 ) );
    m_bots.insert( makeBot( 110, 140 ) );
    m_bots.insert( makeBot( 54, 50 ) );
}

std::unique_ptr< Bot > AIModel::makeBot( int x, int y ) {
    return std::unique_ptr< Bot >( new Bot( x, y ) );
}

void AIModel::doStep() {
    for( const std::unique_ptr< Bot >& b : m_bots ) {
        if( m_ai ) {
            m_ai->doStep( *this, b.get() );
        }

        if( !b->isMoving() ) {
            continue;
        }

        b->move( b->getDirection(), b->getSpeed() );

        while( hasCollisions( *b ) ) {
            b->move( b->getDirection(), -1 );
        }
    }
}

int AIModel::getBlockType( int x, int y ) const {
    if(
        x < 0 || m_field[ 0 ].size() <= static_cast< size_t >( x ) ||
        y < 0 || m_field.size() <= static_cast< size_t >( y )
    ) {
        return -1;
    }

    return m_field[ y ][ x ];
}

const std::set< std::unique_ptr< Bot > >& AIModel::getBots() const {
    return m_bots;
}

bool AIModel::hasCollisions( const Bot& bot ) {
    const int halfBotSize = bot.getSize() / 2;

    if( bot.getX() - halfBotSize < 0 || bot.getY() - halfBotSize < 0 ) {
        return true;
    }

    const int left = ( bot.getX() - halfBotSize ) / BLOCK_SIZE;
    int right = ( bot.getX() + halfBotSize ) / BLOCK_SIZE;
    if( ( bot.getX() + halfBotSize ) % BLOCK_SIZE == 0 ) {
        right -= 1;
    }
    const int top = ( bot.getY() - halfBotSize ) / BLOCK_SIZE;
    int bottom = ( bot.getY() + halfBotSize ) / BLOCK_SIZE;
    if( ( bot.getY() + halfBotSize ) % BLOCK_SIZE == 0 ) {
        bottom -= 1;
    }

    for( int x = left; x <= right; ++x ) {
        for( int y = top; y <= bottom; ++y ) {
            if( getBlockType( x, y ) != 0 ) {
                return true;
            }
        }
    }

    return false;
}
