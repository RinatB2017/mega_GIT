/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include <QQuickView>
//--------------------------------------------------------------------------------
#include "test_qml_mainbox_GUI.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "myfiledialog.hpp"
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
class MainBox : public MainBox_GUI
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> commands;

    QPointer<QToolBar> testbar;
    QPointer<QComboBox> cb_test;

    QString filename = "main.qml";

    void init(void);
    void createTestBar(void);

    bool test(void);
    void add_menu(void);
    void show_qml(void);

    void new_qml(void);
    void load_qml(void);
    void save_qml(void);
    void save_qml_as(void);

    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
