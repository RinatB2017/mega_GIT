/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum CURVE {
    DOTS = 0,
    LINES,
    SPLINE_LINES
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class PlotPicker;
class GrapherBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void choice_test(void);
    void test0(void);
    void test1(void);
    void test2(void);
    void test3(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> commands;
    QPointer<QComboBox> cb_test;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    bool all_break = false;

    GrapherBox *grapher_widget;

#ifdef ONE_CURVE
    int curve_0 = 0;
#else
    int curves[MAX_CHANNELS];
#endif

    void init(void);
    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

    void test_data(void);
    void test_data2(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
