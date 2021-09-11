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
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QToolBar>
#include <QtWebKit>
#include <QWebView>
#include <QSplitter>
#include <QComboBox>
//--------------------------------------------------------------------------------
#include "ui_teacher_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "teacher_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    is_blocked(false)
{
    init();;
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_browsers(void)
{
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->setChildrenCollapsible(false);

    ui->browser_site->setParent(splitter);
    ui->browser_translate->setParent(splitter);
    splitter->addWidget(ui->browser_site);
    splitter->addWidget(ui->browser_translate);

    ui->browser_layout->addWidget(splitter);

    //ui->browser_site->installEventFilter(this);

    site_address->setText(DEFAULT_SITE);

    ui->browser_site->setUrl(QUrl(DEFAULT_SITE));
    ui->browser_translate->setUrl(QUrl(DEFAULT_TRANSLATE));
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif
    prepare_browsers();

    ui->gridLayout->addWidget(new QLabel("я"),   0, 0);
    ui->gridLayout->addWidget(new QLabel("ты"),  0, 1);
    ui->gridLayout->addWidget(new QLabel("он"),  0, 2);
    ui->gridLayout->addWidget(new QLabel("она"), 0, 3);
    ui->gridLayout->addWidget(new QLabel("мы"),  0, 4);
    ui->gridLayout->addWidget(new QLabel("они"), 0, 5);

    ui->gridLayout->addWidget(new QLabel("I"),   1, 0);
    ui->gridLayout->addWidget(new QLabel("YOU"), 1, 1);
    ui->gridLayout->addWidget(new QLabel("HE"),  1, 2);
    ui->gridLayout->addWidget(new QLabel("SHE"), 1, 3);
    ui->gridLayout->addWidget(new QLabel("WE"),  1, 4);
    ui->gridLayout->addWidget(new QLabel("THEY"),1, 5);

    ui->frame->setFixedWidth(ui->frame->sizeHint().width());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    site_address = new QLineEdit(this);
    testbar->addWidget(site_address);

    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "test",
                                              "test");

    connect(site_address, SIGNAL(returnPressed()), this, SLOT(test()));
    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QString address = site_address->text();
    if(address.isEmpty())
    {
        emit error("address is empty!");
        return;
    }

    if(!address.toLower().startsWith("http"))
    {
        QString temp;
        temp.clear();
        temp.append("http://");
        temp.append(address);
        address = temp;

        site_address->setText(temp);
    }

    ui->browser_site->setUrl(QUrl(address));
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
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        emit info("MouseButtonPress");
        //return true;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        emit info("MouseButtonRelease");
        //return true;
    }
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
