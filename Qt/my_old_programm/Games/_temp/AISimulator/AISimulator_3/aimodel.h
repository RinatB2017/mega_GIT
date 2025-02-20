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

    static const int DEATH_RADIUS = 0;
    static const int EXTREME_DANGER_RADIUS = 1;
    static const int WARNING_RADIUS = 2;

    enum DangerLevel {
        WARNING = 10,
        CRITICAL = 30,
        EXTREME = 50,
        DEATH = 100
    };

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

    Bot doMove( const Bot& bot ) const;

    int getBlockType( int x, int y ) const;

    QList< std::shared_ptr< Bot > > getBots() const;
    QList< std::shared_ptr< Bot > > getBots( int botType ) const;

    bool hasCollisions( const Bot& bot ) const;
    std::vector< Bot::Direction > findValidDirections( const Bot& bot ) const;

    std::vector< Bot::Direction > findPath( const Bot& bot, int x, int y ) const;

    const Matrix& getDangerMap() const;

private:
    static std::shared_ptr< Bot > makeBot( int x, int y, int type = 2 );

    void refreshDangerMap();
    void markDangerArea( int x, int y, int radius, int score );

private:
    QHash< int, std::shared_ptr< BotAI > > m_aiMap;

    Matrix m_field;
    Matrix m_dangerMap;

    QHash< int, std::shared_ptr< Bot > > m_bots;

    std::shared_ptr< CollisionResolver > m_resolver;
};

#endif // AIMODEL_H
