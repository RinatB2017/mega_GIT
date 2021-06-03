/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwidget_gui.hpp"
//--------------------------------------------------------------------------------
class MainWidget : public MainWidget_GUI
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent,
                        MySplashScreen *splash);
    virtual ~MainWidget();

public slots:
    bool test(void);

private slots:
    void choice_test(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainWidget::*func)(void);
    } CMD_t;
    QList<CMD> commands;

    QPointer<QToolBar> testbar;
    QPointer<QComboBox> cb_test;
    QPointer<QCheckBox> cb_block;

    void init(void);
    void createTestBar(void);
};
//--------------------------------------------------------------------------------
#endif // MAINWIDGET_HPP
