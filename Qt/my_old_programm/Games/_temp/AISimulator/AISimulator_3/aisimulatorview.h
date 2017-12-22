#ifndef AISIMULATORVIEW_H
#define AISIMULATORVIEW_H

#include <QWidget>
#include <QTimer>

#include "aimodel.h"

class QComboBox;

class AISimulatorView;

namespace Ui {
class MainWidget;
}

// ********************************************************************************
class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget( QWidget* parent = 0 );
    ~MainWidget();

    void registerAI( const QString& name, const std::set< int >& botTypes, BotAI* ai );

private slots:
    void onReset();
    void onAIChanged( int i );
    void onAIChanged( int botType, int i );

private:
    Ui::MainWidget* ui;

    AIModel m_model;
    AISimulatorView* m_view;

    std::vector< std::shared_ptr< BotAI > > m_ais;

    QHash< int, QComboBox* > m_cmbMap;
    QHash< int, QHash< int, int > > m_aiMap;
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
    static void drawBlock( int xPoints, int yPoints, int sizePoints, const QColor& color, QPainter* painter );

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
