/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef NEW_MAINWINDOW
#define NEW_MAINWINDOW
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
class NewMainWindow : public MainWindow
{
    Q_OBJECT

public:
    NewMainWindow(MainWindow *parent = nullptr);

private slots:
    void choice_test(void);
    bool test(void);

    void push(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (NewMainWindow::*func)(void);
    } CMD_t;
    QPointer<QComboBox> cb_test;
    QCheckBox *cb_block;
    QList<CMD> commands;

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void create_test_bar(void);
};
//--------------------------------------------------------------------------------
#endif
