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

    static const int DEFAULT_WIDTH = 15;
    static const int DEFAULT_HEIGHT = 15;

    static const int MIN_WIDTH = 5;
    static const int MIN_HEIGHT = 5;

    static const int MAX_WIDTH = 75;
    static const int MAX_HEIGHT = 30;

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
    AIModel( int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT );

    int getWidth() const;
    int getHeight() const;

    void resize( int width, int height );

    bool save( const QString& fileName ) const;
    bool load( const QString& fileName );

    void setAI( const std::shared_ptr< BotAI >& ai, int botType );

    void setCollisionResolver( const std::shared_ptr< CollisionResolver >& resolver );

    bool addWall( int x, int y );
    bool remove( int x, int y );
    bool kill( int x, int y );

    std::shared_ptr<Bot> addBot( int x, int y, int type = 2 );
    bool addBot( const std::shared_ptr< Bot >& bot );

    std::shared_ptr< Bot > findBot( int x, int y ) const;

    void reset( int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT );
    void killBots();
    void doStep();
    void cleanDeadBots();

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
    void resetDangerMap();

private:
    QHash< int, std::shared_ptr< BotAI > > m_aiMap;

    Matrix m_field;
    Matrix m_dangerMap;

    QHash< int, std::shared_ptr< Bot > > m_bots;

    std::shared_ptr< CollisionResolver > m_resolver;
};

#endif // AIMODEL_H
