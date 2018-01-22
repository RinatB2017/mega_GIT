#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализируем объекты QLineEdit
    line1 = new QLineEdit();
    line2 = new QLineEdit();

    // Устанавлвиваем их в gridLayout
    ui->gridLayout->addWidget(line1,0,1);
    ui->gridLayout->addWidget(line2,0,2);

    scene = new QGraphicsScene();       // Инициализируем графическю сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем сцену в graphicsView
    scene->setSceneRect(0,0,300,300);   // Устанавливаем область сцены
    square = new Square();              // Инициализируем квадрат
    square->setCallbackFunc(getPosition);   // Устанавливаем в квадрат callback функцию
    square->setPos(100,100);            // Устанавливаем стартовую позицию квадрата
    scene->addItem(square);             // Добавляем квадрат на графическую сцену
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* callback функция получает позицию квадрата
 * и помещает его координаты в line1 и line2
 * */
void MainWindow::getPosition(QPointF point)
{
    line1->setText(QString::number(point.x()));
    line2->setText(QString::number(point.y()));
}

QLineEdit * MainWindow::line1;
QLineEdit * MainWindow::line2;
