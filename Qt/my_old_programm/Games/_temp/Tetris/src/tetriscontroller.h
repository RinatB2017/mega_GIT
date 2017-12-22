#ifndef TETRISCONTROLLER_H
#define TETRISCONTROLLER_H

#include <QObject>
#include <QTimer>

class TetrisModel;
class TetrisView;

class TetrisController : QObject {
    Q_OBJECT
    static const int STEP_TIME_INTERVAL = 33;

public:
    TetrisController( TetrisModel* model, TetrisView* view, QObject* parent = 0 );

public slots:
    void onStart();
    void onPause();
    void onResume();
    void onStep();

    void onMoveLeft();
    void onMoveRight();
    void onRotate();

    void onDropEnabled( bool enabled );

    void onTogglePause();

private:
    void onAction( void ( TetrisModel::*action )() );

private:
    TetrisModel* m_model;
    TetrisView* m_view;
    QTimer m_timer;
};

#endif // TETRISCONTROLLER_H
