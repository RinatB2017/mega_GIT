#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtScript/QScriptEngine>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = 0 );
    ~MainWidget();

private slots:
    void loadScript();
    void runScript();

private:
    Ui::MainWidget* ui;

    QScriptEngine m_engine;

};

#endif // MAINWIDGET_H
