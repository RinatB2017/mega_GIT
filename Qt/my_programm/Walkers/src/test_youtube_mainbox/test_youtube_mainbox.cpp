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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_test_youtube_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_youtube_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "youtube_walker.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
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

    youtube_walker = new Youtube_walker(this);
    connect(youtube_walker, SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(youtube_walker, SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(youtube_walker, SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(youtube_walker, SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    connect(youtube_walker, SIGNAL(show_url(QString)),  this,   SLOT(show_url(QString)));

    QGridLayout *grid = new QGridLayout;
    int x = 0;
    int y = 0;
    for(int n=0; n<20; n++)
    {
        QWebEngineView *webView = new QWebEngineView();
        //webView->setUrl(QUrl(site));
        webView->page()->setAudioMuted(true);
        l_views.append(webView);

        grid->addWidget(webView, y, x);
        if(x < 4)
        {
            x++;
        }
        else
        {
            x=0;
            y++;
        }
    }

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(youtube_walker);
    hbox->addLayout(grid);

    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *testbar = new QToolBar("testbar", this);
    testbar->setObjectName("testbar");

    testbar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));

    mw->add_windowsmenu_action(testbar->toggleViewAction());    //TODO странно
}
//--------------------------------------------------------------------------------
void MainBox::show_url(QString url)
{
    if(cnt < (l_views.count() - 1))
    {
        cnt++;
    }
    else
    {
        cnt = 0;
    }
    if(l_views[cnt])
    {
        l_views[cnt]->setUrl(QUrl(url));
    }
    else
    {
        emit error(QString("bad cnt %1").arg(cnt));
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    youtube_walker->setUrl(QUrl("https://www.youtube.com/"));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
