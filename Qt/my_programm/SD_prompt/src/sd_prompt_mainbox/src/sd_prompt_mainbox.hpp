/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QClipboard>
//--------------------------------------------------------------------------------
#include "sd_prompt_mainbox_GUI.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
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
    void choice_test();
    void choice_programm();
    bool test();

    bool create_positive_prompt();
    bool create_negative_prompt();

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)();
    } CMD_t;
    QList<CMD> test_commands;
    QList<CMD> programm_commands;

    QPointer<QToolBar> test_bar;
    QPointer<QToolBar> programm_bar;
    QPointer<QComboBox> cb_test;
    QPointer<QComboBox> cb_programm;

    void init();
    void create_test_bar();
    void create_programm_bar();

    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
