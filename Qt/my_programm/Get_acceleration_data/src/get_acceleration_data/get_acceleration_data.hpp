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
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void read_data(QByteArray data);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    int curve_valuesAccel_x = 0;
    int curve_valuesAccel_y = 0;
    int curve_valuesAccel_z = 0;
    int curve_valuesResult_x = 0;
    int curve_valuesResult_y = 0;
    int curve_valuesResult_z = 0;
    int curve_valuesAccelMotion_x = 0;
    int curve_valuesAccelMotion_y = 0;
    int curve_valuesAccelMotion_z = 0;
    int curve_valuesAccelGravity_x = 0;
    int curve_valuesAccelGravity_y = 0;
    int curve_valuesAccelGravity_z = 0;
    int curve_valuesLinAccel_x = 0;
    int curve_valuesLinAccel_y = 0;
    int curve_valuesLinAccel_z = 0;
    int curve_valuesGravity_x = 0;
    int curve_valuesGravity_y = 0;
    int curve_valuesGravity_z = 0;
    int curve_valuesMagnet_x = 0;
    int curve_valuesMagnet_y = 0;
    int curve_valuesMagnet_z = 0;

    QList<QLCDNumber *> display_widgets;

    void init(void);
    void init_serial(void);
    void init_grapher(void);
    void init_gl_widget(void);

    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
