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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_template_fast_trading.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "template_fast_trading.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    init_grapher_data();
    init_grapher_profit();
    init_widgets();

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
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
void MainBox::init_grapher_data(void)
{
    grapher_data = new GrapherBox(this);
    grapher_data->set_title("Data");
    grapher_data->set_title_axis_X("number");
    grapher_data->set_title_axis_Y("price");
    grapher_data->set_axis_scale_x(0, 1e6);
    grapher_data->set_axis_scale_y(0, 1e6);

    grapher_data->set_visible_btn_Load(false);
    grapher_data->set_visible_btn_Save(false);
    grapher_data->set_visible_btn_Clear(false);
    grapher_data->set_visible_btn_all_ON(false);
    grapher_data->set_visible_btn_all_OFF(false);
    grapher_data->set_visible_btn_Options(false);
    grapher_data->set_visible_btn_Statistic(false);
    grapher_data->set_silense(true);

    curve_data = grapher_data->add_curve("data");

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    mw->add_dock_widget("Grapher data", "grapher_data", Qt::RightDockWidgetArea, grapher_data);
}
//--------------------------------------------------------------------------------
void MainBox::init_grapher_profit(void)
{
    grapher_profit = new GrapherBox(this);
    grapher_profit->set_title("Profit");
    grapher_profit->set_title_axis_X("number");
    grapher_profit->set_title_axis_Y("price");
    grapher_profit->set_axis_scale_x(0, 1e6);
    grapher_profit->set_axis_scale_y(0, 1e6);

    grapher_profit->set_visible_btn_Load(false);
    grapher_profit->set_visible_btn_Save(false);
    grapher_profit->set_visible_btn_Clear(false);
    grapher_profit->set_visible_btn_all_ON(false);
    grapher_profit->set_visible_btn_all_OFF(false);
    grapher_profit->set_visible_btn_Options(false);
    grapher_profit->set_visible_btn_Statistic(false);
    grapher_profit->set_silense(true);

    curve_profit = grapher_profit->add_curve("profit");

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    mw->add_dock_widget("Grapher profit", "grapher_profit", Qt::RightDockWidgetArea, grapher_profit);
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->sb_count->setRange(1e3, 1e6);
    ui->sb_price->setRange(1e3, 1e6);
    ui->sb_inc_price->setRange(1, 1e3);
    ui->sb_profit->setRange(0, 1e3);
    ui->sb_loss->setRange(0, 1e3);

    connect(ui->btn_generate,   SIGNAL(clicked()),   this,   SLOT(generate()));
    connect(ui->btn_calc,       SIGNAL(clicked()),   this,   SLOT(calc()));
}
//--------------------------------------------------------------------------------
void MainBox::generate(void)
{
    emit trace(Q_FUNC_INFO);

    int cnt = get_count();
    if(cnt <= 0)
    {
        emit error("Bad count");
        return;
    }

    int price = get_price();
    int inc_price = get_inc_price();

    block_interface(true);

    grapher_data->clear();
    data_prices.clear();
    for(int n=0; n<cnt; n++)
    {
        int inc = (rand() % inc_price) - inc_price / 2;

        grapher_data->add_curve_data(curve_data, price);
        data_prices.append(price);

        price += inc;
    }

    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::calc(void)
{
    emit trace(Q_FUNC_INFO);

    typedef struct {
        qreal hi;
        qreal lo;
    } ORDER;

    ORDER order_up;
    ORDER order_down;

    int begin_price = get_price();
    int end_price = begin_price;
    int inc = get_inc_price();

    order_up.hi = begin_price + inc;
    order_up.lo = begin_price - inc / 2;

    order_down.hi = begin_price - inc;
    order_down.lo = begin_price + inc / 2;

    grapher_profit->clear();

    emit info(QString("Begin price %1").arg(begin_price));
    foreach(qreal price, data_prices)
    {
        if(end_price <= 0)
        {
            emit error("You lost!");
            return;
        }

        if(price >= order_up.hi)
        {

        }
        if(price < order_up.lo)
        {

        }

        if(price <= order_down.hi)
        {

        }
        if(price > order_down.lo)
        {

        }

        grapher_profit->add_curve_data(curve_profit, price);
    }
    emit info(QString("End price %1").arg(end_price));
}
//--------------------------------------------------------------------------------
int MainBox::get_count(void)
{
    return ui->sb_count->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_inc_price(void)
{
    return ui->sb_inc_price->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_price(void)
{
    return ui->sb_price->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_profit(void)
{
    return ui->sb_profit->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_loss(void)
{
    return ui->sb_loss->value();
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
