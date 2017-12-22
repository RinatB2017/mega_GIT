#include "tetriscontroller.h"

#include "tetrismodel.h"
#include "tetrisview.h"

#include <QDebug>

TetrisController::TetrisController( TetrisModel* model , TetrisView* view, QObject* parent ) :
    QObject( parent ),
    m_model( model ),
    m_view( view )
{
    connect( &m_timer, SIGNAL( timeout() ), SLOT( onStep() ) );
}

void TetrisController::onStart()
{
    m_model->resetGame();
    onResume();
}

void TetrisController::onStep()
{
    m_model->doStep();
    m_view->refresh();
    if( m_model->isGameOver() )
    {
        qDebug() << m_model->getScore();
        m_model->resetGame();
    }
}

void TetrisController::onPause()
{
    m_timer.stop();
}

void TetrisController::onResume()
{
    m_timer.start( STEP_TIME_INTERVAL );
}

void TetrisController::onMoveLeft()
{
    onAction( &TetrisModel::moveItemLeft );
}

void TetrisController::onMoveRight()
{
    onAction( &TetrisModel::moveItemRight );
}

void TetrisController::onRotate()
{
    onAction( &TetrisModel::rotateItem );
}

void TetrisController::onDropEnabled( bool enabled )
{
    onAction( enabled ? &TetrisModel::startDrop : &TetrisModel::stopDrop );
}

void TetrisController::onTogglePause()
{
    m_timer.isActive() ? onPause() : onResume();
}

void TetrisController::onAction( void ( TetrisModel::*action )() )
{
    if( !m_timer.isActive() )
    {
        return;
    }

    ( m_model->*action )();
    m_view->refresh();
}
