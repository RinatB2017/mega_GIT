#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QWidget(parent)
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QLinearGradient gradient(100, 0, 100, 200);

    //gradient.setInterpolationMode(QLinearGradient::ColorInterpolation);
    //gradient.setInterpolationMode(QLinearGradient::ComponentInterpolation);

    gradient.setColorAt(1.0, QColor(0,   0,   0));
    gradient.setColorAt(0.8, QColor(85,  0,   170));
    gradient.setColorAt(0.6, QColor(170, 0,   170));
    gradient.setColorAt(0.4, QColor(255, 85,  0));
    gradient.setColorAt(0.2, QColor(255, 170, 0));
    gradient.setColorAt(0.0, QColor(255, 255, 255));

    painter.setBrush(gradient);
    painter.drawRect(0, 0, 100, 200);
}
