#include "widget1.hpp"
#include "ui_widget1.h"

Widget1::Widget1(QWidget *parent) :
    Widget2(parent),
    ui(new Ui::Widget1)
{
    ui->setupUi(this);
}

Widget1::~Widget1()
{
    delete ui;
}
