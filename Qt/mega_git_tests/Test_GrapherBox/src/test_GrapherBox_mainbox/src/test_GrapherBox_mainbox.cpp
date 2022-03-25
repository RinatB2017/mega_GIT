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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <qwt_picker_machine.h>
#include <qwt_curve_fitter.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_text.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "ui_test_GrapherBox_mainbox.h"
#include "testdialog.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "grapherbox.hpp"
#include "defines.hpp"
#include "test_GrapherBox_mainbox.hpp"
#include "sleeper.h"
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
    all_break = true;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    all_break = false;

    createTestBar();

#ifdef USE_SCALE_POINT_DATETIME
    //    uint x = QDateTime::currentDateTime().toTime_t();
    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    //    ui->grapher_widget->set_axis_label_rotation(45);

#if 1
    QDateTime dt = QDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));
    ui->grapher_widget->set_axis_scale_x(dt.toSecsSinceEpoch(),
                                         dt.addDays(10).toSecsSinceEpoch());
#else
    ui->grapher_widget->set_axis_scale_x(0, 3600);
#endif

    ui->grapher_widget->set_axis_scale_y(-100, 100);
#elif defined(USE_SCALE_POINT_TIME)
    uint x = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(x, x+100);
    ui->grapher_widget->set_axis_scale_y(-100, 100);
#else
    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 100);
#endif

    //ui->grapher_widget->set_legend_is_visible(true);
#ifdef ONE_CURVE
    curve_0 = ui->grapher_widget->add_curve("test");
#else
    //for(int n=0; n<MAX_CHANNELS; n++)
    for(int n=0; n<8; n++)
    {
        curves[n] = ui->grapher_widget->add_curve(QString(tr("curve %1")).arg(n));
    }
#endif
    ui->grapher_widget->legends_all_on();
    //---
    //test_data();
    //test_data2();

    ui->grapher_widget->push_btn_Horizontal(true);
    ui->grapher_widget->push_btn_Vertical(true);
    //---

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->add_dock_widget("Graphic",
                            "graphic",
                            Qt::LeftDockWidgetArea,
                            reinterpret_cast<QWidget *>(ui->grapher_widget));
        setVisible(false);
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::test_data(void)
{
    typedef struct
    {
        int x;
        qreal y;
    } temp_f;
    QList<temp_f> l_temp;
    l_temp.append({ 5000,  50000 });
    l_temp.append({ 5500,  60000 });
    l_temp.append({ 6000,  70000 });
    l_temp.append({ 6500,  80000 });
    l_temp.append({ 7000,  90000 });
    l_temp.append({ 7500,  100000 });
    l_temp.append({ 8000,  120000 });
    l_temp.append({ 8500,  140000 });
    l_temp.append({ 9000,  160000 });
    l_temp.append({ 9500,  180000 });
    l_temp.append({ 10000, 200000 });
    l_temp.append({ 11000, 225000 });
    l_temp.append({ 12000, 250000 });
    l_temp.append({ 13000, 275000 });
    l_temp.append({ 14000, 300000 });
    l_temp.append({ 15000, 350000 });
    l_temp.append({ 16000, 400000 });
    l_temp.append({ 17000, 450000 });
    l_temp.append({ 18000, 500000 });
    l_temp.append({ 19000, 550000 });
    l_temp.append({ 20000, 600000 });
    l_temp.append({ 22000, 650000 });
    l_temp.append({ 24000, 700000 });
    l_temp.append({ 25000, 750000 });
    l_temp.append({ 28000, 800000 });
    l_temp.append({ 30000, 850000 });
    l_temp.append({ 33000, 900000 });
    l_temp.append({ 36000, 950000 });
    l_temp.append({ 39000, 1000000 });
    l_temp.append({ 40000, 1100000 });

    foreach(temp_f temp, l_temp)
    {
#ifdef ONE_CURVE
        ui->grapher_widget->add_curve_data(curve_0, temp.x, temp.y);
#else
        ui->grapher_widget->add_curve_data(curves[0], temp.x, temp.y);
#endif
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_data2(void)
{
    qreal begin_y = 1000;

    qreal delta = 0.1;
    for(int n=0; n<10000; n++)
    {
#ifdef ONE_CURVE
        int temp = rand() % 10;
        if(temp >= 5)
        {
            begin_y+=delta;
        }
        else {
            begin_y-=delta;
        }
        ui->grapher_widget->add_curve_data(curve_0, n, begin_y);
#else
        int r = 20;
        for(int c_index=0; c_index<ui->grapher_widget->get_curves_count(); c_index++)
        {
            int temp = rand() % r;
            if(temp >= r / 2)
            {
                begin_y+=delta;
            }
            else {
                begin_y-=delta;
            }
            ui->grapher_widget->add_curve_data(curves[c_index], n, begin_y);
        }
#endif
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    commands.clear(); int id = 0;
    commands.append({ id++, "Фрактал Мурата",       &MainBox::m_fractal });
    commands.append({ id++, "test",                 &MainBox::test });
    commands.append({ id++, "test load",            &MainBox::test_load });
    commands.append({ id++, "test save",            &MainBox::test_save });
    commands.append({ id++, "test_sinus",           &MainBox::test0 });
    commands.append({ id++, "test_single_sinus",    &MainBox::test1 });
    commands.append({ id++, "test_random_data",     &MainBox::test2 });

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

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
        typedef void (MainBox::*function)(void);
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
void MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");

#ifdef USE_SCALE_POINT_TIME
    //TODO тест QTime
    QTime t1 = QTime(0, 0, 0);
    QTime t2 = t1.addSecs(60);
    QTime t3 = t2.addSecs(60);
    QTime t4 = t3.addSecs(60);
    QTime t5 = t4.addSecs(60);

    qDebug() << t1.toString();
    qDebug() << t2.toString();
    qDebug() << t3.toString();
    qDebug() << t4.toString();
    qDebug() << t5.toString();

    ui->grapher_widget->add_curve_data(curve_0, t1, 0);
    ui->grapher_widget->add_curve_data(curve_0, t2, 10);
    ui->grapher_widget->add_curve_data(curve_0, t3, 0);
    ui->grapher_widget->add_curve_data(curve_0, t4, 10);
    ui->grapher_widget->add_curve_data(curve_0, t5, 0);
#endif

#ifdef USE_SCALE_POINT_DATETIME
    //TODO тест QDateTime
    QDateTime dt1 = QDateTime(QDate(2020, 12, 1), QTime(0, 0, 0));
    //QDateTime(QDate(2020, 12, 3), QTime(0, 0, 0));
    QDateTime dt2 = dt1.addDays(1);
    QDateTime dt3 = dt2.addDays(1);
    QDateTime dt4 = dt3.addDays(1);
    QDateTime dt5 = dt4.addDays(1);

    qDebug() << dt1.toString();
    qDebug() << dt1.toSecsSinceEpoch();
    qDebug() << dt2.toString();
    qDebug() << dt2.toSecsSinceEpoch();
    qDebug() << dt3.toString();
    qDebug() << dt3.toSecsSinceEpoch();
    qDebug() << dt4.toString();
    qDebug() << dt4.toSecsSinceEpoch();
    qDebug() << dt5.toString();
    qDebug() << dt5.toSecsSinceEpoch();

    ui->grapher_widget->add_curve_data(curve_0, dt1, 0);
    ui->grapher_widget->add_curve_data(curve_0, dt2, 10);
    ui->grapher_widget->add_curve_data(curve_0, dt3, 0);
    ui->grapher_widget->add_curve_data(curve_0, dt4, 10);
    ui->grapher_widget->add_curve_data(curve_0, dt5, 0);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::m_fractal(void)
{
    qreal X = 1;
    qreal Y = 0;

    for(int N=1; N<21; N++)
    {
#ifdef ONE_CURVE
        ui->grapher_widget->add_curve_data(curve_0, X, Y);
#else
        //график всего один
        ui->grapher_widget->add_curve_data(curves[0], X, Y);
#endif
        emit info(QString("%1|%2")
                  .arg(X, 0, 'f', 3)
                  .arg(Y, 0, 'f', 3));
#if 1
        emit info(QString("curves: %1")
                  .arg(ui->grapher_widget->get_curves_count()));
#endif

        X = X + 6.0 * (qreal)N;
        Y = Y + 3.0 * (2.0 * (qreal)N + 1.0);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_load(void)
{
    QByteArray ba = load_bytearray("test_data");
    emit info(QString("%1").arg(ba.size()));

    int temp = static_cast<int>(ba.size()) % static_cast<int>(sizeof(qreal));
    if(temp != 0)
    {
        emit error("bad array");
        return;
    }

    QDataStream in(ba);
    qreal data;
    int size = static_cast<int>(ba.size()) / static_cast<int>(sizeof(qreal));
    //    int offset = 0;
    //    ui->grapher_widget->get_curve_data_count(0, &offset);
    for(int n=0; n<size; n++)
    {
        in >> data;
        ui->grapher_widget->add_curve_data(0, data);
        //        ui->grapher_widget->add_curve_data(0, offset + n, data);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_save(void)
{
    int cnt = 0;
    bool ok = ui->grapher_widget->get_curve_data_count(0, &cnt);
    if(ok)
    {
        emit info(QString("cnt %1").arg(cnt));
        if(cnt <= 0)
        {
            emit error(QString("cnt == %1").arg(cnt));
            return;
        }

        QByteArray ba;
        qreal temp;
        QDataStream out(&ba, QIODevice::WriteOnly);
        for(int n=0; n<cnt; n++)
        {
            ok = ui->grapher_widget->get_curve_data(0, n, &temp);
            if(ok == false)
            {
                emit error("get_curve_data return false");
                return;
            }
            out << temp;
        }
        save_bytearray("test_data", ba);
        emit error(QString("ba size %1").arg(ba.length()));
    }
    else
    {
        emit error("FAIL");
    }
}
//--------------------------------------------------------------------------------
void MainBox::test0(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);
    ui->grapher_widget->test_sinus();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test1(void)
{
    TestDialog *dlg = new TestDialog(ui->grapher_widget->get_curves_count()-1,
                                     1000);
    int res = dlg->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }

    block_interface(true);
    ui->grapher_widget->test_single_sinus(dlg->get_index(),
                                          dlg->get_offset());
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    TestDialog *dlg = new TestDialog(ui->grapher_widget->get_curves_count()-1,
                                     1000);
    int res = dlg->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }

    block_interface(true);
    ui->grapher_widget->test_random_data(dlg->get_index(),
                                         dlg->get_offset());
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test3(void)
{
    block_interface(true);
    ui->grapher_widget->test_draw_circle();
    block_interface(false);
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
