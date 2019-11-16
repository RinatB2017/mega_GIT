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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------
class MyScaleDraw : public QwtScaleDraw
{
public:
    MyScaleDraw(double val)
    {
        divider = val;
    }
    virtual QwtText label(double value) const
    {
        return QString("%1").arg(value / divider, 0, 'f', 1);
    }
private:
    double divider = 1.0;
};
//--------------------------------------------
class PlotPicker: public QwtPlotPicker
{
public:
    PlotPicker(int xAxis,
               int yAxis,
               RubberBand rubberBand,
               DisplayMode trackerMode,
               double val,
               QWidget * wgt)
        :QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, wgt)
    {
        divider = val;
    }

    QwtText trackerText(const QPoint &point) const
    {
        QwtText text;
        text.setText(QString("%1:%2")
                     .arg(invTransform(point).x() / divider)
                     .arg(invTransform(point).y()));
        return text;
    }
private:
    double divider = 1.0;
};
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

    grapher_widget->set_legend_is_visible(true);
#ifdef ONE_CURVE
    curve_0 = grapher_widget->add_curve("test");
#else
    for(int n=0; n<MAX_CHANNELS; n++)
    {
        curves[n] = grapher_widget->add_curve(QString(tr("curve %1")).arg(n));
    }
#endif
    grapher_widget->legends_all_on();
    //---
    //test_data();
    test_data2();

    grapher_widget->push_btn_Horizontal(true);
    grapher_widget->push_btn_Vertical(true);

    setFixedSize(1, 1); //TODO не забудь про это
    //---

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    mw->add_dock_widget("График", "grapher", Qt::LeftDockWidgetArea, grapher_widget);
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
        int temp = rand() % 10;
        if(temp >= 5)
        {
            begin_y+=delta;
        }
        else {
            begin_y-=delta;
        }
#ifdef ONE_CURVE
        grapher_widget->add_curve_data(curve_0, n, begin_y);
#else
        grapher_widget->add_curve_data(curves[0], n, begin_y);
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

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon(":/red/0.png"),
                                       "test",
                                       "test");
    QToolButton *btn_test_2 = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(":/red/2.png"),
                                         "test2",
                                         "test2");
    QToolButton *btn_test_3 = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(":/red/3.png"),
                                         "test3",
                                         "test3");
    QToolButton *btn_test_4 = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(":/red/4.png"),
                                         "test4",
                                         "test4");
    QToolButton *btn_test_5 = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(":/red/5.png"),
                                         "test5",
                                         "test5");

    connect(btn_test,   SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_test_2, SIGNAL(clicked()), this, SLOT(test2()));
    connect(btn_test_3, SIGNAL(clicked()), this, SLOT(test3()));
    connect(btn_test_4, SIGNAL(clicked()), this, SLOT(test4()));
    connect(btn_test_5, SIGNAL(clicked()), this, SLOT(test5()));
}
//--------------------------------------------------------------------------------
void MainBox::load(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{

}
//--------------------------------------------------------------------------------
#include <QtMath>
//y = -3*x*log(x)+(0.03)*exp(-(36*x -36/e)^4)
//y = 3*x*log(x)-(0.13)*exp(-(36*x -36/e)^4)

void MainBox::test(void)
{
    block_interface(true);

#if 0
    for(qreal x=0; x<100; x+=0.01)
    {
        qreal y = -3*x*qLn(x)+(0.03)*exp(-qPow(36*x - 36/M_E,4));
        grapher_widget->add_curve_data(curve_0, static_cast<int>(x), y);
    }
#endif

#if 1
    for(qreal x=0; x<100; x+=0.01)
    {
        qreal y = 3*x*qLn(x)-(0.13)*exp(-qPow(36*x-36/M_E,4));
#ifdef ONE_CURVE
        grapher_widget->add_curve_data(curve_0, static_cast<int>(x), y);
#else
        grapher_widget->add_curve_data(curves[0], static_cast<int>(x), y);
#endif
    }
#endif

#if 0
    for(int n=0; n<MAX_CHANNELS; n++)
    {
        ui->grapher_widget->set_curve_color(n, QColor(Qt::blue));
    }
#endif

#if 0
    grapher_widget->test();
#endif

    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test1(void)
{
    block_interface(true);
    grapher_widget->test_sinus();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    block_interface(true);
    grapher_widget->test_single_sinus();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test3(void)
{
    block_interface(true);
    grapher_widget->setAxisScaleDraw(QwtPlot::xBottom, new MyScaleDraw(100.0));
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test4(void)
{
    quint64 buf[100] = { 0 };

    block_interface(true);
    for(int n=0; n<1000000; n++)
    {
        int x = rand() % 100;
        buf[x]++;
    }
    for(int n=0; n<100; n++)
    {
        grapher_widget->add_curve_data(0, n, buf[n]);
    }
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test5(void)
{

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
