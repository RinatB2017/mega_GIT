#include "mytoolbar.hpp"
#include "ui_mytoolbar.h"

MyToolBar::MyToolBar(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::MyToolBar)
{
    ui->setupUi(this);
}

MyToolBar::~MyToolBar()
{
    delete ui;
}
