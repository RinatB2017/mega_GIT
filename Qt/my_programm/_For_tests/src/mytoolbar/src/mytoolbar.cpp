#include "mytoolbar.hpp"
#include "ui_mytoolbar.h"

MyToolBar::MyToolBar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::MyToolBar)
{
    ui->setupUi(this);

    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setText(QString("%1").arg(n));
        addWidget(btn);
    }
}

MyToolBar::~MyToolBar()
{
    delete ui;
}
