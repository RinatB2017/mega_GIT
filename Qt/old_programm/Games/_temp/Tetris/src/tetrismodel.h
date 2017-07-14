#ifndef TETRISMODEL_H
#define TETRISMODEL_H

#include <vector>

typedef std::vector< std::vector< int > > Matrix;

static const int BLOCK_SIZE = 12;
static const int HALF_BLOCK_SIZE = BLOCK_SIZE / 2;

int blocksToPoints( int x );

// ********************************************************************************
class TetrisItem {
public:
    TetrisItem( int xPoints = 0, int yPoints = 0 );
    static TetrisItem generateRandom();

    bool isNull() const;

    void rotate();

    void setPosition( int xPoints, int yPoints );
    int getXPoints() const;
    int getYPoints() const;

    int getSizeBlocks() const;
    int getBlockType( int innerXBlocks, int innerYBlocks ) const;
    int getBlockXPoints( int innerXBlocks ) const;
    int getBlockYPoints( int innerYBlocks ) const;

private:
    explicit TetrisItem( const Matrix& matrix );

private:
    int m_xPoints;
    int m_yPoints;
    Matrix m_matrix;
};

// ********************************************************************************
class TetrisModel {
    static const int MIN_SPEED = 1;
    static const int MAX_SPEED = 10;

    static const int DEFAULT_FIELD_WIDTH_BLOCKS = 15;
    static const int DEFAULT_FIELD_HEIGHT_BLOCKS = 25;

    static const int SCORE_COUNT_FOR_NEXT_LEVEL = 10;

    static const int MAX_TOUCH_COUNT = 20;

public:
    TetrisModel( int widthBlocks = DEFAULT_FIELD_WIDTH_BLOCKS, int heightBlocks = DEFAULT_FIELD_HEIGHT_BLOCKS );

    void doStep();
    bool isGameOver() const;
    int getScore() const;
    void resetGame();

    const TetrisItem& getItem() const;

    void rotateItem();
    void moveItemLeft();
    void moveItemRight();

    void startDrop();
    void stopDrop();

    int getWidthBlocks() const;
    int getHeightBlocks() const;

    int getWidthPoints() const;
    int getHeightPoints() const;

    int getBlockType( int xBlocks, int yBlocks ) const;

private:
    void moveItemX( int offsetPoints );
    bool hasCollisions( const TetrisItem& item ) const;
    bool hasCollisions( int xPoints, int yPoints ) const;

    void clean();
    void incScore();
    void incSpeed();

private:
    int m_widthBlocks;
    int m_heightBlocks;
    Matrix m_fieldMatrix;

    int m_score;
    int m_speed;

    bool m_dropEnabled;

    int m_itemBottomTouchCounter;
    TetrisItem m_activeItem;

    bool m_gameOver;
};

#endif // TETRISMODEL_H
