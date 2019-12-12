/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "ui_oscilloscopebox.h"
//--------------------------------------------------------------------------------
#include "oscilloscopedata.hpp"
#include "oscilloscopebox.hpp"
#include "mainwindow.hpp"
#include "csvreader.hpp"
//--------------------------------------------------------------------------------
#include "oscilloscopebox_controls.hpp"
//--------------------------------------------------------------------------------
OscilloscopeBox::OscilloscopeBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::OscilloscopeBox)
{
    init();
}
//--------------------------------------------------------------------------------
int OscilloscopeBox::add_curve(int index_curve,
                               int pos_y,
                               const QColor &color)
{
    OSCILLOSCOPE_CURVE curve;
    this->color = color;

    QVector<QPointF> samples;
    for(int n=0; n<axis_X_max; n++)
    {
        samples.append(QPointF(n, pos_y));
    }

    curve.data_curve = new CurveData();
    curve.data_curve->setSamples(samples);
    curve.title = QString("%1").arg(index_curve);
    curve.color = color;

    curve.curve_ID = curves.count();

    curve.correction_multiply = 1.0f;
    curve.correction_pos_y = pos_y;
    curve.last_value = 0;

    curve.index_curve = index_curve;
    curve.plot_curve = new QwtPlotCurve();
    curve.plot_curve->setPen(color);
    curve.plot_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve.plot_curve->attach(ui->qwtPlot);
    curve.plot_curve->setData(curve.data_curve);

    showCurve(curve.plot_curve, true);

    curves.append(curve);

    ui->controls_widget->add_control(color, curve.title);

    updateText();
    return curves.count() - 1;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::set_color(int index)
{
    //emit debug(QString("index = %1").arg(index));
    QColor color = curves[index].color;

    QColorDialog *dlg = new QColorDialog();
    dlg->setCurrentColor(color);

    int btn = dlg->exec();
    if(btn == QColorDialog::Accepted)
    {

    }
}
//--------------------------------------------------------------------------------
QVariant OscilloscopeBox::itemToInfo(QwtPlotItem *plotItem) const
{
    QVariant itemInfo;
    itemInfo.setValue(plotItem);

    return itemInfo;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::showCurve(QwtPlotItem *item, bool on)
{
    if(legend == nullptr)
    {
        return;
    }

    item->setVisible(on);

    QList<QWidget *> legendWidgets = legend->legendWidgets(itemToInfo(item));

    if(legendWidgets.size() == 1)
    {
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>(legendWidgets[0]);
        if(legendLabel)
        {
            legendLabel->setChecked(on);
        }
    }

    updateGraphics();
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::create_widgets(void)
{
    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->attach(ui->qwtPlot);

#ifndef GRAPHER_NOT_PANNING
    // panning with the left mouse button
    plot_panner = new QwtPlotPanner(ui->qwtPlot->canvas());
#endif

#ifndef GRAPHER_NOT_ZOOM
    // zoom in/out with the wheel
    plot_magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
#endif

    legend = new QwtLegend();
    if(legend_is_visible)
    {
        legend->setDefaultItemMode(QwtLegendData::Checkable);

        ui->qwtPlot->insertLegend(legend, QwtPlot::RightLegend);
        connect(legend, SIGNAL(checked(QVariant, bool, int)), this, SLOT(legend_checked(QVariant, bool)));
    }

    d_picker = new QwtPlotPicker(QwtPlot::xBottom,
                                 QwtPlot::yLeft,
                                 QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::AlwaysOn,
                                 ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::blue));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

    if(title.isEmpty() == false)
        ui->qwtPlot->setTitle(title);

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   axis_Y_min, axis_Y_max);

    if(title_axis_X.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title_axis_X);

    if(title_axis_Y.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title_axis_Y);

    ui->knob_position->setUpperBound(1000);
    ui->knob_multiply->setUpperBound(2);

    //---
    connect(ui->btn_RUN,    SIGNAL(clicked(bool)),  this,   SLOT(click_RUN()));

    connect(ui->knob_position,  SIGNAL(valueChanged(double)),  this,   SLOT(position_changed(double)));
    connect(ui->knob_multiply,  SIGNAL(valueChanged(double)),  this,   SLOT(multiply_changed(double)));

    connect(ui->controls_widget,    SIGNAL(s_select(bool)), this,   SLOT(click(bool)));
    connect(ui->controls_widget,    SIGNAL(s_color(int)), this,   SLOT(set_color(int)));
    //---
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::click(bool state)
{
    emit trace(Q_FUNC_INFO);

    int index = ui->controls_widget->get_active_index();
    emit debug(QString("index %1").arg(index));

    click_channel(index, state);
    emit debug(state ? "ON" : "OFF");
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::click_channel(int channel, bool state)
{
    current_channel = channel;

    ui->knob_position->setEnabled(state);
    ui->knob_position->setValue(static_cast<double>(curves[current_channel].correction_pos_y));

    ui->knob_multiply->setEnabled(state);
    ui->knob_multiply->setValue(static_cast<double>(curves[current_channel].correction_multiply));

    legend_state(channel, state);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::click_RUN(void)
{
    state_RUN = !state_RUN;
    if(state_RUN)
    {
        timer->start();
    }
    else
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::position_changed(double value)
{
    if(current_channel < 0)
    {
        emit debug("current_channel < 0");
        return;
    }
    if(current_channel >= num_curves)
    {
        emit debug("current_channel >= num_curves");
        return;
    }
    curves[current_channel].correction_pos_y = static_cast<float>(value);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::multiply_changed(double value)
{
    if(current_channel < 0)
    {
        emit debug("current_channel < 0");
        return;
    }
    if(current_channel >= num_curves)
    {
        emit debug("current_channel >= num_curves");
        return;
    }
    curves[current_channel].correction_multiply = static_cast<float>(value);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::legend_checked(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *item = itemInfo.value<QwtPlotItem *>();
    item->setVisible(on);
    updateGraphics();
}
//--------------------------------------------------------------------------------
OscilloscopeBox::~OscilloscopeBox()
{
    curves.clear();

    delete ui;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::init()
{
    ui->setupUi(this);

    num_curves = 5;

    axis_X_min = 0;
    axis_X_max = 1000;
    axis_Y_min = 0;
    axis_Y_max = 1000;

    title = "oscilloscope";
    title_axis_X = "time";
    title_axis_Y = "voltage";

    create_widgets();
    create_timer();
    create_curves();

    set_legend_is_visible(false);
    set_zoom(false, false);
    set_panning(false, false);

    updateText();
    updateGraphics();
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::create_curves(void)
{
    QList<QColor> l_colors;
    l_colors << QColor(Qt::red);
    l_colors << QColor(Qt::green);
    l_colors << QColor(Qt::blue);
    l_colors << QColor(Qt::magenta);
    l_colors << QColor(Qt::cyan);
    l_colors << QColor(Qt::yellow);
    l_colors << QColor(Qt::darkRed);
    l_colors << QColor(Qt::darkGreen);
    l_colors << QColor(Qt::darkBlue);
    l_colors << QColor(Qt::darkMagenta);
    l_colors << QColor(Qt::darkCyan);
    l_colors << QColor(Qt::darkYellow);
    l_colors << QColor(Qt::white);
    l_colors << QColor(Qt::lightGray);
    l_colors << QColor(Qt::gray);
    l_colors << QColor(Qt::darkGray);

    int step = static_cast<int>(axis_Y_max / num_curves / 2);
    int y = step * num_curves * 2 - step;
    //qDebug() << num_curves;
    int index = 0;
    for(int n=0; n<num_curves; n++)
    {
        if(index >= l_colors.count())
        {
            index = 0;
        }
        add_curve(n, y, l_colors.at(index++));
        y-=(step*2);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::create_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::update(void)
{
    //int cnt = 0;
    for(int channel=0; channel<num_curves; channel++)
    {
        QPointF point;
        point.setX(axis_X_max);
        point.setY(static_cast<qreal>(curves[static_cast<int>(channel)].last_value));
        curves[static_cast<int>(channel)].last_value = curves[static_cast<int>(channel)].correction_pos_y;

        QVector<QPointF> samples = curves[static_cast<int>(channel)].data_curve->samples();
        samples.remove(0);
        samples.append(point);
        int n = 0;
        foreach (QPointF point, samples)
        {
            point.setX(n);
            samples.replace(n, point);
            n++;
            //cnt++;
        }
        curves[static_cast<int>(channel)].data_curve->setSamples(samples);
    }
    //emit info(QString("cnt %1").arg(cnt));    // 4000
    updateGraphics();
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::add_curve_data(int channel,
                                     qreal data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }

    data *= static_cast<qreal>(curves[channel].correction_multiply);
    data += static_cast<qreal>(curves[channel].correction_pos_y);
    curves[channel].last_value = static_cast<float>(data);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::updateGraphics(void)
{
    ui->qwtPlot->replot();
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::updateText()
{
    ui->retranslateUi(this);

    for(int n=0; n<curves.count(); n++)
    {
        curves.at(n).plot_curve->setTitle(curves.at(n).title);
    }
}
//--------------------------------------------------------------------------------
bool OscilloscopeBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void OscilloscopeBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
void OscilloscopeBox::test(void)
{

}
//--------------------------------------------------------------------------------
bool OscilloscopeBox::set_curve_color(int index, QColor color)
{
    if(index < 0)
    {
        emit error("index too small");
        return false;
    }
    if(index >= curves.count())
    {
        emit error("index too large 0");
        return false;
    }
    curves[index].color = color;
    curves[index].plot_curve->setPen(color);
    return true;
}
//--------------------------------------------------------------------------------
bool OscilloscopeBox::get_curve_color(int index, QColor *color)
{
    if(index < 0)
    {
        emit error("index too small");
        return false;
    }
    if(index >= curves.count())
    {
        emit error("index too large 0");
        return false;
    }
    *color = curves[index].color;
    return true;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::set_zoom(bool x_state, bool y_state)
{
#ifndef GRAPHER_NOT_ZOOM
    if(plot_magnifier == nullptr)
    {
        return;
    }
    plot_magnifier->setAxisEnabled(QwtPlot::yLeft, y_state);
    plot_magnifier->setAxisEnabled(QwtPlot::xBottom, x_state);
#else
    Q_UNUSED(x_state)
    Q_UNUSED(y_state)
#endif
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::set_panning(bool x_state, bool y_state)
{
#ifndef GRAPHER_NOT_PANNING
    if(plot_panner == nullptr)
    {
        return;
    }
    plot_panner->setAxisEnabled(QwtPlot::yLeft, y_state);
    plot_panner->setAxisEnabled(QwtPlot::xBottom, x_state);
#else
    Q_UNUSED(x_state)
    Q_UNUSED(y_state)
#endif
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::legend_state(int curve_ID, bool state)
{
    foreach(OSCILLOSCOPE_CURVE curve, curves)
    {
        if(curve.curve_ID == curve_ID)
        {
            showCurve(curve.plot_curve, state);
        }
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::set_legend_is_visible(bool state)
{
    legend_is_visible = state;
}
//--------------------------------------------------------------------------------
