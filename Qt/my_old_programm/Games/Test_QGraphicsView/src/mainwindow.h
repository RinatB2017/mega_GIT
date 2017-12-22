//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//--------------------------------------------------------------------------------
#include <QMainWindow>
//--------------------------------------------------------------------------------
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
//--------------------------------------------------------------------------------
class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update(void);

private:
    QGraphicsScene *m_scene = 0;
    //QGraphicsEllipseItem *m_elipse = 0;
    QGraphicsView *graphicsView = 0;

    qreal pos_x = 0;
    qreal pos_y = 0;
    //qreal delta_x = 1;
    //qreal delta_y = 1;
    qreal delta = 1;

    qreal min_x = 0;
    qreal max_x = 0;
    qreal min_y = 0;
    qreal max_y = 0;

    bool flag_up = false;
    bool flag_down = false;
    bool flag_left = false;
    bool flag_right = false;

protected:
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINWINDOW_H
