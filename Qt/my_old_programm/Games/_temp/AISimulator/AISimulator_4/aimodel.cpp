#include "aimodel.h"

#include <QSet>
#include <QQueue>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "botai.h"
#include "collisionresolver.h"

template< typename T >
uint qHash( const std::shared_ptr< T >& ptr ) {
    return qHash( ptr.get() );
}

uint qHash( const QPoint& point ) {
    return qHash( static_cast < qint64 >( point.x() ) << 32 | point.y() );
}

AIModel::AIModel( int width, int height ) {
    srand( time( 0 ) );
    reset( width, height );
}

int AIModel::getWidth() const {
    return m_field[ 0 ].size();
}

int AIModel::getHeight() const {
    return m_field.size();
}

bool AIModel::save( const QString& fileName ) const {
    QFile f( fileName );
    if( !f.open( QIODevice::WriteOnly ) ) {
        return false;
    }

    QTextStream str( &f );
    for( auto row : m_field ) {
        for( auto it = row.begin(); it != row.end(); ++it ) {
            str << *it;
            if( it + 1 != row.end() ) {
                str << ", ";
            }
        }
        str << "\n";
    }

    return true;
}

bool AIModel::load( const QString& fileName ) {
    QFile f( fileName );
    if( !f.open( QIODevice::ReadOnly ) ) {
        return false;
    }

    QTextStream str( &f );
    Matrix field;
    QStringList rowStr;
    while( ( rowStr = str.readLine().split( "," ) ).length() > 1 ) {
        if( !field.empty() && field[ 0 ].size() != static_cast< size_t >( rowStr.length() ) ) {
            return false;
        }
        Row row;
        for( auto rStr : rowStr ) {
            bool ok = false;
            row.push_back( rStr.toInt( &ok ) );
            if( !ok ) {
                return false;
            }
        }

        field.push_back( row );
    }

    m_bots.clear();
    m_field = field;
    resetDangerMap();

    return true;
}

void AIModel::setAI( const std::shared_ptr< BotAI >& ai, int botType ) {
    m_aiMap[ botType ] = ai;
}

void AIModel::setCollisionResolver( const std::shared_ptr< CollisionResolver >& resolver ) {
    m_resolver = resolver;
}

bool AIModel::addWall( int x, int y ) {
    if( getBlockType( x, y ) < 0 ) {
        return false;
    }

    m_field[ y ][ x ] = 1;

    for( auto b : m_bots ) {
        if( hasCollisions( *b ) ) {
            m_field[ y ][ x ] = 0;
            return false;
        }
    }

    return true;
}

bool AIModel::remove( int x, int y ) {
    if( getBlockType( x, y ) < 0 ) {
        return false;
    }

    m_field[ y ][ x ] = 0;
    kill( x, y );

    return true;
}

bool AIModel::kill( int x, int y ) {
    if( getBlockType( x, y ) < 0 ) {
        return false;
    }

    for( auto b : m_bots ) {
        if( b->getX() / BLOCK_SIZE == x && b->getY() / BLOCK_SIZE == y ) {
            b->die();
        }
    }

    return true;
}

bool AIModel::addBot( int x, int y, int type ) {
    return addBot( makeBot( x, y, type ) );
}

bool AIModel::addBot( const std::shared_ptr< Bot >& bot ) {
    if( hasCollisions( *bot ) ) {
        return false;
    }

    m_bots.insertMulti( bot->getType(), bot );
    return true;
}

void AIModel::reset( int width, int height ) {
    m_field.clear();
    for( int i = 0; i < height; ++i ) {
        m_field.push_back( Row( width ) );
    }

    resetDangerMap();

    killBots();
}

void AIModel::killBots() {
    m_bots.clear();
}

std::shared_ptr< Bot > AIModel::makeBot( int x, int y, int type ) {
    return std::make_shared< Bot >( x, y, type );
}

void AIModel::doStep() {
    refreshDangerMap();

    for( auto b : m_bots ) {
        if( m_aiMap.contains( b->getType() ) ) {
            m_aiMap[ b->getType() ]->doStep( *this, b.get() );
        }

        *b = doMove( *b );
    }

    QSet< QPair< std::shared_ptr< Bot >, std::shared_ptr< Bot > > > resolved;
    if( m_resolver ) {
        for( auto b1 : m_bots ) {
            for( auto b2 : m_bots ) {
                auto pair = qMakePair( b1, b2 );
                if( resolved.contains( pair ) ) {
                    continue;
                }
                if( b1->hasCollisions( *b2 ) ) {
                    m_resolver->resolve( *this, b1.get(), b2.get() );
                    resolved << pair;
                }
            }
        }

        auto it = m_bots.begin();
        while( it != m_bots.end() ) {
            if( it.value()->isDead() ) {
                it = m_bots.erase( it );
            } else {
                ++it;
            }
        }
    }
}

Bot AIModel::doMove( const Bot& bot ) const {
    Bot botCopy = bot;
    botCopy.move();

    while( hasCollisions( botCopy ) ) {
        botCopy.move( -1 );
    }

    return botCopy;
}

void AIModel::refreshDangerMap() {
    for( Row & r : m_dangerMap ) {
        std::fill( r.begin(), r.end(), 0 );
    }

    for( auto b : m_bots ) {
        if( b->getType() != 2 ) {
            continue;
        }

        int x = b->getX() / BLOCK_SIZE;
        int y = b->getY() / BLOCK_SIZE;
        markDangerArea( x, y, DEATH_RADIUS, DEATH );
        markDangerArea( x, y, EXTREME_DANGER_RADIUS, EXTREME );
        markDangerArea( x, y, WARNING_RADIUS, WARNING );
    }
}

void AIModel::markDangerArea( int x, int y, int radius, int score ) {
    for( int row = y - radius; row <= y + radius; ++row ) {
        for( int column = x - radius; column <= x + radius; ++column ) {
            if(
                column < 0 || m_field[ 0 ].size() <= static_cast< size_t >( column ) ||
                row < 0 || m_field.size() <= static_cast< size_t >( row )
            ) {
                continue;
            }
            m_dangerMap[ row ][ column ] += score;
        }
    }
}

void AIModel::resetDangerMap() {
    m_dangerMap.clear();
    for( size_t i = 0; i < m_field.size(); ++i ) {
        m_dangerMap.push_back( Row( m_field[ 0 ].size() ) );
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

QList< std::shared_ptr< Bot > > AIModel::getBots() const {
    return m_bots.values();
}

QList< std::shared_ptr< Bot > > AIModel::getBots( int botType ) const {
    return m_bots.values( botType );
}

bool AIModel::hasCollisions( const Bot& bot ) const {
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

std::vector< Bot::Direction > AIModel::findValidDirections( const Bot& bot ) const {
    std::vector< Bot::Direction > directions;
    for( int i = 0; i < Bot::DIRECTION_COUNT; ++i ) {
        Bot botCopy = bot;
        Bot::Direction d = Bot::Direction( i );
        botCopy.setDirection( d );
        botCopy.move( 1 );
        if( !hasCollisions( botCopy ) ) {
            directions.push_back( d );
        }
    }

    return directions;
}

std::vector< Bot::Direction > AIModel::findPath( const Bot& bot, int x, int y ) const {
    QSet< QPoint > visitedPoints;
    visitedPoints << QPoint( bot.getX(), bot.getY() );

    struct Node {
        Node( const Bot& b, std::shared_ptr< Node > p = nullptr ) :
            bot( b ), prev( p ) { }

        Bot bot;
        std::shared_ptr< Node > prev;
    };

    QQueue< std::shared_ptr< Node > > q;
    q << std::make_shared< Node >( bot );

    while( !q.isEmpty() ) {
        std::shared_ptr< Node > node = q.dequeue();
        for( int i = 0; i < Bot::DIRECTION_COUNT; ++i ) {
            Bot::Direction d = Bot::Direction( i );
            Bot bCopy = node->bot;
            bCopy.setDirection( d );
            bCopy.startMoving();
            bCopy = doMove( bCopy );
            QPoint p( bCopy.getX(), bCopy.getY() );
            if( p.x() == x && p.y() == y ) {
                std::vector< Bot::Direction > path;
                path.push_back( d );
                std::shared_ptr< Node > n = node;
                while( n->prev ) {
                    path.push_back( n->bot.getDirection() );
                    n = n->prev;
                }
                std::reverse( path.begin(), path.end() );
                return path;
            }
            if( !visitedPoints.contains( p ) ) {
                visitedPoints << p;
                q << std::make_shared< Node >( bCopy, node );
            }
        }
    }

    return std::vector< Bot::Direction >();
}

const Matrix& AIModel::getDangerMap() const {
    return m_dangerMap;
}
