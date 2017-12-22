#ifndef AIMODEL_H
#define AIMODEL_H

#include <vector>
#include <set>
#include <memory>

#include "bot.h"
#include "botai.h"

typedef std::vector< int > Row;
typedef std::vector< Row > Matrix;

class AIModel {
public:
    static const int BLOCK_SIZE = 12;
    static const int HALF_BLOCK_SIZE = BLOCK_SIZE / 2;
    static int blocksToPoints( int x ) { return x * BLOCK_SIZE; }

public:
    AIModel();

    int getWidth() const;
    int getHeight() const;

    void setAI( const std::shared_ptr< BotAI >& ai );

    void reset();
    void doStep();

    int getBlockType( int x, int y ) const;

    const std::set< std::unique_ptr< Bot > >& getBots() const;

    bool hasCollisions( const Bot& bot ) const;
    std::vector< Bot::Direction > findValidDirections( const Bot& bot ) const;

private:
    static std::unique_ptr< Bot > makeBot( int x, int y );

private:
    std::shared_ptr< BotAI > m_ai;

    Matrix m_field;

    std::set< std::unique_ptr< Bot > > m_bots;
};

#endif // AIMODEL_H
