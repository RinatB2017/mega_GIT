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
class QPushButton;
class QCheckBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

    int xxx(int x);
    void test_ptr(QWidget *ptr);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void choice_widget(void);

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
    QCheckBox *cb_block;
    QList<CMD> commands;

    void init(void);

    QPushButton *btn_qwwcolorbutton;
    QPushButton *btn_qwwhuesatradialpicker;
    QPushButton *btn_qwwlongspinbox;
    QPushButton *btn_qwwrichtextedit;
    QPushButton *btn_qwwcolorcombobox;
    QPushButton *btn_qwwled;
    QPushButton *btn_qwwnavigationbar;
    QPushButton *btn_qwwtaskpanel;
    QPushButton *btn_qwwbreadcrumb;
    QPushButton *btn_qwwconfigwidget;
    QPushButton *btn_qwwlistnavigator;
    QPushButton *btn_qwwnumpad;
    QPushButton *btn_qwwtextspinbox;
    QPushButton *btn_qwwbuttonlineedit;
    QPushButton *btn_qwwfilechooser;
    QPushButton *btn_qwwlistwidget;
    QPushButton *btn_qwwresetlineedit;
    QPushButton *btn_qwwtipwidget;
    QPushButton *btn_qwwclearlineedit;
    QPushButton *btn_qwwhuesatpicker;
    QPushButton *btn_qwwloginbox;
    QPushButton *btn_qwwrichtextbutton;
    QPushButton *btn_qwwtwocolorindicator;

    //---
    void f_qwwcolorbutton(int col, int row);
    void f_qwwhuesatradialpicker(int col, int row);
    void f_qwwlongspinbox(int col, int row);
    void f_qwwrichtextedit(int col, int row);
    void f_qwwcolorcombobox(int col, int row);
    void f_qwwled(int col, int row);
    void f_qwwnavigationbar(int col, int row);
    void f_qwwtaskpanel(int col, int row);
    void f_qwwbreadcrumb(int col, int row);
    void f_qwwconfigwidget(int col, int row);
    void f_qwwlistnavigator(int col, int row);
    void f_qwwnumpad(int col, int row);
    void f_qwwtextspinbox(int col, int row);
    void f_qwwbuttonlineedit(int col, int row);
    void f_qwwfilechooser(int col, int row);
    void f_qwwlistwidget(int col, int row);
    void f_qwwresetlineedit(int col, int row);
    void f_qwwtipwidget(int col, int row);
    void f_qwwclearlineedit(int col, int row);
    void f_qwwhuesatpicker(int col, int row);
    void f_qwwloginbox(int col, int row);
    void f_qwwrichtextbutton(int col, int row);
    void f_qwwtwocolorindicator(int col, int row);
    //---

    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject*, QEvent* event);
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
