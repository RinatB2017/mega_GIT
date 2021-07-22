/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include <QMetaProperty>
//--------------------------------------------------------------------------------
#include "for_tests_mainbox_gui.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MainBox_GUI
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent, MySplashScreen *splash);
    virtual ~MainBox();

public slots:
    void choice_test(void);

    bool test(void);
    bool test2(void);
    bool load_qss(void);

    void get_temp(int *result)
    {
        *result = 666;
    }
    void get_temp(QString *result)
    {
        *result = "t666";
    }

    void xxx(void)
    {
        fail();
    }

    void print_mp(QWidget *widget);

    //TODO здесь только для теста
    bool f1(int value);
    bool f2(int value);
    bool f3(int value);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } *cmd_t;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;

    QPointer<QComboBox> cb_test;
    QPointer<QSpinBox>  sb_test;
    QPointer<QCheckBox> cb_block;

    bool set_theme_windows(void);
    bool set_norton_commander(void);
    bool set_styles(void);

    void init(void);
    void createTestBar(void);

    template<typename T1, typename T2>
    void test_template(void);

    template<class T>
    void test_function(int delay);
    void test_function2(bool (MainBox::*func)(void));

    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
