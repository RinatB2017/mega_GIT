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
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QWebFrame>
#include <QLineEdit>
#include <QToolBar>
#include <QWidget>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "ui_platanov_browser_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "javascript.hpp"
#include "waiting.hpp"
#include "webview.hpp"
#include "platanov_browser_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    is_loaded(false),
    browser(0),
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
    createTestBar();

    setMinimumSize(320, 200);
}
//--------------------------------------------------------------------------------
void MainBox::web_started(void)
{
    is_loaded = false;
}
//--------------------------------------------------------------------------------
void MainBox::web_finished(bool state)
{
    if(!state) return;

    progressBar->reset();
    is_loaded = true;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(100);
    progressBar->setToolTip(tr("web progress"));

    address = new QLineEdit;
    address->setText("http://unixforum.org");

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
    connect(browser, SIGNAL(titleChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(browser, SIGNAL(loadStarted()), this, SLOT(web_started()));
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(web_finished(bool)));
    connect(browser, SIGNAL(loadProgress(int)), progressBar, SLOT(setValue(int)));

    connect(browser, SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));

    connect(btn_prev, SIGNAL(clicked()), browser, SLOT(back()));
    connect(btn_next, SIGNAL(clicked()), browser, SLOT(forward()));

    connect(address, SIGNAL(returnPressed()), this, SLOT(run()));
    connect(btn_run, SIGNAL(clicked()), this, SLOT(run()));

    btn_livejournal = new QPushButton(tr("Живой Журнал"), this);
    btn_odnoklassniki = new QPushButton(tr("Одноклассники"), this);
    btn_twitter = new QPushButton(tr("Twitter"), this);
    btn_facebook = new QPushButton(tr("Facebook"), this);
    btn_platanov = new QPushButton(tr("www.platanov.ru"), this);

    connect(btn_livejournal, SIGNAL(clicked()), this, SLOT(slot_livejournal()));
    connect(btn_odnoklassniki, SIGNAL(clicked()), this, SLOT(slot_odnoklassniki()));
    connect(btn_twitter, SIGNAL(clicked()), this, SLOT(slot_twitter()));
    connect(btn_facebook, SIGNAL(clicked()), this, SLOT(slot_facebook()));
    connect(btn_platanov, SIGNAL(clicked()), this, SLOT(slot_platanov()));

    testbar->addWidget(btn_livejournal);
    testbar->addWidget(btn_odnoklassniki);
    testbar->addWidget(btn_twitter);
    testbar->addWidget(btn_facebook);
    testbar->addWidget(btn_platanov);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(browser);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    browser->load(address->text());
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
void MainBox::slot_livejournal(void)
{
    is_loaded = false;
    browser->load(QString("http://www.livejournal.ru/"));
    Waiting::is_loaded(&is_loaded);

    emit info("email ==> Platanov");
    emit info("password ==> Ced72a63a285de49ac7c1563d99610");
}
//--------------------------------------------------------------------------------
void MainBox::slot_odnoklassniki(void)
{
    is_loaded = false;
    browser->load(QString("http://www.odnoklassniki.ru/"));
    Waiting::is_loaded(&is_loaded);

    JavaScript::setByName(browser->page()->mainFrame(), "st.email", "platanov.roman@yandex.ru");
    JavaScript::setByName(browser->page()->mainFrame(), "st.password", "9eae457f7e912ae87e52850ac67e859e");
    JavaScript::clickByName(browser->page()->mainFrame(), "button_go");
}
//--------------------------------------------------------------------------------
void MainBox::slot_twitter(void)
{
    is_loaded = false;
    browser->load(QString("https://twitter.com/"));
    Waiting::is_loaded(&is_loaded);

    emit info("email ==> platanov.roman@gmail.com");
    emit info("password ==> 5adc3b864648ced672d2848b84d9e258");
}
//--------------------------------------------------------------------------------
void MainBox::slot_facebook(void)
{
    is_loaded = false;
    browser->load(QString("https://www.facebook.com/"));
    Waiting::is_loaded(&is_loaded);

    JavaScript::setByName(browser->page()->mainFrame(), "email", "platanov.roman@gmail.com");
    JavaScript::setByName(browser->page()->mainFrame(), "pass", "1269c2def18d86259bb60782db4bdfae");

    QWebElement submit = browser->page()->mainFrame()->findFirstElement("input[type=submit]");
    JavaScript::click_element(submit);
}
//--------------------------------------------------------------------------------
void MainBox::slot_platanov(void)
{
    is_loaded = false;
    browser->load(QString("http://www.platanov.ru/"));
    Waiting::is_loaded(&is_loaded);
}
//--------------------------------------------------------------------------------
