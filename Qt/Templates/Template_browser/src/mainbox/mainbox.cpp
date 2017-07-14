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
    ui(new Ui::MainBox),
    flag(false),
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
void MainBox::finished(bool state)
{
    if(!state) return;

    progressBar->reset();
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    if(toolBar == nullptr)
    {
        return;
    }
    toolBar->setObjectName("toolBar");

    progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(200);
    progressBar->setToolTip(tr("web progress"));

    address = new QLineEdit;
    address->setText("https://2ip.ru/");

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_prev = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ArrowLeft),
                                       "prev",
                                       "prev");
    QToolButton *btn_next = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ArrowRight),
                                       "prev",
                                       "prev");
    toolBar->addWidget(address);
    QToolButton *btn_run = add_button(toolBar,
                                      new QToolButton(this),
                                      qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                                      "run",
                                      "run");
    toolBar->addWidget(progressBar);

    browser = new WebView(this);
    connect(browser,    SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(browser,    SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(browser,    SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(browser,    SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    connect(browser,    SIGNAL(titleChanged(QString)),  this,           SLOT(setWindowTitle(QString)));
    connect(browser,    SIGNAL(loadProgress(int)),      progressBar,    SLOT(setValue(int)));
    connect(browser,    SIGNAL(loadFinished(bool)),     this,           SLOT(finished(bool)));

    connect(browser,    SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));

    connect(btn_prev,   SIGNAL(clicked()), browser, SLOT(back()));
    connect(btn_next,   SIGNAL(clicked()), browser, SLOT(forward()));

    connect(address,    SIGNAL(returnPressed()), this, SLOT(run()));
    connect(btn_run,    SIGNAL(clicked()), this, SLOT(run()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(browser);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    browser->load(address->text());
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
