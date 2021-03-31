#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(0, []{
        QApplication::aboutQt();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

