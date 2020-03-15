#include <QDebug>

#include "widget2.hpp"
#include "ui_widget2.h"

Widget2::Widget2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget2)
{
    ui->setupUi(this);
}

Widget2::~Widget2()
{
    delete ui;
}

void Widget2::test(void)
{
    qDebug() << "test2";
}
