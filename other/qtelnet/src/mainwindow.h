#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtelnet.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void consoleChanged();

private:
    Ui::MainWindow *ui;
    qtelnet *tracker;
    QString consoleText;

    static void onDataRecv(const char *data, int size, void *bundle);

private slots:
    void onConnectClicked();
    void onConsoleChanged();
    void onSendClicked();
};

#endif // MAINWINDOW_H
