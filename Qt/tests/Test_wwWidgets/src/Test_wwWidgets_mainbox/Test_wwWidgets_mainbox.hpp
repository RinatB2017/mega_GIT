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

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QCheckBox *cb_block = 0;
    QList<CMD> commands;

    void init(void);

    QPushButton *btn_qwwcolorbutton = 0;
    QPushButton *btn_qwwhuesatradialpicker = 0;
    QPushButton *btn_qwwlongspinbox = 0;
    QPushButton *btn_qwwrichtextedit = 0;
    QPushButton *btn_qwwcolorcombobox = 0;
    QPushButton *btn_qwwled = 0;
    QPushButton *btn_qwwnavigationbar = 0;
    QPushButton *btn_qwwtaskpanel = 0;
    QPushButton *btn_qwwbreadcrumb = 0;
    QPushButton *btn_qwwconfigwidget = 0;
    QPushButton *btn_qwwlistnavigator = 0;
    QPushButton *btn_qwwnumpad = 0;
    QPushButton *btn_qwwtextspinbox = 0;
    QPushButton *btn_qwwbuttonlineedit = 0;
    QPushButton *btn_qwwfilechooser = 0;
    QPushButton *btn_qwwlistwidget = 0;
    QPushButton *btn_qwwresetlineedit = 0;
    QPushButton *btn_qwwtipwidget = 0;
    QPushButton *btn_qwwclearlineedit = 0;
    QPushButton *btn_qwwhuesatpicker = 0;
    QPushButton *btn_qwwloginbox = 0;
    QPushButton *btn_qwwrichtextbutton = 0;
    QPushButton *btn_qwwtwocolorindicator = 0;

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

protected:
    bool eventFilter(QObject*, QEvent* event);
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
