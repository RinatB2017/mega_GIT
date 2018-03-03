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
#include <QLibraryInfo>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ui_aggregator_mainbox.h"
//--------------------------------------------------------------------------------
#include "listwidget_link.hpp"
#include "listwidget_site.hpp"
#include "mainwindow.hpp"
#include "webview.hpp"
#include "aggregator_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
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

    createTestBar();

    webview = new WebView(this);
    webview->load(QUrl("http://www.google.com"));

    lw_site = new ListWidget_Site(webview, this);
    lw_site->add_item("http://www.opennet.ru");
    lw_site->add_item("http://www.opennet.ru/#mini");
    lw_site->add_item("http://www.securitylab.ru");

    lw_link = new ListWidget_Link(webview, this);
    connect(lw_site, SIGNAL(ready(QModelIndex)), lw_link, SLOT(get_links(QModelIndex)));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(lw_site);
    hbox->addWidget(lw_link);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(webview);
    vbox->addLayout(hbox);

    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    // qDebug() << "(MainWindow *)" << mw;
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit debug("тест");
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
