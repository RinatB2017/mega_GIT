#include "aisimulatorview.h"

#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMouseEvent>

#include "botai.h"
#include "collisionresolver.h"

#include "ui_mainwidget.h"

static const bool DEBUG = true;
static const int PIXELS_IN_MODEL_POINT = 2;
static const int STEP_TIME_INTERVAL = 33;

int modelPointsToPixels( int x ) {
    return x * PIXELS_IN_MODEL_POINT;
}

static const int BLOCK_SIZE_PIXELS = modelPointsToPixels( AIModel::BLOCK_SIZE );

static const QColor BACKGROUND_COLOR = Qt::black;
static const QColor DEBUG_GRID_COLOR = QColor( 70, 70, 70 );

// ********************************************************************************
MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::MainWidget ), m_view( new AISimulatorView( &m_model ) ) {
    ui->setupUi( this );

    ui->viewLayout->addWidget( m_view );
    connect( ui->btnReset, SIGNAL( clicked() ), SLOT( onReset() ) );

    m_cmbMap[ 2 ] = ui->cmbAttackBotAI;
    m_cmbMap[ 3 ] = ui->cmbDefenseBotAI;

    for( QComboBox * cmb : m_cmbMap ) {
        connect( cmb, SIGNAL( currentIndexChanged( int ) ), SLOT( onAIChanged( int ) ) );
    }

    m_model.setCollisionResolver( std::make_shared< CollisionResolver >() );
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::onReset() {
    m_model.reset();
    for( auto ai : m_ais ) {
        ai->reset();
    }
}

void MainWidget::registerAI( const QString& name, const std::set< int >& botTypes, BotAI* ai ) {
    m_ais.push_back( std::shared_ptr< BotAI >( ai ) );

    for( int type : botTypes ) {
        if( m_cmbMap.contains( type ) ) {
            m_cmbMap[ type ]->addItem( name );
            m_aiMap[ type ][ m_cmbMap[ type ]->count() - 1 ] = m_ais.size() - 1;

            if( m_aiMap[ type ].size() == 1 ) {
                onAIChanged( type, 0 );
            }
        }
    }
}

void MainWidget::onAIChanged( int i ) {
    if( QComboBox* cmb = dynamic_cast< QComboBox* >( sender() ) ) {
        for( int botType : m_cmbMap.keys() ) {
            if( m_cmbMap[ botType ] == cmb ) {
                onAIChanged( botType, i );
                return;
            }
        }
    }
}

void MainWidget::onAIChanged( int botType, int i ) {
    if( m_aiMap.contains( botType ) && m_aiMap[ botType ].contains( i ) ) {
        m_model.setAI( m_ais[ m_aiMap[ botType ][ i ] ], botType );
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

QColor colorForDangerLevel( int level ) {
    static const QVector< QPair< AIModel::DangerLevel, QColor > > COLOR_TABLE = {
        qMakePair( AIModel::EXTREME, QColor( 0x33, 0x00, 0x00 ) ),
        qMakePair( AIModel::CRITICAL, QColor( 0x33, 0x1C, 0x00 ) ),
        qMakePair( AIModel::WARNING, QColor( 0x33, 0x31, 0x00 ) )
    };

    for( auto p : COLOR_TABLE ) {
        if( p.first <= level ) {
            return p.second;
        }
    }

    return BACKGROUND_COLOR;
}

void AISimulatorView::paintEvent( QPaintEvent* ) {
    QPainter painter( this );
    painter.fillRect( 0, 0, m_width, m_height, BACKGROUND_COLOR );

    const Matrix& m = m_model->getDangerMap();
    for( size_t y = 0; y < m.size(); ++y ) {
        for( size_t x = 0; x < m[ y ].size(); ++x ) {
            if( m[ y ][ x ] == 0 ) {
                continue;
            }

            drawBlock(
                AIModel::blocksToPoints( x ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::blocksToPoints( y ) + AIModel::HALF_BLOCK_SIZE,
                AIModel::BLOCK_SIZE,
                colorForDangerLevel( m[ y ][ x ] ),
                &painter
            );
        }
    }

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

    for( auto b : m_model->getBots() ) {
        drawBlock( b->getX(), b->getY(), b->getSize(), b->getType(), &painter );
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

    drawBlock( xPoints, yPoints, sizePoints, COLOR_TABLE[ type - 1 ], painter );
}

void AISimulatorView::drawBlock( int xPoints, int yPoints, int sizePoints, const QColor& color, QPainter* painter ) {
    const int sizePixels = modelPointsToPixels( sizePoints );
    const int halfSizePixels = sizePixels / 2;

    const int xPixels = modelPointsToPixels( xPoints ) - halfSizePixels;
    const int yPixels = modelPointsToPixels( yPoints ) - halfSizePixels;
    painter->fillRect( xPixels, yPixels, sizePixels, sizePixels, color );
}

void AISimulatorView::onTimeOut() {
    m_model->doStep();
    repaint();
}

void AISimulatorView::mousePressEvent( QMouseEvent* e ) {
    if( e->button() != Qt::LeftButton ) {
        QWidget::mousePressEvent( e );
        return;
    }

    int x = AIModel::blocksToPoints( e->pos().x() / PIXELS_IN_MODEL_POINT / AIModel::BLOCK_SIZE ) + AIModel::HALF_BLOCK_SIZE;
    int y = AIModel::blocksToPoints( e->pos().y() / PIXELS_IN_MODEL_POINT / AIModel::BLOCK_SIZE ) + AIModel::HALF_BLOCK_SIZE;
    m_model->addBot( x, y, 3 );
    repaint();
}
