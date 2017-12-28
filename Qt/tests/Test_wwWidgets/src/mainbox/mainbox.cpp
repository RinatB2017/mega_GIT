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
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "qwwcolorbutton.h"
#include "qwwhuesatradialpicker.h"
#include "qwwlongspinbox.h"
#include "qwwrichtextedit.h"
#include "qwwcolorcombobox.h"
#include "qwwled.h"
#include "qwwnavigationbar.h"
#include "qwwtaskpanel.h"
#include "qwwbreadcrumb.h"
#include "qwwconfigwidget.h"
#include "qwwlistnavigator.h"
#include "qwwnumpad.h"
#include "qwwtextspinbox.h"
#include "qwwbuttonlineedit.h"
#include "qwwfilechooser.h"
#include "qwwlistwidget.h"
#include "qwwresetlineedit.h"
#include "qwwtipwidget.h"
#include "qwwclearlineedit.h"
#include "qwwhuesatpicker.h"
#include "qwwloginbox.h"
#include "qwwrichtextbutton.h"
#include "qwwtwocolorindicator.h"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
    load_config();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    //---
    f_qwwcolorbutton();
    f_qwwhuesatradialpicker();
    f_qwwlongspinbox();
    f_qwwrichtextedit();
    f_qwwcolorcombobox();
    f_qwwled();
    f_qwwnavigationbar();
    f_qwwtaskpanel();
    f_qwwbreadcrumb();
    f_qwwconfigwidget();
    f_qwwlistnavigator();
    f_qwwnumpad();
    f_qwwtextspinbox();
    f_qwwbuttonlineedit();
    f_qwwfilechooser();
    f_qwwlistwidget();
    f_qwwresetlineedit();
    f_qwwtipwidget();
    f_qwwclearlineedit();
    f_qwwhuesatpicker();
    f_qwwloginbox();
    f_qwwrichtextbutton();
    f_qwwtwocolorindicator();
    //---
    ui->btn_layout->addStretch(1);
    //---

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::choice_widget(void)
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    bool ok = false;
    int index = btn->property("index").toInt(&ok);
    if(ok)
    {
        ui->stackedWidget->setCurrentIndex(index);
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwcolorbutton(void)
{
    btn_qwwcolorbutton = new QPushButton(this);
    btn_qwwcolorbutton->setObjectName("qwwcolorbutton");
    btn_qwwcolorbutton->setText("qwwcolorbutton");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwColorButton *w = new QwwColorButton;
    w->setShowName(false);
    //w->setText("test");
    //---

    QLabel *lbl = new QLabel("qwwcolorbutton");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwcolorbutton);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwcolorbutton->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwcolorbutton, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwhuesatradialpicker(void)
{
    btn_qwwhuesatradialpicker = new QPushButton(this);
    btn_qwwhuesatradialpicker->setObjectName("qwwhuesatradialpicker");
    btn_qwwhuesatradialpicker->setText("qwwhuesatradialpicker");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwHueSatRadialPicker *w = new QwwHueSatRadialPicker;
    w->setValue(5);
    w->setColor(Qt::blue);
    //---

    QLabel *lbl = new QLabel("qwwhuesatradialpicker");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwhuesatradialpicker);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwhuesatradialpicker->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwhuesatradialpicker, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwlongspinbox(void)
{
    btn_qwwlongspinbox = new QPushButton(this);
    btn_qwwlongspinbox->setObjectName("qwwlongspinbox");
    btn_qwwlongspinbox->setText("qwwlongspinbox");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwLongSpinBox *w = new QwwLongSpinBox;
    //---

    QLabel *lbl = new QLabel("qwwlongspinbox");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwlongspinbox);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwlongspinbox->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwlongspinbox, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwrichtextedit(void)
{
    btn_qwwrichtextedit = new QPushButton(this);
    btn_qwwrichtextedit->setObjectName("qwwrichtextedit");
    btn_qwwrichtextedit->setText("qwwrichtextedit");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwRichTextEdit *w = new QwwRichTextEdit;
    w->setText("<B>Hello</B> <font color='red'>world</font>");
    //---

    QLabel *lbl = new QLabel("qwwrichtextedit");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwrichtextedit);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwrichtextedit->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwrichtextedit, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwcolorcombobox(void)
{
    btn_qwwcolorcombobox = new QPushButton(this);
    btn_qwwcolorcombobox->setObjectName("qwwcolorcombobox");
    btn_qwwcolorcombobox->setText("qwwcolorcombobox");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwColorComboBox *w = new QwwColorComboBox;
    w->addColor(Qt::red, "red");
    //---

    QLabel *lbl = new QLabel("qwwcolorcombobox");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwcolorcombobox);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwcolorcombobox->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwcolorcombobox, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwled(void)
{
    btn_qwwled = new QPushButton(this);
    btn_qwwled->setObjectName("qwwled");
    btn_qwwled->setText("qwwled");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwLed *w = new QwwLed(this);
    w->setAnimated(true);
    w->setColor(Qt::red);
    w->setPeriod(100);
    w->blink();
    //---

    QLabel *lbl = new QLabel("qwwled");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwled);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwled->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwled, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwnavigationbar(void)
{
    btn_qwwnavigationbar = new QPushButton(this);
    btn_qwwnavigationbar->setObjectName("qwwnavigationbar");
    btn_qwwnavigationbar->setText("qwwnavigationbar");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwNavigationBar *w = new QwwNavigationBar;
    //---

    QLabel *lbl = new QLabel("qwwnavigationbar");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwnavigationbar);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwnavigationbar->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwnavigationbar, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwtaskpanel(void)
{
    btn_qwwtaskpanel = new QPushButton(this);
    btn_qwwtaskpanel->setObjectName("qwwtaskpanel");
    btn_qwwtaskpanel->setText("qwwtaskpanel");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QWidget *w1 = new QWidget;
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(new QLabel("0"));
    vbox1->addWidget(new QLabel("1"));
    vbox1->addWidget(new QLabel("2"));
    vbox1->addWidget(new QLabel("3"));

    w1->setLayout(vbox1);

    QwwTaskPanel *w = new QwwTaskPanel;
    w->addTask(w1, "test");
    w->setMaximumHeight(200);
    //---

    QLabel *lbl = new QLabel("qwwtaskpanel");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwtaskpanel);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwtaskpanel->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwtaskpanel, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwbreadcrumb(void)
{
    btn_qwwbreadcrumb = new QPushButton(this);
    btn_qwwbreadcrumb->setObjectName("qwwbreadcrumb");
    btn_qwwbreadcrumb->setText("qwwbreadcrumb");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwBreadCrumb *w = new QwwBreadCrumb;
    //---

    QLabel *lbl = new QLabel("qwwbreadcrumb");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwbreadcrumb);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwbreadcrumb->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwbreadcrumb, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwconfigwidget(void)
{
    btn_qwwconfigwidget = new QPushButton(this);
    btn_qwwconfigwidget->setObjectName("qwwconfigwidget");
    btn_qwwconfigwidget->setText("qwwconfigwidget");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QWidget * w1 = new QWidget;
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(new QLabel("0"));
    vbox1->addWidget(new QLabel("1"));
    vbox1->addWidget(new QLabel("2"));
    vbox1->addWidget(new QLabel("3"));
    w1->setLayout(vbox1);

    QWidget * w2 = new QWidget;
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(new QLabel("4"));
    vbox2->addWidget(new QLabel("5"));
    vbox2->addWidget(new QLabel("6"));
    vbox2->addWidget(new QLabel("7"));
    w2->setLayout(vbox2);


    QwwConfigWidget *w = new QwwConfigWidget;
    w->addGroup(w1, qApp->style()->standardIcon(QStyle::SP_MediaPlay), "test1");
    w->addGroup(w2, qApp->style()->standardIcon(QStyle::SP_MediaStop), "test2");
    //---

    QLabel *lbl = new QLabel("qwwconfigwidget");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwconfigwidget);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwconfigwidget->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwconfigwidget, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwlistnavigator(void)
{
    btn_qwwlistnavigator = new QPushButton(this);
    btn_qwwlistnavigator->setObjectName("qwwlistnavigator");
    btn_qwwlistnavigator->setText("qwwlistnavigator");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwListNavigator *w = new QwwListNavigator;
    //---

    QLabel *lbl = new QLabel("qwwlistnavigator");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwlistnavigator);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwlistnavigator->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwlistnavigator, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwnumpad(void)
{
    btn_qwwnumpad = new QPushButton(this);
    btn_qwwnumpad->setObjectName("qwwnumpad");
    btn_qwwnumpad->setText("qwwnumpad");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwNumPad *w = new QwwNumPad;
    //---

    QLabel *lbl = new QLabel("qwwnumpad");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwnumpad);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwnumpad->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwnumpad, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwtextspinbox(void)
{
    btn_qwwtextspinbox = new QPushButton(this);
    btn_qwwtextspinbox->setObjectName("qwwtextspinbox");
    btn_qwwtextspinbox->setText("qwwtextspinbox");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwTextSpinBox *w = new QwwTextSpinBox;
    //---

    QLabel *lbl = new QLabel("qwwtextspinbox");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwtextspinbox);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwtextspinbox->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwtextspinbox, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwbuttonlineedit(void)
{
    btn_qwwbuttonlineedit = new QPushButton(this);
    btn_qwwbuttonlineedit->setObjectName("qwwbuttonlineedit");
    btn_qwwbuttonlineedit->setText("qwwbuttonlineedit");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwButtonLineEdit *w = new QwwButtonLineEdit;
    //---

    QLabel *lbl = new QLabel("qwwbuttonlineedit");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwbuttonlineedit);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwbuttonlineedit->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwbuttonlineedit, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwfilechooser(void)
{
    btn_qwwfilechooser = new QPushButton(this);
    btn_qwwfilechooser->setObjectName("qwwfilechooser");
    btn_qwwfilechooser->setText("qwwfilechooser");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwFileChooser *w = new QwwFileChooser;
    //---

    QLabel *lbl = new QLabel("qwwfilechooser");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwfilechooser);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwfilechooser->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwfilechooser, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwlistwidget(void)
{
    btn_qwwlistwidget = new QPushButton(this);
    btn_qwwlistwidget->setObjectName("qwwlistwidget");
    btn_qwwlistwidget->setText("qwwlistwidget");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwListWidget *w  =  new QwwListWidget;
    //---

    QLabel *lbl = new QLabel("qwwlistwidget");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwlistwidget);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwlistwidget->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwlistwidget, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwresetlineedit(void)
{
    btn_qwwresetlineedit = new QPushButton(this);
    btn_qwwresetlineedit->setObjectName("qwwresetlineedit");
    btn_qwwresetlineedit->setText("qwwresetlineedit");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwResetLineEdit *w = new QwwResetLineEdit;
    //---

    QLabel *lbl = new QLabel("qwwresetlineedit");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwresetlineedit);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwresetlineedit->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwresetlineedit, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwtipwidget(void)
{
    btn_qwwtipwidget = new QPushButton(this);
    btn_qwwtipwidget->setObjectName("qwwtipwidget");
    btn_qwwtipwidget->setText("qwwtipwidget");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QStringList sl;
    sl << "one" << "two" << "three";
    QwwTipWidget *w = new QwwTipWidget(sl,this);
    //---

    QLabel *lbl = new QLabel("qwwtipwidget");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwtipwidget);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwtipwidget->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwtipwidget, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwclearlineedit(void)
{
    btn_qwwclearlineedit = new QPushButton(this);
    btn_qwwclearlineedit->setObjectName("qwwclearlineedit");
    btn_qwwclearlineedit->setText("qwwclearlineedit");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwClearLineEdit *w  =  new QwwClearLineEdit;
    //---

    QLabel *lbl = new QLabel("qwwclearlineedit");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwclearlineedit);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwclearlineedit->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwclearlineedit, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwhuesatpicker(void)
{
    btn_qwwhuesatpicker = new QPushButton(this);
    btn_qwwhuesatpicker->setObjectName("qwwhuesatpicker");
    btn_qwwhuesatpicker->setText("qwwhuesatpicker");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwHueSatPicker *w = new QwwHueSatPicker;
    //---

    QLabel *lbl = new QLabel("qwwhuesatpicker");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwhuesatpicker);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwhuesatpicker->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwhuesatpicker, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwloginbox(void)
{
    btn_qwwloginbox = new QPushButton(this);
    btn_qwwloginbox->setObjectName("qwwloginbox");
    btn_qwwloginbox->setText("qwwloginbox");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwLoginBox *w = new QwwLoginBox;
    //---

    QLabel *lbl = new QLabel("qwwloginbox");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwloginbox);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwloginbox->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwloginbox, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwrichtextbutton(void)
{
    btn_qwwrichtextbutton = new QPushButton(this);
    btn_qwwrichtextbutton->setObjectName("qwwrichtextbutton");
    btn_qwwrichtextbutton->setText("qwwrichtextbutton");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwRichTextButton *w = new QwwRichTextButton;
    //---

    QLabel *lbl = new QLabel("qwwrichtextbutton");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwrichtextbutton);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwrichtextbutton->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwrichtextbutton, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::f_qwwtwocolorindicator(void)
{
    btn_qwwtwocolorindicator = new QPushButton(this);
    btn_qwwtwocolorindicator->setObjectName("qwwtwocolorindicator");
    btn_qwwtwocolorindicator->setText("qwwtwocolorindicator");

    QWidget *main_widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    main_widget->setLayout(vbox);

    //---
    QwwTwoColorIndicator *w  = new QwwTwoColorIndicator;
    //---

    QLabel *lbl = new QLabel("qwwtwocolorindicator");
    vbox->addWidget(lbl);
    vbox->addWidget(w);
    vbox->addStretch(1);

    ui->btn_layout->addWidget(btn_qwwtwocolorindicator);

    int index = ui->stackedWidget->addWidget(main_widget);
    btn_qwwtwocolorindicator->setProperty("index", index);

    ui->stackedWidget->setCurrentWidget(main_widget);

    connect(btn_qwwtwocolorindicator, SIGNAL(clicked(bool)),  this,  SLOT(choice_widget()));
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if(mouseEvent == nullptr)
    {
        return false;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::paintEvent(QPaintEvent *)
{
#if 0
    QPainter p(this);
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    p.drawLine(0, 0, width(), height());
    p.drawLine(0, height(), width(), 0);
#endif
}
//--------------------------------------------------------------------------------
