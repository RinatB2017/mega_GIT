#ifndef TETRISVIEW_H
#define TETRISVIEW_H

#include <QWidget>

#include <tetrismodel.h>
#include <tetriscontroller.h>

class TetrisView : public QWidget {
    Q_OBJECT

public:
    explicit TetrisView( QWidget* parent = 0 );
    ~TetrisView();

    void refresh();

protected:
    void paintEvent( QPaintEvent* );
    void keyPressEvent( QKeyEvent* e );
    void keyReleaseEvent( QKeyEvent* e );

    void drawBlock( int xPoints, int yPoints, int type, QPainter* painter );

private:
    TetrisModel m_model;
    TetrisController* m_controller;

    int m_width;
    int m_height;
};

#endif // TETRISVIEW_H
