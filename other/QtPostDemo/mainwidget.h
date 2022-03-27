#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSettings>

class QNetworkReply;
class QNetworkAccessManager;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = 0 );
    ~MainWidget();

private slots:
    void onSend();
    void onResponse( QNetworkReply* reply );

private:
    Ui::MainWidget* ui;
    QNetworkAccessManager* m_manager;
    QSettings m_settings;
};

#endif // MAINWIDGET_H
