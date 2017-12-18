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
    void f_qwwcolorbutton(void);
    void f_qwwhuesatradialpicker(void);
    void f_qwwlongspinbox(void);
    void f_qwwrichtextedit(void);
    void f_qwwcolorcombobox(void);
    void f_qwwled(void);
    void f_qwwnavigationbar(void);
    void f_qwwtaskpanel(void);
    void f_qwwbreadcrumb(void);
    void f_qwwconfigwidget(void);
    void f_qwwlistnavigator(void);
    void f_qwwnumpad(void);
    void f_qwwtextspinbox(void);
    void f_qwwbuttonlineedit(void);
    void f_qwwfilechooser(void);
    void f_qwwlistwidget(void);
    void f_qwwresetlineedit(void);
    void f_qwwtipwidget(void);
    void f_qwwclearlineedit(void);
    void f_qwwhuesatpicker(void);
    void f_qwwloginbox(void);
    void f_qwwrichtextbutton(void);
    void f_qwwtwocolorindicator(void);
    //---

    void createTestBar(void);

protected:
    virtual void changeEvent(QEvent *event);
    virtual bool eventFilter(QObject*, QEvent* event);
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
