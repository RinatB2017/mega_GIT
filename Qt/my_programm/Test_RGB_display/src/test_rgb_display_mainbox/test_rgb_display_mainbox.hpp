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
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

public slots:
    void show_picture(int begin_x, int begin_y);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void load_ico(void);
    void read_data(QByteArray data);

    void move_ul(void);
    void move_u(void);
    void move_ur(void);
    void move_l(void);
    void move_r(void);
    void move_dl(void);
    void move_d(void);
    void move_dr(void);

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

    MySplashScreen *splash = nullptr;
    Ui::MainBox *ui = nullptr;

    QList<RGB_dislpay_led *> l_buttons;
    double w_led = 0;
    double h_led = 0;

    QImage picture;
    int max_x = 0;
    int max_y = 0;
    int begin_x = 0;
    int begin_y = 0;

    QComboBox *cb_test = nullptr;
    QCheckBox *cb_block = nullptr;
    QList<CMD> commands;

    void init(void);
    void createTestBar(void);
    void create_display(void);
    bool prepare_data(QByteArray input, QByteArray *output);
    void load_leds(void);
    void save_leds(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
