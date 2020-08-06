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

    //frame_1 = new WIFI_frame("client", false, this);
    //frame_2 = new WIFI_frame("server", true, this);
    //frame_3 = new WIFI_frame("client", false, this);

    //QHBoxLayout *main_layout = new QHBoxLayout();
    //main_layout->addWidget(frame_1);
    //main_layout->addWidget(frame_2);
    //main_layout->addWidget(frame_3);

    connect(ui->tcp_widget, &TCP_Server::output,    this,   &MainBox::info);

    createTestBar();

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        //tcp_server не надо заводить в док
        mw->add_dock_widget("wifi",     "wifi",     Qt::LeftDockWidgetArea, reinterpret_cast<QWidget *>(ui->wifi_framewidget));
        mw->add_dock_widget("HLK-RM04", "HLK-RM04", Qt::LeftDockWidgetArea, reinterpret_cast<QWidget *>(ui->hlk_rm04_widget));
    }

    //setLayout(main_layout);

    load_widgets();
    load_widgets_ipv4();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
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
bool MainBox::test_0(void)
{
    emit info("Test_0()");

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
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
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
