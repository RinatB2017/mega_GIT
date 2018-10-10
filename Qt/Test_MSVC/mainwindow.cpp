
#include <QSettings>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("MyCompany", "MyApp");

    restoreGeometry(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings settings("MyCompany", "MyApp");

    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
}
