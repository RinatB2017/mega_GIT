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
#include <QToolBar>
#include <QWidget>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "webview.hpp"
#include "mainbox.hpp"
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

    setMinimumSize(320, 200);
}
//--------------------------------------------------------------------------------
void MainBox::finished(bool state)
{
    if(state)
    {
        progressBar->reset();
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(200);
    progressBar->setToolTip(tr("web progress"));

    address = new QLineEdit(this);
    address->setText("https://2ip.ru/");
    //address->setText("https://www.youtube.com");
    //address->setText("http://bash.im/quote/448209");
    //address->setText("http://bash.im");
    //address->setText("https://maps.google.com/");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_prev = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ArrowLeft),
                                       "prev",
                                       "prev");
    QToolButton *btn_next = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ArrowRight),
                                       "prev",
                                       "prev");
    testbar->addWidget(address);
    QToolButton *btn_run = add_button(testbar,
                                      new QToolButton(this),
                                      qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                                      "run",
                                      "run");
    testbar->addWidget(progressBar);

    browser = new WebView(this);
    connect(browser,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(browser,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(browser,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(browser,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    connect(browser,    SIGNAL(titleChanged(QString)),  this,           SLOT(setWindowTitle(QString)));
    connect(browser,    SIGNAL(loadProgress(int)),      progressBar,    SLOT(setValue(int)));
    connect(browser,    SIGNAL(loadFinished(bool)),     this,           SLOT(finished(bool)));

    connect(browser,    SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));

    connect(btn_prev,   SIGNAL(clicked()), browser, SLOT(back()));
    connect(btn_next,   SIGNAL(clicked()), browser, SLOT(forward()));

    connect(address,    SIGNAL(returnPressed()),    this,   SLOT(run()));
    connect(btn_run,    SIGNAL(clicked()),          this,   SLOT(run()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(browser);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    if(address->text().isEmpty())
    {
        emit error("address is empty!");
        return;
    }
    browser->load(address->text());
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
