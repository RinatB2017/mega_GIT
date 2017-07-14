#include "tetrisview.h"

#include <QPainter>
#include <QKeyEvent>

static const bool DEBUG = true;

static const int PIXELS_IN_MODEL_POINT = 2;

int modelPointsToPixels( int x )
{
    return x * PIXELS_IN_MODEL_POINT;
}

static const int BLOCK_SIZE_PIXELS = modelPointsToPixels( BLOCK_SIZE );
static const int HALF_BLOCK_SIZE_PIXELS = BLOCK_SIZE_PIXELS / 2;

static const QColor BACKGROUND_COLOR = Qt::black;
static const QColor DEBUG_GRID_COLOR = Qt::gray;

// ********************************************************************************
TetrisView::TetrisView( QWidget* parent ) : QWidget( parent )
{
    m_width = modelPointsToPixels( m_model.getWidthPoints() );
    m_height = modelPointsToPixels( m_model.getHeightPoints() );

    m_controller = new TetrisController( &m_model, this, this );
    m_controller->onStart();
    resize( m_width, m_height );
}

TetrisView::~TetrisView()
{
}

void TetrisView::refresh()
{
    repaint();
}

void TetrisView::paintEvent( QPaintEvent* )
{
    QPainter painter( this );
    painter.fillRect( 0, 0, m_width, m_height, BACKGROUND_COLOR );

    if( DEBUG )
    {
        painter.setPen( DEBUG_GRID_COLOR );
        for( int x = BLOCK_SIZE_PIXELS; x < m_width; x += BLOCK_SIZE_PIXELS )
        {
            painter.drawLine( x, 0, x, m_height );
        }
        for( int y = BLOCK_SIZE_PIXELS; y < m_height; y += BLOCK_SIZE_PIXELS )
        {
            painter.drawLine( 0, y, m_width, y );
        }
    }

    for( int x = 0; x < m_model.getWidthBlocks(); ++x )
    {
        for( int y = 0; y < m_model.getHeightBlocks(); ++y )
        {
            drawBlock( blocksToPoints( x ) + HALF_BLOCK_SIZE, blocksToPoints( y ) + HALF_BLOCK_SIZE, m_model.getBlockType( x, y ), &painter );
        }
    }

    const TetrisItem& item = m_model.getItem();
    for( int x = 0; x < item.getSizeBlocks(); ++x )
    {
        for( int y = 0; y < item.getSizeBlocks(); ++y )
        {
            drawBlock( item.getBlockXPoints( x ), item.getBlockYPoints( y ), item.getBlockType( x, y ), &painter );
        }
    }
}

void TetrisView::keyPressEvent( QKeyEvent* e )
{
    switch( e->key() )
    {
    case Qt::Key_Left:
        m_controller->onMoveLeft();
        break;
    case Qt::Key_Right:
        m_controller->onMoveRight();
        break;
    case Qt::Key_Up:
        m_controller->onRotate();
        break;
    case Qt::Key_Down:
        m_controller->onDropEnabled( true );
        break;
    case Qt::Key_Space:
        m_controller->onTogglePause();
        break;
    case Qt::Key_Escape:
        m_controller->onStart();
        break;
    default:
        QWidget::keyPressEvent( e );
    }
}

void TetrisView::keyReleaseEvent( QKeyEvent* e )
{
    switch( e->key() )
    {
    case Qt::Key_Down:
        m_controller->onDropEnabled( false );
        break;
    default:
        QWidget::keyPressEvent( e );
    }
}

void TetrisView::drawBlock( int xPoints, int yPoints, int type , QPainter* painter )
{
    static const std::vector< QColor > COLOR_TABLE = {
        Qt::white,
        Qt::yellow,
        Qt::green,
        Qt::red,
        Qt::cyan,
        Qt::magenta,
        Qt::lightGray
    };

    if( type <= 0 || COLOR_TABLE.size() < type )
    {
        return;
    }

    int xPixels = modelPointsToPixels( xPoints ) - HALF_BLOCK_SIZE_PIXELS;
    int yPixels = modelPointsToPixels( yPoints ) - HALF_BLOCK_SIZE_PIXELS;
    painter->fillRect( xPixels, yPixels, BLOCK_SIZE_PIXELS, BLOCK_SIZE_PIXELS, COLOR_TABLE[ type - 1 ] );
}
