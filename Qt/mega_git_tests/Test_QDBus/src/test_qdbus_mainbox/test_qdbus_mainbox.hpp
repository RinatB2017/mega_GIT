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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QDBusConnectionInterface>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QCheckBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test();
    bool test_0();
    bool test_1();
    bool test_2();
    bool test_3();
    bool test_4();
    bool test_5();

    void UDisks_deviceAdded(QDBusObjectPath dev);
    void UDisks_deviceRemoved(QDBusObjectPath dev);
    void DBus_deviceAdded(QDBusObjectPath dev);
    void DBus_deviceRemoved(QDBusObjectPath dev);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)();
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    //QDBusConnection system_bus;
    //QDBusConnection session_bus;

    void init();

    void connect_system_bus();
    void connect_session_bus();

    void createTestBar();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
