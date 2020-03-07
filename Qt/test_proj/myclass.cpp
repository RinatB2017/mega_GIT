#include <QDebug>

#include "myclass.h"
#include "ui_myclass.h"

MyClass::MyClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyClass)
{
    ui->setupUi(this);

    qDebug() << objectName();
}

MyClass::~MyClass()
{
    delete ui;
}
