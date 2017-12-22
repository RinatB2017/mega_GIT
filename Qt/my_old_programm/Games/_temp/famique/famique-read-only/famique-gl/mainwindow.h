#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gl_draw_widget.h"
#include "nes.h"
#include <QLabel>
#include <QTimer>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected :
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);
        void keyPressEvent(QKeyEvent *);
        void keyReleaseEvent(QKeyEvent *);



private:
    Ui::MainWindow *ui;
    GLWidget *m_GLDrawArea;
    NES *m_NES;
    QTimer *m_RefreshTimer;
    QTimer *m_RefreshGraphics;

    QLabel *file_name; //used for the status bar

    void Run();

    void CheckAndStopThreads();
private slots:
    void on_action_Load_binary_triggered();

public slots:
    void UpdateLogic();

};

#endif // MAINWINDOW_H
