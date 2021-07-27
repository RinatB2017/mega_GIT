#include <QDebug>

#include "widget1.hpp"
#include "ui_widget1.h"

Widget1::Widget1(QWidget *parent) :
    Widget2(parent),
    ui(new Ui::Widget1)
{
    ui->setupUi(this);
    test();
}

Widget1::~Widget1()
{
    delete ui;
}

void Widget1::test(void)
{
    qDebug() << "test1";
}
