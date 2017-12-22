#include "tetrismodel.h"

#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <numeric>

int blocksToPoints( int x )
{
    return x * BLOCK_SIZE;
}

// ********************************************************************************
TetrisModel::TetrisModel( int widthBlocks, int heightBlocks ) :
    m_widthBlocks( widthBlocks ),
    m_heightBlocks( heightBlocks ),
    m_dropEnabled( false )
{
    if( m_widthBlocks <= 0 || m_heightBlocks <= 0 )
    {
        throw std::invalid_argument( "Width and height of the tetris field must be > 0" );
    }

    srand( time( 0 ) );
    resetGame();
}

const TetrisItem& TetrisModel::getItem() const
{
    return m_activeItem;
}

void TetrisModel::doStep()
{
    if( m_activeItem.isNull() )
    {
        m_itemBottomTouchCounter = 0;
        m_activeItem = TetrisItem::generateRandom();
        int xPoints = blocksToPoints( getWidthBlocks() / 2 );
        if( m_activeItem.getSizeBlocks() % 2 == 1 )
        {
            // Если элемент состоит из нечетного числа блоков, то выравниваем его по сетке:
            xPoints += HALF_BLOCK_SIZE;
        }
        m_activeItem.setPosition( xPoints, 0 );
        if( hasCollisions( m_activeItem ) )
        {
            m_gameOver = true;
        }
    }

    if( isGameOver() )
    {
        return;
    }

    int speed = m_dropEnabled ? MAX_SPEED : m_speed;
    TetrisItem item = m_activeItem;
    item.setPosition( m_activeItem.getXPoints(), m_activeItem.getYPoints() + speed );

    if( !hasCollisions( item ) )
    {
        m_activeItem = item;
        m_itemBottomTouchCounter = 0;
    }
    else
    {
        while( hasCollisions( item ) )
        {
            item.setPosition( item.getXPoints(), item.getYPoints() - 1 );
        }

        if( MAX_TOUCH_COUNT < m_itemBottomTouchCounter )
        {
            m_activeItem = TetrisItem();
            for( int xBlocks = 0; xBlocks < item.getSizeBlocks(); ++xBlocks )
            {
                for( int yBlocks = 0; yBlocks < item.getSizeBlocks(); ++yBlocks )
                {
                    int blockType = item.getBlockType( xBlocks, yBlocks );
                    if( blockType != 0 )
                    {
                        int xPoints = item.getBlockXPoints( xBlocks );
                        int yPoints = item.getBlockYPoints( yBlocks );
                        m_fieldMatrix[ yPoints / BLOCK_SIZE ][  xPoints / BLOCK_SIZE ] = blockType;
                    }
                }
            }
            clean();
        }
        else
        {
            m_activeItem = item;
            ++m_itemBottomTouchCounter;
        }
    }
}

bool TetrisModel::isGameOver() const
{
    return m_gameOver;
}

int TetrisModel::getScore() const
{
    return m_score;
}

void TetrisModel::resetGame()
{
    m_activeItem = TetrisItem();

    m_fieldMatrix.clear();
    m_fieldMatrix.resize( getHeightBlocks() );
    for( std::vector< int >& row : m_fieldMatrix )
    {
        row.resize( getWidthBlocks() );
    }

    m_speed = MIN_SPEED;
    m_score = 0;
    m_gameOver = false;
}

void TetrisModel::rotateItem()
{
    TetrisItem item = m_activeItem;
    item.rotate();
    if( !hasCollisions( item ) )
    {
        m_activeItem = item;
        return;
    }

    item.setPosition( item.getXPoints() + BLOCK_SIZE, item.getYPoints() );
    if( !hasCollisions( item ) )
    {
        m_activeItem = item;
        return;
    }

    item.setPosition( item.getXPoints() - blocksToPoints( 2 ), item.getYPoints() );
    if( !hasCollisions( item ) )
    {
        m_activeItem = item;
        return;
    }
}

void TetrisModel::moveItemLeft()
{
    moveItemX( -BLOCK_SIZE );
}

void TetrisModel::moveItemRight()
{
    moveItemX( BLOCK_SIZE );
}

void TetrisModel::moveItemX( int offsetPoints )
{
    TetrisItem item = m_activeItem;
    item.setPosition( item.getXPoints() + offsetPoints, item.getYPoints() );
    if( !hasCollisions( item ) )
    {
        m_activeItem = item;
    }
}

void TetrisModel::startDrop()
{
    m_dropEnabled = true;
}

void TetrisModel::stopDrop()
{
    m_dropEnabled = false;
}

int TetrisModel::getWidthBlocks() const
{
    return m_widthBlocks;
}

int TetrisModel::getHeightBlocks() const
{
    return m_heightBlocks;
}

int TetrisModel::getWidthPoints() const
{
    return blocksToPoints( getWidthBlocks() );
}

int TetrisModel::getHeightPoints() const
{
    return blocksToPoints( getHeightBlocks() );
}

int TetrisModel::getBlockType( int xBlocks, int yBlocks ) const
{
    static const int BORDER_BLOCK_TYPE = -1;

    if( xBlocks < 0 || getWidthBlocks() <= xBlocks || getHeightBlocks() <= yBlocks )
    {
        return BORDER_BLOCK_TYPE;
    }
    else if( yBlocks < 0 )
    {
        return 0;
    }

    return m_fieldMatrix[ yBlocks ][ xBlocks ];
}

bool TetrisModel::hasCollisions( const TetrisItem& item ) const
{
    for( int xBlocks = 0; xBlocks < item.getSizeBlocks(); ++xBlocks )
    {
        for( int yBlocks = 0; yBlocks < item.getSizeBlocks(); ++yBlocks )
        {
            if(item.getBlockType( xBlocks, yBlocks ) > 0 && hasCollisions( item.getBlockXPoints( xBlocks ), item.getBlockYPoints( yBlocks ) ) )
            {
                return true;
            }
        }
    }

    return false;
}

bool TetrisModel::hasCollisions( int xPoints, int yPoints ) const
{
    int xBlocks = ( xPoints < 0 ) ? -1 : xPoints / BLOCK_SIZE;
    int yTopBlocks = yPoints - HALF_BLOCK_SIZE;
    if( getBlockType( xBlocks, yTopBlocks / BLOCK_SIZE ) )
    {
        return true;
    }
    int yBottomBlocks = yPoints + HALF_BLOCK_SIZE;
    if( yTopBlocks % BLOCK_SIZE != 0 && getBlockType( xBlocks, yBottomBlocks / BLOCK_SIZE ) )
    {
        return true;
    }

    return false;
}

void TetrisModel::clean() {
    for( int i = m_heightBlocks - 1; i >= 0; --i )
    {
        int counter = std::accumulate(
            m_fieldMatrix[ i ].begin(),
            m_fieldMatrix[ i ].end(),
            0,
            []( int a, int b ) { return ( b == 0 ) ? a : a + 1; }
        );

        if( counter == 0 )
        {
            return;
        }
        else if( counter == getWidthBlocks() )
        {
            m_fieldMatrix.erase( m_fieldMatrix.begin() + i );
            std::vector< int > v( getWidthBlocks() );
            m_fieldMatrix.insert( m_fieldMatrix.begin(), v );
            incScore();
            ++i;
        }
    }
}

void TetrisModel::incScore()
{
    ++m_score;
    if( m_score % SCORE_COUNT_FOR_NEXT_LEVEL == 0 )
    {
        incSpeed();
    }
}

void TetrisModel::incSpeed()
{
    if( m_speed < MAX_SPEED )
    {
        ++m_speed;
    }
}


// ********************************************************************************
TetrisItem::TetrisItem( int xPoints, int yPoints ) : m_xPoints( xPoints ), m_yPoints( yPoints )
{
}

TetrisItem TetrisItem::generateRandom() {
    static const std::vector< TetrisItem > ITEMS = {
        TetrisItem( {
            { 1, 1 },
            { 1, 1 },
        } ),
        TetrisItem( {
            { 0, 2, 0, 0 },
            { 0, 2, 0, 0 },
            { 0, 2, 0, 0 },
            { 0, 2, 0, 0 },
        } ),
        TetrisItem( {
            { 0, 3, 0 },
            { 0, 3, 0 },
            { 0, 3, 3 },
        } ),
        TetrisItem( {
            { 0, 4, 0 },
            { 0, 4, 0 },
            { 4, 4, 0 },
        } ),
        TetrisItem( {
            { 0, 5, 0 },
            { 5, 5, 5 },
            { 0, 0, 0 },
        } ),
        TetrisItem( {
            { 6, 6, 0 },
            { 0, 6, 6 },
            { 0, 0, 0 },
        } ),
        TetrisItem( {
            { 0, 7, 7 },
            { 7, 7, 0 },
            { 0, 0, 0 },
        } )
    };
    int type = rand() % ITEMS.size();
    return ITEMS[ type ];
}

bool TetrisItem::isNull() const
{
    return m_matrix.empty();
}

void TetrisItem::rotate()
{
    Matrix rotatedMatrix( getSizeBlocks() );
    for( int i = 0; i < getSizeBlocks(); ++i )
    {
        rotatedMatrix[ i ].resize( getSizeBlocks() );
        for( int j = 0; j < getSizeBlocks(); ++j ) {
            rotatedMatrix[ i ][ j ] = m_matrix[ j ][ getSizeBlocks() - 1 - i ];
        }
    }

    m_matrix = rotatedMatrix;
}

void TetrisItem::setPosition( int xPoints, int yPoints )
{
    m_xPoints = xPoints;
    m_yPoints = yPoints;
}

int TetrisItem::getXPoints() const
{
    return m_xPoints;
}

int TetrisItem::getYPoints() const
{
    return m_yPoints;
}

int TetrisItem::getSizeBlocks() const
{
    return m_matrix.size();
}

int TetrisItem::getBlockType( int innerXBlocks, int innerYBlocks ) const
{
    if( innerXBlocks < 0 || getSizeBlocks() <= innerXBlocks || innerYBlocks < 0 || getSizeBlocks() <= innerYBlocks )
    {
        return 0;
    }

    return m_matrix[ innerYBlocks ][ innerXBlocks ];
}

int TetrisItem::getBlockXPoints( int innerXBlocks ) const
{
    int innerXPoints = blocksToPoints( innerXBlocks ) + HALF_BLOCK_SIZE;
    int innerXCenterPoints = blocksToPoints( getSizeBlocks() ) / 2;
    return m_xPoints - innerXCenterPoints + innerXPoints;
}

int TetrisItem::getBlockYPoints( int innerYBlocks ) const
{
    int innerYPoints = blocksToPoints( innerYBlocks ) + HALF_BLOCK_SIZE;
    int innerYCenterPoints = blocksToPoints( getSizeBlocks() ) / 2;
    return m_yPoints - innerYCenterPoints + innerYPoints;
}

TetrisItem::TetrisItem( const Matrix& matrix ) : TetrisItem()
{
    for( const std::vector< int >& row : matrix ) {
        if( row.size() != matrix.size() ) {
            throw std::invalid_argument( "Tetris item must be represented by a square matrix" );
        }
    }

    m_matrix = matrix;
}
