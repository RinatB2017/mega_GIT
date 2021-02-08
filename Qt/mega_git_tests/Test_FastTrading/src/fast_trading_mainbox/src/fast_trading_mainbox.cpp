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
#include "ui_fast_trading_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "fast_trading_mainbox.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    init_grapher_data();
    init_grapher_profit();
    init_widgets();

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
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
    //grapher_data->set_visible_btn_Clear(false);
    grapher_data->set_visible_btn_all_ON(false);
    grapher_data->set_visible_btn_all_OFF(false);
    grapher_data->set_visible_btn_Options(false);
    grapher_data->set_visible_btn_Statistic(false);
    grapher_data->set_silense(true);

    grapher_data->push_btn_Horizontal(true);
    grapher_data->push_btn_Vertical(true);

    curve_data = grapher_data->add_curve("data");

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("Grapher data", "grapher_data", Qt::RightDockWidgetArea, reinterpret_cast<QWidget *>(grapher_data));
    }
#endif
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
    //grapher_profit->set_visible_btn_Clear(false);
    grapher_profit->set_visible_btn_all_ON(false);
    grapher_profit->set_visible_btn_all_OFF(false);
    grapher_profit->set_visible_btn_Options(false);
    grapher_profit->set_visible_btn_Statistic(false);
    grapher_profit->set_silense(true);

    grapher_profit->push_btn_Horizontal(true);
    grapher_profit->push_btn_Vertical(true);

    curve_profit = grapher_profit->add_curve("profit");

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("Grapher profit", "grapher_profit", Qt::RightDockWidgetArea, reinterpret_cast<QWidget *>(grapher_profit));
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->sb_count->setRange(1e3, 1e6);
    ui->sb_price->setRange(1e3, 1e6);
    ui->sb_inc_price->setRange(1, 1e3);
    ui->sb_order_up_profit->setRange(0, 1e3);
    ui->sb_order_up_loss->setRange(0, 1e3);
    ui->sb_order_down_profit->setRange(0, 1e3);
    ui->sb_order_down_loss->setRange(0, 1e3);

    connect(ui->btn_generate,   SIGNAL(clicked()),   this,   SLOT(generate()));
    connect(ui->btn_calc,       SIGNAL(clicked()),   this,   SLOT(calc()));

    connect(ui->btn_draw_generate,  SIGNAL(clicked()),  this,   SLOT(redraw_generate_data()));
    connect(ui->btn_draw_calc,      SIGNAL(clicked()),  this,   SLOT(redraw_calc_data()));
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

    if(ui->cb_auto_generate->isChecked())
    {
        grapher_data->clear();
    }
    prices_data.clear();

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Генерация данных");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(0);
    dlg->setMaximum(cnt);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

    for(int n=0; n<cnt; n++)
    {
        if((n % 10) == 0)
        {
            dlg->setLabelText(QString("Пожалуйста, ждите! (осталось %1 из %2)")
                              .arg(cnt - n)
                              .arg(cnt));
            dlg->setValue(n);
        }
        if(dlg->wasCanceled())
        {
            break;
        }

        int inc = (rand() % inc_price) - inc_price / 2;

        if(ui->cb_auto_generate->isChecked())
        {
            grapher_data->add_curve_data(curve_data, price);
        }
        prices_data.append(price);

        price += inc;
    }

    dlg->close();
    dlg->deleteLater();

    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::calc(void)
{
    emit trace(Q_FUNC_INFO);

    typedef struct {
        qreal profit;
        qreal loss;
    } ORDER;

    ORDER order_up;
    ORDER order_down;

    int begin_price = get_price();
    int end_price = begin_price;
    int inc = get_inc_price();

    int up_profit = get_order_up_profit();
    int up_loss   = get_order_up_loss();

    int down_profit = get_order_down_profit();
    int down_loss   = get_order_down_loss();

    order_up.profit = begin_price + inc;
    order_up.loss   = begin_price - inc / 2;

    order_down.profit = begin_price - inc;
    order_down.loss   = begin_price + inc / 2;

    if(ui->cb_auto_calc->isChecked())
    {
        grapher_profit->clear();
    }
    prices_profit.clear();

    emit info(QString("Begin price %1").arg(begin_price));
    int cnt = prices_data.count();
    int n=0;

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Подсчёт");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(0);
    dlg->setMaximum(cnt);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

    foreach(qreal price, prices_data)
    {
        if((n % 10) == 0)
        {
            dlg->setLabelText(QString("Пожалуйста, ждите! (осталось %1 из %2)")
                              .arg(cnt - n)
                              .arg(cnt));
            dlg->setValue(n);
        }
        if(dlg->wasCanceled())
        {
            break;
        }
        n++;

        if(end_price <= 0)
        {
            emit error("You lost!");
            return;
        }

        if(price >= order_up.profit)
        {
            end_price += up_profit;
        }
        if(price < order_up.loss)
        {
            end_price -= up_loss;
        }

        if(price > order_down.loss)
        {
            end_price -= down_loss;
        }
        if(price <= order_down.profit)
        {
            end_price += down_profit;
        }

        if(ui->cb_auto_calc->isChecked())
        {
            grapher_profit->add_curve_data(curve_profit, end_price);
        }
        prices_profit.append(end_price);

        order_up.profit = end_price + up_profit;
        order_up.loss   = end_price - up_loss / 2;

        order_down.profit = end_price - down_profit;
        order_down.loss   = end_price + down_loss / 2;
    }

    dlg->close();
    dlg->deleteLater();

    emit info(QString("End price %1").arg(end_price));
    emit info(QString("Profit <font style=\"color:red\">%1</font>").arg(end_price - begin_price));
}
//--------------------------------------------------------------------------------
void MainBox::redraw_generate_data(void)
{
    block_interface(true);

    int cnt = prices_data.count();

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Отрисовка данных");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(0);
    dlg->setMaximum(cnt);
    dlg->setValue(0);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

    grapher_data->clear();
    grapher_data->push_btn_Horizontal(false);
    grapher_data->push_btn_Vertical(false);
    int n = 0;
    foreach(qreal price, prices_data)
    {
        if((n % 100) == 0)
        {
            dlg->setLabelText(QString("Пожалуйста, ждите! (осталось %1 из %2)")
                              .arg(cnt - n)
                              .arg(cnt));
            dlg->setValue(n);
        }
        if(dlg->wasCanceled())
        {
            break;
        }
        n++;

        grapher_data->add_curve_data(curve_data, price);
    }
    dlg->close();
    dlg->deleteLater();

    grapher_data->push_btn_Horizontal(true);
    grapher_data->push_btn_Vertical(true);

    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::redraw_calc_data(void)
{
    block_interface(true);

    int cnt = prices_profit.count();

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Отрисовка данных");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(0);
    dlg->setMaximum(cnt);
    dlg->setValue(0);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

    grapher_profit->clear();
    grapher_profit->push_btn_Horizontal(false);
    grapher_profit->push_btn_Vertical(false);
    int n = 0;
    foreach(qreal price, prices_profit)
    {
        if((n % 100) == 0)
        {
            dlg->setLabelText(QString("Пожалуйста, ждите! (осталось %1 из %2)")
                              .arg(cnt - n)
                              .arg(cnt));
            dlg->setValue(n);
        }
        if(dlg->wasCanceled())
        {
            break;
        }
        n++;

        grapher_profit->add_curve_data(curve_profit, price);
    }
    dlg->close();
    dlg->deleteLater();

    grapher_profit->push_btn_Horizontal(true);
    grapher_profit->push_btn_Vertical(true);

    block_interface(false);
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
int MainBox::get_order_up_profit(void)
{
    return ui->sb_order_up_profit->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_order_up_loss(void)
{
    return ui->sb_order_up_loss->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_order_down_profit(void)
{
    return ui->sb_order_down_profit->value();
}
//--------------------------------------------------------------------------------
int MainBox::get_order_down_loss(void)
{
    return ui->sb_order_down_loss->value();
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
