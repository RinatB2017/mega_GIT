/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_walkers.h"
#include "walkers.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();;
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();

    if(youtube_walker)
    {
        youtube_walker->close();
        youtube_walker->deleteLater(); 
    }
    if(google_walker)
    {
        google_walker->close();
        google_walker->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    google_walker = new Google_walker(this);
    connect_log_signals(google_walker,  this);

    google_walker->setUrl(QUrl("https://www.google.com/"));

    youtube_walker = new Youtube_walker(this);
    connect_log_signals(youtube_walker, this);

    youtube_walker->setUrl(QUrl("https://www.youtube.com/"));

    QTabWidget *tab = new QTabWidget(this);
    tab->addTab(google_walker, "Google");
    tab->addTab(youtube_walker, "Youtube");

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(tab);
    setLayout(box);

#if 1
    QGridLayout *grid = new QGridLayout;
    int x = 0;
    int y = 0;
    for(int n=0; n<10; n++)
    {
        Youtube_walker *webView = new Youtube_walker(this);
        connect_log_signals(webView,    this);

//        webView->setUrl(QUrl("https://www.youtube.com/"));
        webView->setUrl(QUrl("https://www.youtube.com/watch?v=puhO75x3kmg"));
        webView->run(5000);
        //l_views.append(webView);

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

    QWidget *w_test = new QWidget(this);
    w_test->setLayout(grid);

    tab->addTab(w_test, "Test");
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *testbar = new QToolBar("testbar", this);
    testbar->setObjectName("testbar");

    testbar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
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
