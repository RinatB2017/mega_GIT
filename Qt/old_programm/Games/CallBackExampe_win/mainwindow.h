#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLineEdit>

#include <square.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;  // Объявляем графическую сцену
    Square *square;         // Объявляем квадрат, в который будем передавать callback функцию
    static QLineEdit *line1;    // Объявляем static QLineEdit, с которым будет работать callback функция
    static QLineEdit *line2;    // Объявляем static QLineEdit, с которым будет работать callback функция

private:
    // Объявляем callback функцию
    static void getPosition(QPointF point);
};

#endif // MAINWINDOW_H
