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
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "for_tests_mainbox_gui.hpp"
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "myfiledialog.hpp"
#include "test_classes.hpp"
#include "mywidget.hpp"
// #include "ogl_widget.hpp"
// #include "cubewidget.h"
#include "defines.hpp"
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
    bool run_cube_widget(void);
    bool run_ogl_widget(void);

private:
    typedef struct
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD;
    QList<CMD> commands;

    QPointer<MySplashScreen> splash;

    QPointer<QComboBox> cb_test;
    QPointer<QSpinBox>  sb_test;
    QPointer<QCheckBox> cb_block;

    bool set_theme_windows(void);
    bool set_norton_commander(void);
    bool set_styles(void);

    void init(void);
    void create_test_bar(void);

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);

    bool calc_norm(qreal x,
                   qreal y,
                   qreal w,
                   qreal h,
                   qreal *norm_x,
                   qreal *norm_y);

    QImage create_bone(int num);

    template<typename T1, typename T2>
    void test_template(void);

    void heavy_function(bool *result);

    template<class T>
    void test_function(int delay);
    void test_function2(bool (MainBox::*func)(void));

    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
