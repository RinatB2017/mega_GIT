#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer();
    player->setVideoOutput(ui->video_widget);
}

MainWindow::~MainWindow()
{
    player->deleteLater();

    delete ui;
}

