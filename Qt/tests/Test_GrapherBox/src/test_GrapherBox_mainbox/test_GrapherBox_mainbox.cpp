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
//class MyScaleDraw : public QwtScaleDraw
//{
//public:
//    MyScaleDraw(double val)
//    {
//        divider = val;
//    }
//    virtual QwtText label(double value) const
//    {
//        return QString("%1").arg(value / divider, 0, 'f', 1);
//    }
//private:
//    double divider = 1.0;
//};
//--------------------------------------------
//class PlotPicker: public QwtPlotPicker
//{
//public:
//    PlotPicker(int xAxis,
//               int yAxis,
//               RubberBand rubberBand,
//               DisplayMode trackerMode,
//               double val,
//               QWidget * wgt)
//        :QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, wgt)
//    {
//        divider = val;
//    }

//    QwtText trackerText(const QPoint &point) const
//    {
//        QwtText text;
//        text.setText(QString("%1:%2")
//                     .arg(invTransform(point).x() / divider)
//                     .arg(invTransform(point).y()));
//        return text;
//    }
//private:
//    double divider = 1.0;
//};
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

    grapher_widget = new GrapherBox(this);
    grapher_widget->setObjectName("GrapherBox");

#ifdef USE_SCALE_POINT_DATETIME
    uint x = QDateTime::currentDateTime().toTime_t();
    grapher_widget->set_title("тест");
    grapher_widget->set_title_axis_X("X");
    grapher_widget->set_title_axis_Y("Y");
    grapher_widget->set_axis_scale_x(x, x+100);
    grapher_widget->set_axis_scale_y(-100, 100);
#elif defined(USE_SCALE_POINT_TIME)
    uint x = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
    grapher_widget->set_title("тест");
    grapher_widget->set_title_axis_X("X");
    grapher_widget->set_title_axis_Y("Y");
    grapher_widget->set_axis_scale_x(x, x+100);
    grapher_widget->set_axis_scale_y(-100, 100);
#else
    grapher_widget->set_title("тест");
    grapher_widget->set_title_axis_X("X");
    grapher_widget->set_title_axis_Y("Y");
    grapher_widget->set_axis_scale_x(0, 100);
    grapher_widget->set_axis_scale_y(0, 100);
#endif

    //grapher_widget->set_legend_is_visible(true);
#ifdef ONE_CURVE
    curve_0 = grapher_widget->add_curve("test");
#else
    //for(int n=0; n<MAX_CHANNELS; n++)
    for(int n=0; n<8; n++)
    {
        curves[n] = grapher_widget->add_curve(QString(tr("curve %1")).arg(n));
    }
#endif
    grapher_widget->legends_all_on();
    //---
    //test_data();
    //test_data2();

    grapher_widget->push_btn_Horizontal(true);
    grapher_widget->push_btn_Vertical(true);

    setFixedSize(1, 1); //TODO не забудь про это
    //---

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    mw->add_dock_widget("Graphic", "graphic", Qt::LeftDockWidgetArea, grapher_widget);
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
        grapher_widget->add_curve_data(curve_0, temp.x, temp.y);
#else
        grapher_widget->add_curve_data(curves[0], temp.x, temp.y);
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
        grapher_widget->add_curve_data(curve_0, n, begin_y);
#else
        int r = 20;
        for(int c_index=0; c_index<grapher_widget->get_curves_count(); c_index++)
        {
            int temp = rand() % r;
            if(temp >= r / 2)
            {
                begin_y+=delta;
            }
            else {
                begin_y-=delta;
            }
            grapher_widget->add_curve_data(curves[c_index], n, begin_y);
        }
#endif
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    commands.clear(); int id = 0;
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
            typedef void (MainBox::*my_mega_function)(void);
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
void MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    out << static_cast<float>(1.234);
    out << static_cast<qreal>(1.234);
    emit info(QString("F %1").arg(sizeof(float)));
    emit info(QString("D %1").arg(sizeof(double)));
    emit info(QString("size %1").arg(ba.size()));
#endif

#if 0
    emit info(QString("cnt curves %1").arg(grapher_widget->get_curves_count()));

    int cnt = 0;
    bool ok = grapher_widget->get_curve_data_count(0, &cnt);
    if(ok)
        emit info(QString("real_data cnt: %1").arg(cnt));

    qreal data;
    for(int n=0; n<10; n++)
    {
        ok = grapher_widget->get_curve_data(curves[0], n, &data);
        if(ok)
            emit info(QString("data %1").arg(data));
        else
            return;
    }
#endif
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
//    grapher_widget->get_curve_data_count(0, &offset);
    for(int n=0; n<size; n++)
    {
        in >> data;
        grapher_widget->add_curve_data(0, data);
//        grapher_widget->add_curve_data(0, offset + n, data);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_save(void)
{
    int cnt = 0;
    bool ok = grapher_widget->get_curve_data_count(0, &cnt);
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
            ok = grapher_widget->get_curve_data(0, n, &temp);
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
    grapher_widget->test_sinus();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test1(void)
{
    TestDialog *dlg = new TestDialog(grapher_widget->get_curves_count()-1,
                                     1000);
    int res = dlg->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }

    block_interface(true);
    grapher_widget->test_single_sinus(dlg->get_index(),
                                      dlg->get_offset());
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    TestDialog *dlg = new TestDialog(grapher_widget->get_curves_count()-1,
                                     1000);
    int res = dlg->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }

    block_interface(true);
    grapher_widget->test_random_data(dlg->get_index(),
                                      dlg->get_offset());
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test3(void)
{
    block_interface(true);
    grapher_widget->test_draw_circle();
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
