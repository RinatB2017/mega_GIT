#ifndef AISIMULATORVIEW_H
#define AISIMULATORVIEW_H

#include <QWidget>
#include <QTimer>

#include "aimodel.h"

class QComboBox;
class QPushButton;

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
    void onKillBots();
    void onAIChanged( int i );
    void onAIChanged( int botType, int i );

    void onToolChanged();

    void onLoad();
    void onSave();

private:
    Ui::MainWidget* ui;

    AIModel m_model;
    AISimulatorView* m_view;

    std::vector< std::shared_ptr< BotAI > > m_ais;

    QHash< int, QComboBox* > m_cmbMap;
    QHash< int, QHash< int, int > > m_aiMap;

    QHash< QPushButton*, int > m_toolItemMap;
};

// ********************************************************************************
class AISimulatorView : public QWidget {
    Q_OBJECT
public:
    enum ActiveItem {
        WALL,
        ATTACK_BOT,
        DEFENSE_BOT,

        KILL,
        REMOVE

    };

public:
    AISimulatorView( AIModel* model, QWidget* parent = 0 );
    ~AISimulatorView();

    void setActiveItem( ActiveItem item );

    void setSize( int width, int height );

protected:
    void paintEvent( QPaintEvent* );
    static void drawBlock( int xPoints, int yPoints, int sizePoints, int type, QPainter* painter );
    static void drawBlock( int xPoints, int yPoints, int sizePoints, const QColor& color, QPainter* painter );

    void mousePressEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* e );

    void onMouseEvent( QMouseEvent* e );

private slots:
    void onTimeOut();

private:
    AIModel* m_model;

    int m_width;
    int m_height;

    QTimer m_timer;

    bool m_pressedLeft;
    bool m_pressedRight;
    ActiveItem m_item;

};

#endif // AISIMULATORVIEW_H
