#ifndef AISIMULATORVIEW_H
#define AISIMULATORVIEW_H

#include <QWidget>
#include <QTimer>

#include "aimodel.h"

class AISimulatorView : public QWidget {
    Q_OBJECT

public:
    AISimulatorView( QWidget* parent = 0 );
    ~AISimulatorView();

protected:
    void paintEvent( QPaintEvent* );
    static void drawBlock( int xPoints, int yPoints, int sizePoints, int type, QPainter* painter );

private slots:
    void onTimeOut();

private:
    AIModel m_model;

    int m_width;
    int m_height;

    QTimer m_timer;

};

#endif // AISIMULATORVIEW_H
