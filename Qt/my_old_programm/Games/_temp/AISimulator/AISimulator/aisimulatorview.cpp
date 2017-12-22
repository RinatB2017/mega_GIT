#include "aisimulatorview.h"

#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QToolTip>

#include "botai.h"
#include "collisionresolver.h"

#include "ui_mainwidget.h"

static const bool DEBUG = true;
static const int PIXELS_IN_MODEL_POINT = 2;
static const int STEP_TIME_INTERVAL = 33;

static const QString MAP_FILE_EXT = ".map";

int modelPointsToPixels( int x ) {
    return x * PIXELS_IN_MODEL_POINT;
}

static const int BLOCK_SIZE_PIXELS = modelPointsToPixels( AIModel::BLOCK_SIZE );

static const QColor BACKGROUND_COLOR = Qt::black;
static const QColor DEBUG_GRID_COLOR = QColor( 70, 70, 70 );

// ********************************************************************************
MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::MainWidget ), m_view( new AISimulatorView( &m_model ) ), m_loading( false ) {
    ui->setupUi( this );

    setWindowTitle( "AISimulator" );

    ui->viewLayout->addWidget( m_view );
    connect( ui->bnSave, SIGNAL( clicked() ), SLOT( onSave() ) );
    connect( ui->bnLoad, SIGNAL( clicked() ), SLOT( onLoad() ) );
    connect( ui->btnReset, SIGNAL( clicked() ), SLOT( onReset() ) );
    connect( ui->bnKillBots, SIGNAL( clicked() ), SLOT( onKillBots() ) );

    m_cmbMap[ 2 ] = ui->cmbAttackBotAI;
    m_cmbMap[ 3 ] = ui->cmbDefenseBotAI;

    m_toolItemMap[ ui->bnWall ] = AISimulatorView::WALL;
    m_toolItemMap[ ui->bnAttackBot ] = AISimulatorView::ATTACK_BOT;
    m_toolItemMap[ ui->bnDefenseBot ] = AISimulatorView::DEFENSE_BOT;
    m_toolItemMap[ ui->bnKill ] = AISimulatorView::KILL;

    for( QComboBox * cmb : m_cmbMap ) {
        connect( cmb, SIGNAL( currentIndexChanged( int ) ), SLOT( onAIChanged( int ) ) );
    }

    connect( ui->bnStart, SIGNAL( clicked() ), SLOT( onStartPause() ) );
    connect( ui->bnPause, SIGNAL( clicked() ), SLOT( onStartPause() ) );
    connect( ui->cmbSpeed, SIGNAL( currentIndexChanged( int ) ), SLOT( onStartPause() ) );

    connect( ui->sliderWidth, SIGNAL( valueChanged( int ) ), SLOT( onMapResize() ) );
    connect( ui->sliderHeight, SIGNAL( valueChanged( int ) ), SLOT( onMapResize() ) );

    ui->cmbSpeed->setCurrentIndex( 2 );

    m_model.setCollisionResolver( std::make_shared< CollisionResolver >() );

    for( int i = 0; i < ui->layoutTools->count(); ++i ) {
        if( QPushButton* btn = dynamic_cast< QPushButton* >( ui->layoutTools->itemAt( i )->widget() ) ) {
            connect( btn, SIGNAL( clicked() ), SLOT( onToolChanged() ) );
        }
    }

    connect( new QShortcut( QKeySequence( Qt::Key_W ), this ), SIGNAL( activated() ), ui->bnWall, SLOT( click() ) );
    connect( new QShortcut( QKeySequence( Qt::Key_A ), this ), SIGNAL( activated() ), ui->bnAttackBot, SLOT( click() ) );
    connect( new QShortcut( QKeySequence( Qt::Key_D ), this ), SIGNAL( activated() ), ui->bnDefenseBot, SLOT( click() ) );
    connect( new QShortcut( QKeySequence( Qt::Key_X ), this ), SIGNAL( activated() ), ui->bnKill, SLOT( click() ) );
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::onReset() {
    m_model.reset( m_model.getWidth(), m_model.getHeight() );
    resetAIs();
}

void MainWidget::resetAIs() {
    for( auto ai : m_ais ) {
        ai->reset();
    }
}

void MainWidget::onKillBots() {
    m_model.killBots();
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

void MainWidget::onStartPause() {
    if( QPushButton* b = dynamic_cast< QPushButton* >( sender() ) ) {
        ui->bnStart->setChecked( b == ui->bnStart );
        ui->bnPause->setChecked( b == ui->bnPause );
    }

    if( ui->bnStart->isChecked() ) {
        m_view->start( ui->cmbSpeed->currentText().mid( 1 ).toDouble() );
    } else {
        m_view->pause();
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

void MainWidget::onMapResize() {
    if( m_loading ) {
        return;
    }

    m_model.resize( ui->sliderWidth->value(), ui->sliderHeight->value() );
    m_view->refreshSize();

    if( !isMaximized() && !isMinimized() ) {
        QApplication::processEvents();
        adjustSize();
    }
}

void MainWidget::onToolChanged() {
    for( int i = 0; i < ui->layoutTools->count(); ++i ) {
        if( QPushButton* btn = dynamic_cast< QPushButton* >( ui->layoutTools->itemAt( i )->widget() ) ) {
            btn->setChecked( btn == sender() );
            if( btn->isChecked() ) {
                m_view->setActiveItem( AISimulatorView::ActiveItem( m_toolItemMap[ btn ] ) );
            }
        }
    }
}

void MainWidget::onLoad() {
    QString fileName = QFileDialog::getOpenFileName( this, "Load file", ".", QString( "*%1" ).arg( MAP_FILE_EXT ) );
    if( fileName.isEmpty() ) {
        return;
    }

    if( !fileName.endsWith( MAP_FILE_EXT ) ) {
        fileName += MAP_FILE_EXT;
    }

    if( !m_model.load( fileName ) ) {
        QMessageBox::warning( this, "Error", QString( "Failed to load file '%1'" ).arg( fileName ) );
    }

    m_loading = true;
    ui->sliderWidth->setValue( m_model.getWidth() );
    ui->sliderHeight->setValue( m_model.getHeight() );
    m_loading = false;
    onMapResize();

    resetAIs();
}

void MainWidget::onSave() {
    QString fileName = QFileDialog::getSaveFileName( this, "Save file", ".", QString( "*%1" ).arg( MAP_FILE_EXT ) );
    if( fileName.isEmpty() ) {
        return;
    }

    if( !fileName.endsWith( MAP_FILE_EXT ) ) {
        fileName += MAP_FILE_EXT;
    }

    if( !m_model.save( fileName ) ) {
        QMessageBox::warning( this, "Error", QString( "Failed to save file '%1'" ).arg( fileName ) );
    }
}

// ********************************************************************************
AISimulatorView::AISimulatorView( AIModel* model, QWidget* parent ) :
    QWidget( parent ), m_model( model ), m_pressedLeft( false ), m_pressedRight( false ), m_paused( false ) {
    setActiveItem( WALL );
    setCursor( Qt::CrossCursor );

    refreshSize();

    connect( &m_timer, SIGNAL( timeout() ), SLOT( onTimeOut() ) );
    start();

    m_lblToast = new QLabel( this );
    m_lblToast->setStyleSheet( "background: rgba( 255, 255, 255, 230 ); border: 1px solid gray; font-family: monospace;" );
    m_lblToast->hide();

    connect( new QShortcut( QKeySequence( Qt::Key_Escape ), this ), SIGNAL( activated() ), SLOT( onHideBotInfo() ) );
}

AISimulatorView::~AISimulatorView() {
}

void AISimulatorView::setActiveItem( AISimulatorView::ActiveItem item ) {
    m_item = item;
}

void AISimulatorView::refreshSize() {
    m_width = modelPointsToPixels( AIModel::blocksToPoints( m_model->getWidth() ) );
    m_height = modelPointsToPixels( AIModel::blocksToPoints( m_model->getHeight() ) );
    setMinimumSize( m_width, m_height );
}

void AISimulatorView::start( double xSpeed ) {
    m_paused = false;
    m_timer.start( STEP_TIME_INTERVAL / xSpeed );
}

void AISimulatorView::pause() {
    m_paused = true;
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

    if( m_markedBot ) {
        if( m_markedBot->isDead() || m_model->getBots().isEmpty() ) {
            m_markedBot.reset();
            m_lblToast->hide();
            return;
        }

        const int xPixels = modelPointsToPixels( m_markedBot->getX() );
        const int yPixels = modelPointsToPixels( m_markedBot->getY() );

        QString directionStr;
        switch( m_markedBot->getDirection() ) {
        case Bot::LEFT:
            directionStr = "&larr;";
            break;
        case Bot::RIGHT:
            directionStr = "&rarr;";
            break;
        case Bot::UP:
            directionStr = "&uarr;";
            break;
        case Bot::DOWN:
            directionStr = "&darr;";
            break;
        default:
            break;
        }
        if( !m_markedBot->isMoving() ) {
            directionStr = "&otimes;";
        }

        m_lblToast->setText(
            QString(
                "<table width='100%'>"
                "   <tr>"
                "       <td valign='bottom'>id: <strong>%1</strong></td>"
                "       <td align='right'><font size='5'><strong>%2<strong></font></td>"
                "   </tr>"
                "</table><hr>"
                "<table width='100%'>"
                "   <tr>"
                "       <td><strong>%3</strong></td>"
                "       <td>x</td>"
                "       <td><strong>%4</strong</td>"
                "       <td align='right'>points<td>"
                "   </tr>"
                "   <tr>"
                "       <td><strong>%5</strong></td>"
                "       <td>x</td>"
                "       <td><strong>%6</strong</td>"
                "       <td align='right'>pixels<td>"
                "   </tr>"
                "</table>"
            ).arg( m_markedBot->getID() ).arg( directionStr ).arg( m_markedBot->getX() ).arg( m_markedBot->getY() ).arg( xPixels ).arg( yPixels )
        );

        int toastX = xPixels;
        int toastY = yPixels;

        if( m_width < toastX + m_lblToast->width() ) {
            toastX -= m_lblToast->width();
        }

        if( m_height < toastY + m_lblToast->height() ) {
            toastY -= m_lblToast->height();
        }

        m_lblToast->move( toastX, toastY );

        if( m_prevMarkedBot == m_markedBot ) {
            return;
        }

        m_lblToast->show();

        m_prevMarkedBot = m_markedBot;
    } else {
        m_lblToast->hide();
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
    if( !m_paused ) {
        m_model->doStep();
    }

    repaint();
}

void AISimulatorView::onHideBotInfo() {
    m_markedBot.reset();
    m_prevMarkedBot.reset();
    m_lblToast->hide();
}

void AISimulatorView::mousePressEvent( QMouseEvent* e ) {
    m_pressedLeft = e->button() == Qt::LeftButton;
    m_pressedRight = e->button() == Qt::RightButton;
    onMouseEvent( e );
}

void AISimulatorView::mouseReleaseEvent( QMouseEvent* ) {
    m_pressedLeft = false;
    m_pressedRight = false;
}

void AISimulatorView::mouseMoveEvent( QMouseEvent* e ) {
    if( !m_pressedLeft && ! m_pressedRight ) {
        return;
    }

    onMouseEvent( e );
}

void AISimulatorView::onMouseEvent( QMouseEvent* e ) {
    int xBlocks = e->pos().x() / PIXELS_IN_MODEL_POINT / AIModel::BLOCK_SIZE;
    int yBlocks = e->pos().y() / PIXELS_IN_MODEL_POINT / AIModel::BLOCK_SIZE;

    if( e->button() == Qt::MiddleButton ) {
        if( auto b = m_model->findBot( xBlocks, yBlocks ) ) {
            if( m_markedBot == b ) {
                m_markedBot.reset();
                m_prevMarkedBot.reset();
            } else {
                m_markedBot = b;
            }
            return;
        }
    }

    int xPoints = AIModel::blocksToPoints( xBlocks ) + AIModel::HALF_BLOCK_SIZE;
    int yPoints = AIModel::blocksToPoints( yBlocks ) + AIModel::HALF_BLOCK_SIZE;

    ActiveItem item = m_item;
    if( m_pressedRight ) {
        item = REMOVE;
    }

    switch( item ) {
    case WALL:
        m_model->addWall( xBlocks, yBlocks );
        break;
    case ATTACK_BOT:
        if( auto b = m_model->addBot( xPoints, yPoints, 2 ) ) {
            m_markedBot = b;
        }
        break;
    case DEFENSE_BOT:
        if( auto b = m_model->addBot( xPoints, yPoints, 3 ) ) {
            m_markedBot = b;
        }
        break;
    case KILL:
        m_model->kill( xBlocks, yBlocks );
        break;
    case REMOVE:
        m_model->remove( xBlocks, yBlocks );
        break;
    default:
        break;
    }

    repaint();
}
