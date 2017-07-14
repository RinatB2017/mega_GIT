#include <QApplication>
#include <QtGui>
#include <QMainWindow>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    MainWindow form(&w);
    w.setCentralWidget(&form);
    w.setFixedSize(102, 202);
    w.show();
    return a.exec();
}
