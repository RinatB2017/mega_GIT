#ifndef AISIMULATORVIEW_H
#define AISIMULATORVIEW_H

#include <QWidget>
#include <QTimer>

#include "aimodel.h"

class QComboBox;

class AISimulatorView;

// ********************************************************************************
class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget( QWidget* parent = 0 );

    void registerAI( const QString& name, BotAI* ai );

private slots:
    void onReset();
    void onAIChanged( int i );

private:
    AIModel m_model;
    AISimulatorView* m_view;
    QComboBox* m_aiCmb;

    std::vector< std::shared_ptr< BotAI > > m_ais;
};

// ********************************************************************************
class AISimulatorView : public QWidget {
    Q_OBJECT

public:
    AISimulatorView( AIModel* model, QWidget* parent = 0 );
    ~AISimulatorView();

protected:
    void paintEvent( QPaintEvent* );
    static void drawBlock( int xPoints, int yPoints, int sizePoints, int type, QPainter* painter );

    void mousePressEvent( QMouseEvent* e );

private slots:
    void onTimeOut();

private:
    AIModel* m_model;

    int m_width;
    int m_height;

    QTimer m_timer;

};

#endif // AISIMULATORVIEW_H
