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
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_test_youtube_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_youtube_mainbox.hpp"
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

    QStringList sl;
    sl.append("https://www.youtube.com/watch?v=zsfj9j0kjoU&start_radio=1&list=RDzsfj9j0kjoU");
    sl.append("https://www.youtube.com/watch?v=rKoeE00WuN0&start_radio=1&list=RDGMEMQ1dJ7wXfLlqCjwV0xfSNbAVMrKoeE00WuN0");
    sl.append("https://www.youtube.com/watch?v=PkbEjheIe9o&start_radio=1&list=RDPkbEjheIe9o");
    sl.append("https://www.youtube.com/watch?v=rhEqzW5WtbE&start_radio=1&list=RDrhEqzW5WtbE");
    sl.append("https://www.youtube.com/watch?v=1bGOgY1CmiU&start_radio=1&list=RD1bGOgY1CmiU");

    sl.append("https://www.youtube.com/watch?v=-jTx4bBOCY8");
    sl.append("https://www.youtube.com/watch?v=aNm2Qtwq3yg");
    sl.append("https://www.youtube.com/watch?v=b8I-7Wk_Vbc");
    sl.append("https://www.youtube.com/watch?v=S0jBoJXlchU");
    sl.append("https://www.youtube.com/watch?v=S0jBoJXlchU");

    sl.append("https://www.youtube.com/watch?v=8YCI1zRfqKE");
    sl.append("https://www.youtube.com/watch?v=dZnv1XB_jjk");
    sl.append("https://www.youtube.com/watch?v=46qR1HetnPo");
    sl.append("https://www.youtube.com/watch?v=JMdCM7MDEhA");
    sl.append("https://www.youtube.com/watch?v=2iSTzyAoUYk");

    sl.append("https://www.youtube.com/watch?v=OzHkNT8RqJc");
    sl.append("https://www.youtube.com/watch?v=NhZRiOo-MqE");
    sl.append("https://www.youtube.com/watch?v=oBmVt-zwiwg");
    sl.append("https://www.youtube.com/watch?v=fV9xoCIjeDg");
    sl.append("https://www.youtube.com/watch?v=AJqthEHmli4");

#if 1
    QTabWidget *tw = new QTabWidget;

    int cnt = 0;
    foreach (QString site, sl)
    {
        QWebEngineView *webView = new QWebEngineView();
        webView->setUrl(QUrl(site));
        webView->page()->setAudioMuted(true);

        tw->addTab(webView, QString("Page %1").arg(cnt++));
    }

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(tw);
    setLayout(hbox);
#else
    QGridLayout *grid = new QGridLayout;
    int x = 0;
    int y = 0;
    foreach (QString site, sl)
    {
        QWebEngineView *webView = new QWebEngineView();
        webView->setUrl(QUrl(site));
        webView->page()->setAudioMuted(true);

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
    setLayout(grid);
#endif
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
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{

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
