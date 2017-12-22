#include "aisimulatorview.h"

#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>

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
MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ), m_view( new AISimulatorView( &m_model ) ), m_aiCmb( nullptr ) {

    if( QVBoxLayout* mainLayout = new QVBoxLayout ) {
        if( QHBoxLayout* l = new QHBoxLayout ) {
            l->setMargin( 0 );
            l->addStretch( 1 );
            l->addWidget( m_view );
            l->addStretch( 1 );
            mainLayout->addLayout( l );
        }
        if( QHBoxLayout* panelLayout = new QHBoxLayout ) {
            panelLayout->setMargin( 0 );
            if( QPushButton* resetBtn = new QPushButton( "Reset" ) ) {
                panelLayout->addWidget( resetBtn );
                connect( resetBtn, SIGNAL( clicked() ), SLOT( onReset() ) );
            }
            panelLayout->addStretch( 1 );
            if( ( m_aiCmb = new QComboBox ) ) {
                panelLayout->addWidget( m_aiCmb );
                connect( m_aiCmb, SIGNAL( currentIndexChanged( int ) ), SLOT( onAIChanged( int ) ) );
            }
            mainLayout->addLayout( panelLayout );
        }

        setLayout( mainLayout );
    }

    onAIChanged( 0 );
}

void MainWidget::onReset() {
    m_model.reset();
    for( auto ai : m_ais ) {
        ai->reset();
    }
}

void MainWidget::registerAI( const QString& name, BotAI* ai ) {
    if( m_aiCmb ) {
        m_aiCmb->addItem( name );
        m_ais.push_back( std::shared_ptr< BotAI >( ai ) );
    }

    if( m_ais.size() == 1 ) {
        onAIChanged( 0 );
    }
}

void MainWidget::onAIChanged( int i ) {
    if( 0 <= i && static_cast< size_t >( i ) < m_ais.size() ) {
        m_model.setAI( m_ais[ i ] );
    }
}

// ********************************************************************************
AISimulatorView::AISimulatorView( AIModel* model, QWidget* parent ) :
    QWidget( parent ), m_model( model ) {

    m_width = modelPointsToPixels( AIModel::blocksToPoints( m_model->getWidth() ) );
    m_height = modelPointsToPixels( AIModel::blocksToPoints( m_model->getHeight() ) );
    setMinimumSize( m_width, m_height );

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

    for( int x = 0; x < m_model->getWidth(); ++x ) {
        for( int y = 0; y < m_model->getHeight(); ++y ) {
            drawBlock(
                AIModel::blocksToPoints( x ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::blocksToPoints( y ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::BLOCK_SIZE,
                m_model->getBlockType( x, y ),
                &painter
            );
        }
    }

    for( const std::unique_ptr< Bot >& b : m_model->getBots() ) {
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
    m_model->doStep();
    repaint();
}
