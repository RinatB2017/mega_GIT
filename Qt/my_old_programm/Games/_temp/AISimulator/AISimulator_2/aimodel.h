#ifndef AIMODEL_H
#define AIMODEL_H

#include <vector>
#include <set>
#include <memory>

#include <QHash>

#include "bot.h"

class BotAI;
class CollisionResolver;

typedef std::vector< int > Row;
typedef std::vector< Row > Matrix;

class AIModel {
public:
    static const int BLOCK_SIZE = 12;
    static const int HALF_BLOCK_SIZE = BLOCK_SIZE / 2;
    static int blocksToPoints( int x ) {
        return x * BLOCK_SIZE;
    }

public:
    AIModel();

    int getWidth() const;
    int getHeight() const;

    void setAI( const std::shared_ptr< BotAI >& ai, int botType );

    void setCollisionResolver( const std::shared_ptr< CollisionResolver >& resolver );

    bool addBot( int x, int y, int type = 2 );
    bool addBot( const std::shared_ptr< Bot >& bot );

    void reset();
    void doStep();

    int getBlockType( int x, int y ) const;

    QList< std::shared_ptr< Bot > > getBots() const;
    QList< std::shared_ptr< Bot > > getBots( int botType ) const;

    bool hasCollisions( const Bot& bot ) const;
    std::vector< Bot::Direction > findValidDirections( const Bot& bot ) const;

    std::vector< Bot::Direction > findPath( const Bot& bot, int x, int y ) const;

private:
    static std::shared_ptr< Bot > makeBot( int x, int y , int type = 2 );

    Bot doMove( const Bot& bot ) const;

private:
    QHash< int, std::shared_ptr< BotAI > > m_aiMap;

    Matrix m_field;

    QHash< int, std::shared_ptr< Bot > > m_bots;

    std::shared_ptr< CollisionResolver > m_resolver;
};

#endif // AIMODEL_H
