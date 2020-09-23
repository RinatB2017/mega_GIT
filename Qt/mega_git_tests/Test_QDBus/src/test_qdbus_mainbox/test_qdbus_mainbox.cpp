/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QDBusConnectionInterface>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
//--------------------------------------------------------------------------------
#include "ui_test_qdbus_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_qdbus_mainbox.hpp"
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
#define HAL_SERV      "org.freedesktop.Hal"
#define HAL_MGR_INT   "org.freedesktop.Hal.Manager"
#define HAL_DEV_INT   "org.freedesktop.Hal.Device"

#define HAL_MGR_PATH  "/org/freedesktop/Hal/Manager"
//#define HAL_DEVS_PATH "/org/freedesktop/Hal/devices"
//--------------------------------------------------------------------------------
void MainBox::connect_system_bus(void)
{
    bool ok = false;
    if (QDBusConnection::systemBus().isConnected())
    {
        QDBusConnection system_bus = QDBusConnection::systemBus();

#if 0
        //HAL
        ok = system_bus.connect(HAL_SERV,
                                HAL_MGR_PATH,
                                HAL_MGR_INT,
                                "DeviceAdded",
                                this,
                                SLOT(UDisks_deviceAdded(QDBusObjectPath)));
        emit info(QString("HAL:DeviceAdded return %1").arg(ok ? "true" : "false"));
#endif

        //UDisks
        ok = system_bus.connect("org.freedesktop.UDisks",
                                "/org/freedesktop/UDisks",
                                "org.freedesktop.UDisks",
                                "DeviceAdded",
                                this,
                                SLOT(UDisks_deviceAdded(QDBusObjectPath)));
        emit info(QString("UDisks:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = system_bus.connect("org.freedesktop.UDisks",
                                "/org/freedesktop/UDisks",
                                "org.freedesktop.UDisks",
                                "DeviceRemoved",
                                this,
                                SLOT(UDisks_deviceRemoved(QDBusObjectPath)));
        emit info(QString("UDisks:deviceRemoved return %1").arg(ok ? "true" : "false"));

        //DBus
        ok = system_bus.connect("org.freedesktop.DBus",
                                "/org/freedesktop/DBus",
                                "org.freedesktop.DBus",
                                "DeviceAdded",
                                this,
                                SLOT(DBus_deviceAdded(QDBusObjectPath)));
        emit info(QString("DBus:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = system_bus.connect("org.freedesktop.DBus",
                                "/org/freedesktop/DBus",
                                "org.freedesktop.DBus",
                                "DeviceRemoved",
                                this,
                                SLOT(DBus_deviceRemoved(QDBusObjectPath)));
        emit info(QString("DBus:deviceRemoved return %1").arg(ok ? "true" : "false"));
    }
    else
    {
        qDebug() << "NO QDBusConnection";
        emit error("NO QDBusConnection");
    }
}
//--------------------------------------------------------------------------------
void MainBox::connect_session_bus(void)
{
    bool ok = false;
    if (QDBusConnection::sessionBus().isConnected())
    {
        QDBusConnection session_bus = QDBusConnection::sessionBus();

        //UDisks
        ok = session_bus.connect("org.freedesktop.UDisks",
                                 "/org/freedesktop/UDisks",
                                 "org.freedesktop.UDisks",
                                 "DeviceAdded",
                                 this,
                                 SLOT(UDisks_deviceAdded(QDBusObjectPath)));
        emit info(QString("UDisks:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = session_bus.connect("org.freedesktop.UDisks",
                                 "/org/freedesktop/UDisks",
                                 "org.freedesktop.UDisks",
                                 "DeviceRemoved",
                                 this,
                                 SLOT(UDisks_deviceRemoved(QDBusObjectPath)));
        emit info(QString("UDisks:deviceRemoved return %1").arg(ok ? "true" : "false"));

        //DBus
        ok = session_bus.connect("org.freedesktop.DBus",
                                 "/org/freedesktop/DBus",
                                 "org.freedesktop.DBus",
                                 "DeviceAdded",
                                 this,
                                 SLOT(DBus_deviceAdded(QDBusObjectPath)));
        emit info(QString("DBus:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = session_bus.connect("org.freedesktop.DBus",
                                 "/org/freedesktop/DBus",
                                 "org.freedesktop.DBus",
                                 "DeviceRemoved",
                                 this,
                                 SLOT(DBus_deviceRemoved(QDBusObjectPath)));
        emit info(QString("DBus:deviceRemoved return %1").arg(ok ? "true" : "false"));
    }
    else
    {
        qDebug() << "NO QDBusConnection";
        emit error("NO QDBusConnection");
    }
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    //---
    connect_system_bus();
    //connect_session_bus();
    //---
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void MainBox::UDisks_deviceAdded(QDBusObjectPath dev)
{
    emit info(QString("UDisks_deviceAdded: %1").arg(dev.path()));
}
//--------------------------------------------------------------------------------
void MainBox::UDisks_deviceRemoved(QDBusObjectPath dev)
{
    emit info(QString("UDisks_deviceRemoved: %1").arg(dev.path()));
}
//--------------------------------------------------------------------------------
void MainBox::DBus_deviceAdded(QDBusObjectPath dev)
{
    emit info(QString("DBus_deviceAdded: %1").arg(dev.path()));
}
//--------------------------------------------------------------------------------
void MainBox::DBus_deviceRemoved(QDBusObjectPath dev)
{
    emit info(QString("DBus_deviceRemoved: %1").arg(dev.path()));
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

    QToolBar *testbar = new QToolBar(tr("testbar"));
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

    //toolBar->setFixedWidth(toolBar->sizeHint().width());
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
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    //---
    QDBusReply<QStringList> reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();
    if (!reply.isValid())
    {
        emit error(reply.error().message());
        return false;
    }
    foreach (QString name, reply.value())
    {
        emit info(name);
    }
    //---

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    //---
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface dbus_iface("org.freedesktop.DBus",
                              "/org/freedesktop/DBus",
                              "org.freedesktop.DBus",
                              bus);
    foreach (QVariant name, dbus_iface.call("ListNames").arguments())
    {
        emit info(QString("name %1").arg(name.toString()));
        qDebug() << name;
    }
    //---

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
