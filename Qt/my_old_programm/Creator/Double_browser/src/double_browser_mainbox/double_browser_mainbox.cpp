/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QProgressBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QSplitter>
#include <QToolBar>
#include <QWidget>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "ui_double_browser_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "webview.hpp"
#include "double_browser_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    flag(false),
    other_browser(0),
    my_browser(0),
    progressBar(0),
    address(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    //QSplitter *splitter = new QSplitter(Qt::Horizontal);

    other_browser = new WebView(splitter);
    other_browser->load(QUrl("http://www.habrahabr.ru/"));

    my_browser = new WebView(splitter);
    my_browser->load(QUrl("http://www.platanov.ru/"));

    splitter->show();

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(splitter);
    //vbox->addWidget(other_browser);
    //vbox->addWidget(my_browser);

    setLayout(vbox);

    setMinimumSize(320, 200);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
