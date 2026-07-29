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
#include <QWidget>
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

class RGB_dislpay_led;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void choice_test();
    bool test_0();
    bool test_1();
    bool test_2();
    bool test_3();
    bool test_4();
    bool test_5();

    void read_data(QByteArray data);

    void move_ul();
    void move_u();
    void move_ur();
    void move_l();
    void move_r();
    void move_dl();
    void move_d();
    void move_dr();

    void get_data(QByteArray data);
    void send_data(QString data);

    void set_param();
    void get_param();
    void set_default();

    void send_test_data();

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)();
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    int begin_x = 0;
    int begin_y = 0;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    void init();
    void createTestBar();
    bool prepare_data(QByteArray input, QByteArray *output);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
