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
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QDBusConnection>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
    load_config();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
#define HAL_SERV      "org.freedesktop.Hal"
#define HAL_MGR_INT   "org.freedesktop.Hal.Manager"
#define HAL_DEV_INT   "org.freedesktop.Hal.Device"

#define HAL_MGR_PATH  "/org/freedesktop/Hal/Manager"
#define HAL_DEVS_PATH "/org/freedesktop/Hal/devices"
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifndef QT_DEBUG
    Q_CHECK_PTR(parentWidget());
#endif

    createTestBar();

    //---
    bool ok = false;
    if (QDBusConnection::systemBus().isConnected())
    //if (QDBusConnection::sessionBus().isConnected())
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        //QDBusConnection bus = QDBusConnection::sessionBus();

        //UDisks
        ok = bus.connect("org.freedesktop.UDisks",
                         "/org/freedesktop/UDisks",
                         "org.freedesktop.UDisks",
                         "DeviceAdded",
                         this,
                         SLOT(UDisks_deviceAdded(QDBusObjectPath)));
        emit info(QString("UDisks:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = bus.connect("org.freedesktop.UDisks",
                         "/org/freedesktop/UDisks",
                         "org.freedesktop.UDisks",
                         "DeviceRemoved",
                         this,
                         SLOT(UDisks_deviceRemoved(QDBusObjectPath)));
        emit info(QString("UDisks:deviceRemoved return %1").arg(ok ? "true" : "false"));

        //DBus
        ok = bus.connect("org.freedesktop.DBus",
                         "/org/freedesktop/DBus",
                         "org.freedesktop.DBus",
                         "DeviceAdded",
                         this,
                         SLOT(DBus_deviceAdded(QDBusObjectPath)));
        emit info(QString("DBus:DeviceAdded return %1").arg(ok ? "true" : "false"));

        ok = bus.connect("org.freedesktop.DBus",
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
    //---
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
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
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);
    if(tool_bar == nullptr)
    {
        return nullptr;
    }
    if(tool_button == nullptr)
    {
        return nullptr;
    }

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
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
#include <QDBusConnectionInterface>
#include <QDBusReply>
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
#include <QDBusInterface>
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
bool MainBox::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if(mouseEvent == nullptr)
    {
        return false;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::MouseButtonPress)
    {
        emit info(QString("%1 %2")
                  .arg(mouseEvent->pos().x())
                  .arg(mouseEvent->pos().y()));
        return true;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::paintEvent(QPaintEvent *)
{
#if 0
    QPainter p(this);
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    p.drawLine(0, 0, width(), height());
    p.drawLine(0, height(), width(), 0);
#endif
}
//--------------------------------------------------------------------------------
