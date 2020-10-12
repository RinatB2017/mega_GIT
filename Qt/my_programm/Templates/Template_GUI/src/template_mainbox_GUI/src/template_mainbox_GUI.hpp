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
#ifndef MAINBOX_GUI_HPP
#define MAINBOX_GUI_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox_GUI;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox_GUI : public MyWidget
{
    Q_OBJECT

signals:
    void push_btn_test(void);

public:
    explicit MainBox_GUI(QWidget *parent,
                         MySplashScreen *splash);
    ~MainBox_GUI();

    void set_value(int value);
    int  get_value(void);

public slots:
    void choice_test(void);
    virtual bool test(void) = 0;
    virtual bool test2(void) = 0;

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox_GUI::*func)(void);
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox_GUI *ui;

    QPointer<QToolBar> testbar;
    QPointer<QComboBox> cb_test;
    QPointer<QCheckBox> cb_block;
    QList<CMD> commands;

    void init(void);
    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MainBox_GUI_HPP
