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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "wifi_frame.hpp"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
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

#if 1
    WIFI_frame *wf = new WIFI_frame("WiFi", false, this);
    wf->setFixedSize(wf->sizeHint());
    wf->show();
    layout()->addWidget(wf);
#endif

    createTestBar();

    ui->serialWidget->set_caption("VRM04");

    connect(this,               SIGNAL(send(QByteArray)),   ui->serialWidget,   SLOT(input(QByteArray)));
    connect(ui->serialWidget,   SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
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
    toolBar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("send data"));
    QByteArray ba;
    ba.clear();
    //ba.append("at+mode=?\r");
    //ba.append("at+net_ip=?\r");
    //ba.append("at+ver=?\r");
    ba.append("at+wifi_conf=?\r");

#if 0
    ba.append("at+netmode=1\r");
    ba.append("at+dhcpc=1\r");
    ba.append("at+remoteip=192.168.11.245\r");
    ba.append("at+remoteport=8080\r");
    ba.append("at+remotepro=tcp\r");
    ba.append("at+timeout=0\r");
    ba.append("at+mode=server\r");
    ba.append("at+uart=115200,8,n,1\r");
    ba.append("at+uartpacklen=64\r");
    ba.append("at+uartpacktimeout=10\r");
    ba.append("at+net_commit=1\r");
    ba.append("at+reconn=1\r");
#endif

    data_rs232.clear();
    is_ready = false;
    emit send(ba);
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    for(int n=0; n<ba.length(); n++)
    {
        char temp = ba.at(n);
        if(temp == '\r')
        {
            is_ready = true;
            emit debug(QString("%1").arg(data_rs232.data()));
            data_rs232.clear();
        }
        else
        {
            data_rs232.append(temp);
        }
    }
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
