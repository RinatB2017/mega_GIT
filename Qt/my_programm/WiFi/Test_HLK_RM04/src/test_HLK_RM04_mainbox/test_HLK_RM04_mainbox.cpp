/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include "ui_test_HLK_RM04_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_HLK_RM04_mainbox.hpp"
#include "ipv4_wo_port.hpp"
#include "defines.hpp"

#include <string.h>

#include "wifi_frame.hpp"
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
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets_ipv4();
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    //frame_1 = new WIFI_frame("client", false, this);
    //frame_2 = new WIFI_frame("server", true, this);
    //frame_3 = new WIFI_frame("client", false, this);

    //QHBoxLayout *main_layout = new QHBoxLayout();
    //main_layout->addWidget(frame_1);
    //main_layout->addWidget(frame_2);
    //main_layout->addWidget(frame_3);

    connect(ui->tcp_widget, &TCP_Server::output,    this,   &MainBox::info);

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        //tcp_server не надо заводить в док
        mw->add_dock_widget("wifi",     "wifi",     Qt::LeftDockWidgetArea, reinterpret_cast<QWidget *>(ui->wifi_framewidget));
        mw->add_dock_widget("HLK-RM04", "HLK-RM04", Qt::LeftDockWidgetArea, reinterpret_cast<QWidget *>(ui->hlk_rm04_widget));
    }
    else
    {
        emit error("mw not found!");
    }
#endif

    //setLayout(main_layout);

    load_widgets();
    load_widgets_ipv4();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test", &MainBox::test });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, commands)
        {
            cb_test->addItem(command.cmd_text, QVariant(command.cmd));
        }

        testbar->addWidget(cb_test);
        QToolButton *btn_choice_test = add_button(testbar,
                                                  new QToolButton(this),
                                                  qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                  "choice_test",
                                                  "choice_test");
        btn_choice_test->setObjectName("btn_choice_test");

        connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_widgets_ipv4(void)
{
    QList<IPV4_wo_port *> ipv4_widdgets = topLevelWidget()->findChildren<IPV4_wo_port *>();
    int cnt = ipv4_widdgets.count();
    if(cnt <= 0)
    {
        emit error("IPV4_wo_port not found");
        return;
    }
    for(int n=0; n<cnt; n++)
    {
        QString oname = ipv4_widdgets.at(n)->objectName();
        beginGroup(oname);
        ipv4_widdgets.at(n)->set_a(load_value("a").toInt());
        ipv4_widdgets.at(n)->set_b(load_value("b").toInt());
        ipv4_widdgets.at(n)->set_c(load_value("c").toInt());
        ipv4_widdgets.at(n)->set_d(load_value("d").toInt());
        endGroup();
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_widgets_ipv4(void)
{
    QList<IPV4_wo_port *> ipv4_widdgets = topLevelWidget()->findChildren<IPV4_wo_port *>();
    int cnt = ipv4_widdgets.count();
    if(cnt <= 0)
    {
        emit error("IPV4_wo_port not found");
        return;
    }
    for(int n=0; n<cnt; n++)
    {
        QString oname = ipv4_widdgets.at(n)->objectName();
        beginGroup(oname);
        save_value("a", ipv4_widdgets.at(n)->get_a());
        save_value("b", ipv4_widdgets.at(n)->get_b());
        save_value("c", ipv4_widdgets.at(n)->get_c());
        save_value("d", ipv4_widdgets.at(n)->get_d());
        endGroup();
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test()");

    //ui->tcp_widget->createServerOnPort(QHostAddress::AnyIPv4, 5000);

    //    QUrl url;
    //    url.setHost("192.168.0.1");
    //    ui->hlk_rm04_widget->set_ip(url);
    //    url.setHost("255.255.255.0");
    //    ui->hlk_rm04_widget->set_mask(url);

    QList<QWidget *> allobj = topLevelWidget()->findChildren<QWidget *>();
    foreach(QWidget *widget, allobj)
    {
        if(widget->objectName().left(3) != "qt_")
        {
            emit info(widget->objectName());
        }
    }

    return true;
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
