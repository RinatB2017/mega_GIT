#include "aisimulatorview.h"

#include <QPainter>

#include "easyrandomai.h"

static const bool DEBUG = true;
static const int PIXELS_IN_MODEL_POINT = 2;
static const int STEP_TIME_INTERVAL = 33;

int modelPointsToPixels( int x ) {
    return x * PIXELS_IN_MODEL_POINT;
}

static const int BLOCK_SIZE_PIXELS = modelPointsToPixels( AIModel::BLOCK_SIZE );
static const int HALF_BLOCK_SIZE_PIXELS = BLOCK_SIZE_PIXELS / 2;

static const QColor BACKGROUND_COLOR = Qt::black;
static const QColor DEBUG_GRID_COLOR = QColor( 70, 70, 70 );

// ********************************************************************************
AISimulatorView::AISimulatorView( QWidget* parent )
    : QWidget( parent ) {
    m_width = modelPointsToPixels( AIModel::blocksToPoints( m_model.getWidth() ) );
    m_height = modelPointsToPixels( AIModel::blocksToPoints( m_model.getHeight() ) );
    resize( m_width, m_height );

    m_model.setAI( new EasyRandomAI );

    connect( &m_timer, SIGNAL( timeout() ), SLOT( onTimeOut() ) );
    m_timer.start( STEP_TIME_INTERVAL );
}

AISimulatorView::~AISimulatorView() {
}

void AISimulatorView::paintEvent( QPaintEvent* ) {
    QPainter painter( this );
    painter.fillRect( 0, 0, m_width, m_height, BACKGROUND_COLOR );

    if( DEBUG ) {
        painter.setPen( DEBUG_GRID_COLOR );
        for( int x = BLOCK_SIZE_PIXELS; x < m_width; x += BLOCK_SIZE_PIXELS ) {
            painter.drawLine( x, 0, x, m_height );
        }
        for( int y = BLOCK_SIZE_PIXELS; y < m_height; y += BLOCK_SIZE_PIXELS ) {
            painter.drawLine( 0, y, m_width, y );
        }
    }

    for( int x = 0; x < m_model.getWidth(); ++x ) {
        for( int y = 0; y < m_model.getHeight(); ++y ) {
            drawBlock(
                AIModel::blocksToPoints( x ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::blocksToPoints( y ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::BLOCK_SIZE,
                m_model.getBlockType( x, y ),
                &painter
            );
        }
    }

    for( const std::unique_ptr< Bot >& b : m_model.getBots() ) {
        drawBlock( b->getX(), b->getY(), b->getSize(), 2, &painter );
    }
}

void AISimulatorView::drawBlock( int xPoints, int yPoints, int sizePoints, int type, QPainter* painter ) {
    static const std::vector< QColor > COLOR_TABLE = {
        Qt::white,
        Qt::yellow,
        Qt::green,
        Qt::red,
        Qt::cyan,
        Qt::magenta,
        Qt::lightGray
    };

    if( type <= 0 || COLOR_TABLE.size() < static_cast< size_t >( type ) ) {
        return;
    }

    const int sizePixels = modelPointsToPixels( sizePoints );
    const int halfSizePixels = sizePixels / 2;

    const int xPixels = modelPointsToPixels( xPoints ) - halfSizePixels;
    const int yPixels = modelPointsToPixels( yPoints ) - halfSizePixels;
    painter->fillRect( xPixels, yPixels, sizePixels, sizePixels, COLOR_TABLE[ type - 1 ] );
}

void AISimulatorView::onTimeOut() {
    m_model.doStep();
    repaint();
}
