#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // действия
    // сохранить
    QAction *saveAction = ui->menu->addAction(QIcon(tr(":/img/save.png")), tr("&Сохранить"));
    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveAction->setStatusTip(tr("Сохранить лабиринт в файл (*.txt)"));
    connect( saveAction, SIGNAL(triggered()), ui->widget_maze, SLOT(save()) );

    // сохранить как
    QAction *saveAsAction = ui->menu->addAction(QIcon(tr(":/img/save_as.png")), tr("&Сохранить как.."));
    saveAsAction->setStatusTip(tr("Сохранить лабиринт как.. (*.txt *png)"));
    connect( saveAsAction, SIGNAL(triggered()), ui->widget_maze, SLOT(save()) );

    // открыть
    QAction *openAction = ui->menu->addAction(QIcon(tr(":/img/open.png")), tr("&Открыть.."));
    openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openAction->setStatusTip(tr("Открыть файл лабиринта (*.txt)"));
    connect( openAction, SIGNAL(triggered()), ui->widget_maze, SLOT(save()) );


    // комбинация клавиш для генерации
    ui->pushButton_generate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

    // комбинация клавиш для поиска пути
    ui->pushButton_search->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));


    // добавляем действия в тул-бар
    ui->mainToolBar->addAction(saveAction);
    ui->mainToolBar->addAction(saveAsAction);
    ui->mainToolBar->addAction(openAction);

    connect( ui->pushButton_generate, SIGNAL(clicked()), this, SLOT(blockInterface()) );
    connect( ui->pushButton_generate, SIGNAL(clicked()), ui->widget_maze, SLOT(generate()) );

    connect( ui->pushButton_search, SIGNAL(clicked()), ui->widget_maze, SLOT(pathfinding()) );
    connect( ui->widget_maze, SIGNAL(emitGenerationComplete()), this, SLOT(unBlockInterface()) );

    connect( ui->checkBox_paintingMode, SIGNAL(toggled(bool)), ui->widget_maze, SLOT(paintingModeChanged(bool)) );
    connect( ui->checkBox_paintingMode, SIGNAL(toggled(bool)), ui->widget_maze, SLOT(repaint()) );


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::blockInterface() {
    ui->pushButton_generate->setDisabled(true);
    ui->pushButton_search->setDisabled(true);
}

void MainWindow::unBlockInterface() {
    ui->pushButton_generate->setEnabled(true);
    ui->pushButton_search->setEnabled(true);
}
