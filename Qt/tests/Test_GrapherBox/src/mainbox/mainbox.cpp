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
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QSplitter>
#include <QComboBox>
#include <QDateTime>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <qwt_picker_machine.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
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
    explicit PlotPicker(int xAxis,
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
#include "mainbox.hpp"
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
    uint x = QDateTime::currentDateTime().toTime_t();
    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(x, x+100);
    ui->grapher_widget->set_axis_scale_y(-100, 100);
#elif USE_SCALE_POINT_TIME
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

    //grapher->set_legend_is_visible(false);
    for(int n=0; n<MAX_CHANNELS; n++)
    {
        ui->grapher_widget->add_curve(QString(tr("curve %1")).arg(n));
    }
    ui->grapher_widget->legends_all_on();
    //---
    ui->cb_type_curve->clear();
    ui->cb_type_curve->addItem("точечный", DOTS);
    ui->cb_type_curve->addItem("линейный", LINES);
    ui->cb_type_curve->addItem("линейный (сглаживание)", SPLINE_LINES);

    connect(ui->cb_type_curve, SIGNAL(currentIndexChanged(int)),
            this,   SLOT(grapher_refresh()));

    //---
    grapher_refresh();
}
//--------------------------------------------------------------------------------
void MainBox::grapher_refresh(void)
{
    int type = ui->cb_type_curve->itemData(ui->cb_type_curve->currentIndex()).toInt();
    switch(type)
    {
    case DOTS:
        for(int n=0; n<ui->grapher_widget->get_curves_count(); n++)
        {
            QwtSymbol *symbol = new QwtSymbol();
            symbol->setStyle(QwtSymbol::Ellipse);
            symbol->setPen(ui->grapher_widget->get_curve_color(n));
            symbol->setSize(4);    //TODO

            ui->grapher_widget->set_curve_symbol(n, symbol);
            ui->grapher_widget->set_curve_style(n, QwtPlotCurve::Dots);
            ui->grapher_widget->set_curve_fitter(n, 0);
        }
        ui->grapher_widget->updateGraphics();

        emit debug("set_curve_style(curve, QwtPlotCurve::Dots);");
        break;

    case LINES:
        for(int n=0; n<ui->grapher_widget->get_curves_count(); n++)
        {
            ui->grapher_widget->set_curve_symbol(n, 0);
            ui->grapher_widget->set_curve_style(n, QwtPlotCurve::Lines);
            ui->grapher_widget->set_curve_fitter(n, 0);
        }
        ui->grapher_widget->updateGraphics();

        emit debug("set_curve_style(curve, QwtPlotCurve::Lines);");
        break;

    case SPLINE_LINES:
        for(int n=0; n<ui->grapher_widget->get_curves_count(); n++)
        {
            QwtSplineCurveFitter *fitter=new QwtSplineCurveFitter;
            fitter->setFitMode(QwtSplineCurveFitter::Spline);

            ui->grapher_widget->set_curve_symbol(n, 0);
            ui->grapher_widget->set_curve_style(n, QwtPlotCurve::Lines);

            ui->grapher_widget->set_curve_attribute(n, QwtPlotCurve::Fitted);
            ui->grapher_widget->set_curve_fitter(n, fitter);
        }
        ui->grapher_widget->updateGraphics();

        emit debug("spline");
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    QToolButton *btn_test_2 = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "test2",
                                         "test2");
    QToolButton *btn_test_3 = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "test3",
                                         "test3");
    QToolButton *btn_test_4 = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "test4",
                                         "test4");
    QToolButton *btn_test_5 = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
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
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    block_interface(true);

#if 0
    for(int n=0; n<MAX_CHANNELS; n++)
    {
        ui->grapher_widget->set_curve_color(n, QColor(Qt::blue));
    }
#endif
#if 1
    ui->grapher_widget->test();
    grapher_refresh();
#endif

    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    block_interface(true);
    ui->grapher_widget->test_sinus();
    grapher_refresh();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test3(void)
{
    block_interface(true);
    ui->grapher_widget->test_single_sinus();
    grapher_refresh();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test4(void)
{
    block_interface(true);
    ui->grapher_widget->setAxisScaleDraw(QwtPlot::xBottom, new MyScaleDraw(100.0f));
    grapher_refresh();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::test5(void)
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
        ui->grapher_widget->add_curve_data(0, n, buf[n]);
    }
    grapher_refresh();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
