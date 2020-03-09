#include <QTimer>
#include <QDebug>

#include "myclass.h"
#include "ui_myclass.h"

MyClass::MyClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyClass)
{
    ui->setupUi(this);

    QTimer::singleShot(0, [this]{
        qDebug() << objectName();
    });
}

MyClass::~MyClass()
{
    delete ui;
}
