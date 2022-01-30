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
#include "ui_grapherbox.h"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#ifdef USE_SCALE_POINT_DATETIME
class TimeScaleDraw: public QwtDateScaleDraw
{
public:
    TimeScaleDraw(const QDateTime &base):
        baseTime(base)
    {
        // длина меток в пикселях
        //setTickLength(QwtScaleDiv::MajorTick, 7);
        //setTickLength(QwtScaleDiv::MinorTick, 24);
        //setTickLength(QwtScaleDiv::MediumTick, 0);

        setDateFormat(QwtDate::Day, QString("dd.MM.yyyy hh:mm:ss"));
    }
    virtual QwtText label(qreal v) const
    {
        QDateTime upDateTime;
        upDateTime.setTime_t(v);
#ifdef USE_SCALE_POINT_DATETIME_FULL
        return upDateTime.toString("dd.MM.yyyy hh:mm:ss");
#else
        return upDateTime.toString("dd.MM.yyyy");
#endif
    }

private:
    QDateTime baseTime;
};

class PlotPicker: public QwtPlotPicker
{
public:
    explicit PlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget * wgt)
        :QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, wgt)
    {
    }

    QwtText trackerText(const QPoint &point) const
    {
        QwtText text;
        QDateTime dt;
        dt.setTime_t(invTransform(point).x());
        text.setText(QString("%1 | %2")
                     .arg(dt.toString("dd.MM.yyyy hh:mm:ss"))
                     .arg(invTransform(point).y()));
        return text;
    }
};

#elif defined(USE_SCALE_POINT_TIME)
class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime &base):
        baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        int hour = static_cast<int>(v / 3600);
        int minute = static_cast<int>((v - hour * 3600) / 60);
        int sec = static_cast<int>(v) % 60;
        QTime upTime(hour, minute, sec);
        return upTime.toString();
    }
private:
    QTime baseTime;
};

class PlotPicker: public QwtPlotPicker
{
public:
    explicit PlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget * wgt)
        :QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, wgt)
    {
    }

    QwtText trackerText(const QPoint &point) const
    {
        QwtText text;
        QTime time(0,0);
        QTime temp = time.addSecs(static_cast<int>(invTransform(point).x()));
        text.setText(QString("%1 %2")
                     .arg(temp.toString())
                     .arg(invTransform(point).y()));
        return text;
    }
};
#endif
//--------------------------------------------------------------------------------
GrapherBox::GrapherBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::GrapherBox)
{
    is_silence = false;

    axis_X_min = -100;
    axis_X_max = 100;
    axis_Y_min = -100;
    axis_Y_max = 100;

    title = "title";
    title_axis_X = "X";
    title_axis_Y = "Y";

    init();
}
//--------------------------------------------------------------------------------
QColor GrapherBox::get_curve_color(int channel)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return Qt::black;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return Qt::black;
    }
    return curves[channel].color;
}
//--------------------------------------------------------------------------------
bool GrapherBox::set_curve_color(int channel, QColor color)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    curves[channel].plot_curve->setPen(color);
    curves[channel].color = color;
    updateGraphics();
    return true;
}
//--------------------------------------------------------------------------------
void GrapherBox::set_curve_symbol(int channel,
                                  QwtSymbol *symbol)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }
    curves[channel].plot_curve->setSymbol(symbol);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_curve_style(int channel,
                                 QwtPlotCurve::CurveStyle style)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }
    curves[channel].plot_curve->setStyle(style);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_curve_attribute(int channel,
                                     QwtPlotCurve::CurveAttribute attribute)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }
    curves[channel].plot_curve->setCurveAttribute(attribute);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_curve_fitter(int channel,
                                  QwtSplineCurveFitter *fitter)
{
    if(channel < 0)
    {
        emit error("channel < 0");
        return;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }
    curves[channel].plot_curve->setCurveFitter(fitter);
    updateGraphics();
}
//--------------------------------------------------------------------------------
int GrapherBox::add_curve(const QString &title,
                          QwtSymbol *symbol)
{
    GRAPHER_CURVE curve;

    if(curves.count() >= MAX_CHANNELS)
    {
        emit error(QString("curves.count() %1 > %2").arg(curves.count()).arg(MAX_CHANNELS));
        return -1;
    }

    curve.is_active = true;
    curve.title = title;
    // emit debug(QString("curves.count = %1").arg(curves.count()));
    curve.color = curve_colors[curves.count()];
#ifdef USE_SCALE_POINT_DATETIME
#elif defined(USE_SCALE_POINT_TIME)
#else
    curve.pos_x = 0;
#endif
    curve.view_curve = new CurveData();

    curve.plot_curve = new QwtPlotCurve();

    curve.plot_curve->setPen(curve.color);
    curve.plot_curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve.plot_curve->attach(ui->qwtPlot);
    curve.plot_curve->setData(curve.view_curve);

    curve.curve_ID = curves.count();

    showCurve(curve.plot_curve, true);

    if(symbol)
    {
        curve.symbol_is_active = true;
        curve.symbol_style = symbol->style();
        curve.symbol_brush = symbol->brush();
        curve.symbol_pen = symbol->pen();
        curve.symbol_size = symbol->size();

        curve.plot_curve->setSymbol(symbol);
    }

    curves.append(curve);
    updateText();

    //TODO надо как-то умнее
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max+0.01);

    updateGraphics();
    return curve.curve_ID;
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve(const QString &title,
                           int curve_ID)
{
    if(curves.count() >= MAX_CHANNELS)
    {
        emit error(QString("curves.count() %1 > %2").arg(curves.count()).arg(MAX_CHANNELS));
        return false;
    }

    foreach (GRAPHER_CURVE curve, curves)
    {
        if(curve.curve_ID == curve_ID)
        {
            return false;
        }
    }

    GRAPHER_CURVE curve;

    curve.is_active = true;
    curve.title = title;
    curve.color = curve_colors[curves.count()];
#ifdef USE_SCALE_POINT_DATETIME
#elif defined(USE_SCALE_POINT_TIME)
#else
    curve.pos_x = 0;
#endif
    curve.view_curve = new CurveData();

    curve.plot_curve = new QwtPlotCurve();

    curve.plot_curve->setPen(curve.color);
    curve.plot_curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve.plot_curve->attach(ui->qwtPlot);
    curve.plot_curve->setData(curve.view_curve);

    curve.symbol_is_active = false;
    curve.curve_ID = curve_ID;

    curves.append(curve);
    showCurve(curve.plot_curve, true);
    updateText();

    return true;
}
//--------------------------------------------------------------------------------
void GrapherBox::remove_curve(int curve_ID)
{
    for(int n=0; n<curves.size(); n++)
    {
        GRAPHER_CURVE curve = curves[n];
        //emit info(QString("%1 %2").arg(curve.title).arg(curve.curve_ID));
        if(curve.curve_ID == curve_ID)
        {
            //emit error(QString("delete curve [%1]").arg(curve.title));
            delete curves[n].plot_curve;
            curves.remove(n);
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::remove_all_curve(void)
{
    while(curves.size())
    {
        delete curves[0].plot_curve;
        curves.remove(0);
    }
}
//--------------------------------------------------------------------------------
int GrapherBox::get_curves_count(void)
{
    return curves.count();
}
//--------------------------------------------------------------------------------
QVariant GrapherBox::itemToInfo(QwtPlotItem *plotItem) const
{
    Q_ASSERT(plotItem);

    QVariant itemInfo;
    //qVariantSetValue(itemInfo, plotItem); //TODO deprecated
    itemInfo.setValue(plotItem);

    return itemInfo;
}
//--------------------------------------------------------------------------------
void GrapherBox::showCurve(QwtPlotItem *item, bool on)
{
    Q_ASSERT(item);

    item->setVisible(on);

    // emit debug(QString("showCurve: legend_is_visible is %1").arg(legend_is_visible ? "true" : "false"));
    if(legend_is_visible)
    {
        QList<QWidget *> legendWidgets = legend->legendWidgets(itemToInfo(item));

        if(legendWidgets.size() == 1)
        {
            // QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>(legendWidgets.at(0));

            QWidget *lw = legendWidgets.at(0);
            if(lw)
            {
                QwtLegendLabel *legendLabel = reinterpret_cast<QwtLegendLabel *>(lw);

                if(legendLabel)
                {
                    legendLabel->setChecked(on);
                }
                else
                {
                    emit error("legendLabel not found");
                }
            }
        }
    }

    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::create_widgets(void)
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

    // emit debug(QString("create_widgets: legend_is_visible is %1").arg(legend_is_visible ? "true" : "false"));
    if(legend_is_visible)
    {
        legend = new QwtLegend(this);
        legend->setDefaultItemMode(QwtLegendData::Checkable);

        ui->qwtPlot->insertLegend( legend, QwtPlot::RightLegend );
        connect(legend, SIGNAL(checked(QVariant, bool, int)),
                this,   SLOT(legend_checked(QVariant, bool)));
    }

#ifdef USE_SCALE_POINT_DATETIME
    d_picker = new PlotPicker(QwtPlot::xBottom,
                              QwtPlot::yLeft,
                              QwtPlotPicker::CrossRubberBand,
                              QwtPicker::AlwaysOn,
                              ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::blue));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

#elif defined(USE_SCALE_POINT_TIME)
    d_picker = new PlotPicker(QwtPlot::xBottom,
                              QwtPlot::yLeft,
                              QwtPlotPicker::CrossRubberBand,
                              QwtPicker::AlwaysOn,
                              ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::blue));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

#else
    d_picker = new QwtPlotPicker(QwtPlot::xBottom,
                                 QwtPlot::yLeft,
                                 QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::AlwaysOn,
                                 ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::blue));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());
#endif

    //TODO цвет фона
    ui->qwtPlot->setCanvasBackground( Qt::white );

#ifdef USE_SCALE_POINT_DATETIME
    ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(QDateTime::currentDateTime()));
#elif defined(USE_SCALE_POINT_TIME)
    ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(QTime::currentTime()));
#else
#endif

    connect(ui->btn_all_on,     SIGNAL(clicked(bool)),  this, SLOT(legends_all_on()));
    connect(ui->btn_all_off,    SIGNAL(clicked(bool)),  this, SLOT(legends_all_off()));

    connect(ui->btn_Options,    SIGNAL(clicked()), this, SLOT(options()));
    connect(ui->btn_Clear,      SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_Load,       SIGNAL(clicked()), this, SLOT(load_curves()));
    connect(ui->btn_Save,       SIGNAL(clicked()), this, SLOT(save_curves()));

    connect(ui->btn_Horizontal, SIGNAL(toggled(bool)), this, SLOT(set_horizontal_alignment(bool)));
    connect(ui->btn_Vertical,   SIGNAL(toggled(bool)), this, SLOT(set_vertical_alignment(bool)));

    connect(ui->btn_autoscroll, SIGNAL(toggled(bool)), this, SLOT(set_autoscroll(bool)));

    connect(ui->btn_Statistic,  SIGNAL(clicked()), this, SLOT(statistic()));

#ifdef CONTEXT_MENU
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup(QPoint)));
#endif

    ui->btn_all_on->setToolTip(tr("Все графики включены"));
    ui->btn_all_off->setToolTip(tr("Все графики выключены"));

    ui->btn_Horizontal->setToolTip(tr("Вместить график по горизонтали"));
    ui->btn_Vertical->setToolTip(tr("Вместить график по вертикали"));
    ui->btn_Clear->setToolTip(tr("Стереть график"));
}
//--------------------------------------------------------------------------------
void GrapherBox::set_zoom(bool x_state, bool y_state)
{
#ifndef GRAPHER_NOT_ZOOM
    if(plot_magnifier == nullptr)
    {
        emit error("plot_magnifier is NULL");
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
void GrapherBox::set_panning(bool x_state, bool y_state)
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
void GrapherBox::set_axis_scale_x(qreal new_axis_X_min, qreal new_axis_X_max)
{
    //    emit info(QString("set_axis_scale_x: %1 %2")
    //              .arg(new_axis_X_min)
    //              .arg(new_axis_X_max));
    axis_X_min = new_axis_X_min;
    axis_X_max = new_axis_X_max;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, new_axis_X_min, new_axis_X_max);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_axis_scale_y(qreal new_axis_Y_min, qreal new_axis_Y_max)
{
    //    emit info(QString("set_axis_scale_y: %1 %2")
    //              .arg(new_axis_Y_min)
    //              .arg(new_axis_Y_max));
    axis_Y_min = new_axis_Y_min;
    axis_Y_max = new_axis_Y_max;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   new_axis_Y_min, new_axis_Y_max);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::popup(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *setting_action = new QAction(qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                          QObject::tr("настройки"),
                                          this);

    popup_menu->addAction(setting_action);
    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void GrapherBox::legend_on(int curve_ID)
{
    foreach(GRAPHER_CURVE curve, curves)
    {
        if(curve.curve_ID == curve_ID)
        {
            showCurve(curve.plot_curve, true);
        }
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::legend_off(int curve_ID)
{
    foreach(GRAPHER_CURVE curve, curves)
    {
        if(curve.curve_ID == curve_ID)
        {
            showCurve(curve.plot_curve, false);
        }
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::legends_all_on(void)
{
    foreach(GRAPHER_CURVE curve, curves)
    {
        showCurve(curve.plot_curve, true);
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::legends_all_off(void)
{
    foreach(GRAPHER_CURVE curve, curves)
    {
        showCurve(curve.plot_curve, false);
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::legend_checked(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *item = itemInfo.value<QwtPlotItem *>();
    item->setVisible(on);
    set_horizontal_alignment(ui->btn_Horizontal->isChecked());
    set_vertical_alignment(ui->btn_Vertical->isChecked());
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::clear(void)
{
    if(is_silence == false)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle("Очистка кривых");
        msgBox.setText("Вы уверены, что вы хотите очистить ВСЕ кривые?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int btn = msgBox.exec();

        if(btn != QMessageBox::Yes)
        {
            emit error("Отменено");
            return;
        }
    }

    for(int n=0; n<curves.count(); n++)
    {
        curves[n].real_data.clear();
        curves[n].view_curve->clear();
        curves[n].pos_x = 0;
#ifdef USE_SCALE_POINT_DATETIME
        //nothing
#elif defined(USE_SCALE_POINT_TIME)
        //nothing
#else
#endif
        curves[n].plot_curve->setSamples(curves[n].view_curve);
    }
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::reset(void)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Удаление кривых");
    msgBox.setText("Вы уверены, что вы хотите удалить ВСЕ кривые?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int btn = msgBox.exec();

    if(btn != QMessageBox::Yes)
    {
        emit error("Отменено");
        return;
    }

    int c = curves.count();
    for(int n=0; n<c; n++)
    {
        remove_curve(0);
    }
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::click_legend(void)
{
    emit info("click_legend");

    QwtLegendLabel *w = dynamic_cast<QwtLegendLabel *>(sender());
    if(w)
    {
        emit info(QString("index [%1]").arg(w->property("index").toString()));
        qDebug() << w->property("index");
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::setAxisScaleDraw( int axisId, QwtScaleDraw *scaleDraw )
{
    Q_ASSERT(scaleDraw);
    ui->qwtPlot->setAxisScaleDraw(axisId, scaleDraw);
}
//--------------------------------------------------------------------------------
void GrapherBox::clear_d_picker(void)
{
    if(d_picker)
    {
        delete d_picker;
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::set_d_picker(QwtPlotPicker *picker)
{
    Q_ASSERT(picker);
    d_picker = picker;
}
//--------------------------------------------------------------------------------
QWidget *GrapherBox::get_qwtPlot_canvas(void)
{
    return ui->qwtPlot->canvas();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_axis_label_rotation(int angle)
{
    ui->qwtPlot->setAxisLabelRotation(QwtPlot::xBottom, angle);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_axis_label_alignment(int axisId, Qt::Alignment alignment)
{
    ui->qwtPlot->setAxisLabelAlignment(axisId, alignment);
}
//--------------------------------------------------------------------------------
GrapherBox::~GrapherBox()
{
    curves.clear();

    delete ui;
}
//--------------------------------------------------------------------------------
void GrapherBox::init()
{
    ui->setupUi(this);

    flag_symbol = false;
    curves.clear();

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

    int index = 0;
    for(int n=0; n<MAX_CHANNELS; n++)
    {
        if(index >= l_colors.count())
        {
            index = 0;
        }
        curve_colors[n] = l_colors.at(index++);
    }

    create_widgets();
    updateGraphics();
    updateText();
}
//--------------------------------------------------------------------------------
void GrapherBox::add_curve_data_points(int channel,
                                       QVector<QPointF> *points)
{
    Q_ASSERT(points);

    curves[channel].real_data = *points;
    curves[channel].view_curve->setSamples(*points);
    updateGraphics();
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_data(int channel,
                                int x,
                                qreal data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    data_channels[channel].append(QPointF(x, data));    //TODO добавление данных

    curves[channel].real_data.append(QPointF(x, data));
    curves[channel].view_curve->append(QPointF(x, data));
    curves[channel].pos_x = x;
    set_horizontal_alignment(ui->btn_Horizontal->isChecked());
    set_vertical_alignment(ui->btn_Vertical->isChecked());
    updateGraphics();

    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_data(int channel,
                                qreal x,
                                qreal y)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    data_channels[channel].append(QPointF(x, y));    //TODO добавление данных

    curves[channel].real_data.append(QPointF(x, y));
    curves[channel].view_curve->append(QPointF(x, y));
    curves[channel].pos_x = x;

    set_horizontal_alignment(ui->btn_Horizontal->isChecked());
    set_vertical_alignment(ui->btn_Vertical->isChecked());
    updateGraphics();

    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::set_curve_data(int channel,
                                int index,
                                qreal data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    if(curves[channel].real_data.isEmpty())
    {
        emit error("real_data is empty");
        return false;
    }

    QPoint point(index, data);
    curves[channel].real_data[index]  = point;
    curves[channel].view_curve[index].append(point);
    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_curve_data(int channel,
                                int index,
                                qreal *data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    if(curves[channel].real_data.isEmpty())
    {
        emit error("real_data is empty");
        return false;
    }

    QPointF temp = curves[channel].real_data.at(index);
    *data = temp.y();
    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_pos_x(int channel, long *value)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    if(curves[channel].real_data.isEmpty())
    {
        emit error("real_data is empty");
        return false;
    }

    *value = curves[channel].pos_x;
    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_curve_data_count(int channel, int *cnt)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        *cnt = 0;
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        *cnt = 0;
        return false;
    }
    if(curves[channel].real_data.isEmpty())
    {
        emit error("real_data is empty");
        *cnt = 0;
        return false;
    }

    *cnt = curves[channel].real_data.count();
    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_data(int channel,
                                qreal data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel[%1] >= curves.count[%2]"))
                   .arg(channel)
                   .arg(curves.count()));
        return false;
    }

#ifdef USE_SCALE_POINT_DATETIME
    QDateTime dt;
    dt = QDateTime::currentDateTime();
    qreal x = dt.toTime_t();
    curves[channel).real_data.append(QPointF(x, data));
    curves[channel).view_curve->append(QPointF(x, data));
    curves[channel).pos_x++;
#elif defined(USE_SCALE_POINT_TIME)
    QTime time;
    time = QTime::currentTime();
    qreal x = (time.hour() * 3600) + (time.minute() * 60) + time.second();
    curves[channel).real_data.append(QPointF(x, data));
    curves[channel).view_curve->append(QPointF(x, data));
    curves[channel).pos_x++;
#else
    curves[channel].real_data.append(QPointF(curves[channel].pos_x, data));
    curves[channel].view_curve->append(QPointF(curves[channel].pos_x, data));
    //curves[channel].plot_curve->setSamples(curves[channel].view_curve); //FIXME добавил на пробу
    curves[channel].pos_x++;
#endif

    data_channels[channel].append(QPointF(curves[channel].pos_x, data));    //TODO добавление данных

    if(flag_horizontal_alignment)
    {
        f_horizontal_alignment();
    }
    if(flag_vertical_alignment)
    {
        f_vertical_alignment();
    }

    if(flag_autoscroll)
    {
        //FIXME костыль
        if(channel == 0)
        {
            autoscroll();
        }
    }

    updateGraphics();

    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_data(int channel,
                                QDateTime v_dt,
                                qreal value)
{
    return add_curve_data(channel,
                          (int)v_dt.toSecsSinceEpoch(),
                          value);
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_data(int channel,
                                QTime v_t,
                                qreal value)
{
    return add_curve_data(channel,
                          v_t.hour() * 60 * 60 + v_t.minute() * 60 +v_t.second(),
                          value);
}
//--------------------------------------------------------------------------------
bool GrapherBox::add_curve_array(int channel,
                                 QList<QPointF> a_points)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }

#ifdef USE_SCALE_POINT_DATETIME
    foreach (QPointF point, a_points)
    {
        curves[channel].real_data.append(point);
        curves[channel].view_curve->append(point);
        curves[channel].pos_x++;
    }
#elif defined(USE_SCALE_POINT_TIME)
    foreach (QPointF point, a_points)
    {
        curves[channel].real_data.append(point);
        curves[channel].view_curve->append(point);
        curves[channel].pos_x++;
    }
#else
    foreach (QPointF point, a_points)
    {
        curves[channel].real_data.append(point);
        curves[channel].view_curve->append(point);
        curves[channel].pos_x++;
    }
#endif
    set_horizontal_alignment(ui->btn_Horizontal->isChecked());
    set_vertical_alignment(ui->btn_Vertical->isChecked());

    if(flag_autoscroll)
    {
        //FIXME костыль
        if(channel == 0)
        {
            autoscroll();
        }
    }

    updateGraphics();

    return true;
}
//--------------------------------------------------------------------------------
void GrapherBox::updateGraphics(void)
{
    ui->qwtPlot->replot();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_device_RECORDER(void)
{
    flag_device_RECORDER = true;
    flag_device_OSCILLOSCOPE = false;
}
//--------------------------------------------------------------------------------
void GrapherBox::set_device_OSCILLOSCOPE(void)
{
    flag_device_RECORDER = false;
    flag_device_OSCILLOSCOPE = true;
}
//--------------------------------------------------------------------------------
void GrapherBox::set_type_curve_DOTS(void)
{
    flag_type_curve_DOTS = true;
    flag_type_curve_LINES = false;
    flag_type_curve_SPLINE_LINES = false;

    bool flag_v = get_vertical_alignment();
    bool flag_h = get_horizontal_alignment();

    for(int n=0; n<get_curves_count(); n++)
    {
        QwtSymbol *symbol = new QwtSymbol();
        symbol->setStyle(QwtSymbol::Ellipse);
        symbol->setPen(get_curve_color(n));
        symbol->setSize(4);    //TODO

        set_curve_symbol(n, symbol);
        set_curve_style(n, QwtPlotCurve::Dots);
        set_curve_fitter(n, nullptr);
    }
    updateGraphics();

    set_vertical_alignment(flag_v);
    set_horizontal_alignment(flag_h);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_type_curve_LINES(void)
{
    flag_type_curve_DOTS = false;
    flag_type_curve_LINES = true;
    flag_type_curve_SPLINE_LINES = false;

    bool flag_v = get_vertical_alignment();
    bool flag_h = get_horizontal_alignment();

    for(int n=0; n<get_curves_count(); n++)
    {
        set_curve_symbol(n, nullptr);
        set_curve_style(n, QwtPlotCurve::Lines);
        set_curve_fitter(n, nullptr);
    }
    updateGraphics();

    set_vertical_alignment(flag_v);
    set_horizontal_alignment(flag_h);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_type_curve_SPLINE_LINES(void)
{
    flag_type_curve_DOTS = false;
    flag_type_curve_LINES = false;
    flag_type_curve_SPLINE_LINES = true;

    bool flag_v = get_vertical_alignment();
    bool flag_h = get_horizontal_alignment();

    for(int n=0; n<get_curves_count(); n++)
    {
        QwtSplineCurveFitter *fitter = new QwtSplineCurveFitter();
        //fitter->setFitMode(QwtSplineCurveFitter::Spline); //FIXME возможно, что проблема

        set_curve_symbol(n, nullptr);
        set_curve_style(n, QwtPlotCurve::Lines);

        set_curve_attribute(n, QwtPlotCurve::Fitted);
        set_curve_fitter(n, fitter);
    }
    updateGraphics();

    set_vertical_alignment(flag_v);
    set_horizontal_alignment(flag_h);
}
//--------------------------------------------------------------------------------
void GrapherBox::options(void)
{
    GrapherBox_Options *dlg = new GrapherBox_Options();
    dlg->set_min_axis_X(static_cast<int>(axis_X_min));
    dlg->set_max_axis_X(static_cast<int>(axis_X_max));
    dlg->set_min_axis_Y(static_cast<int>(axis_Y_min));
    dlg->set_max_axis_Y(static_cast<int>(axis_Y_max));
    if(flag_device_RECORDER)
    {
        dlg->set_type_device(GrapherBox_Options::RECORDER);
    }
    if(flag_device_OSCILLOSCOPE)
    {
        dlg->set_type_device(GrapherBox_Options::OSCILLOSCOPE);
    }
    if(flag_type_curve_DOTS)
    {
        dlg->set_type_curve(GrapherBox_Options::DOTS);
    }
    if(flag_type_curve_LINES)
    {
        dlg->set_type_curve(GrapherBox_Options::LINES);
    }
    if(flag_type_curve_SPLINE_LINES)
    {
        dlg->set_type_curve(GrapherBox_Options::SPLINE_LINES);
    }
    //---
    for(int n=0; n<curves.count(); n++)
    {
        dlg->add_color_button(curves[n].color, curves[n].title);
    }
    //---

    int button = dlg->exec();
    if(button == QDialog::Accepted)
    {
        GrapherBox_Options::Device device = dlg->get_type_device();
        switch (device)
        {
        case GrapherBox_Options::RECORDER:
            set_device_RECORDER();
            break;

        case GrapherBox_Options::OSCILLOSCOPE:
            set_device_OSCILLOSCOPE();
            break;
        }

        GrapherBox_Options::TypeCurve type = dlg->get_type_curve();
        switch(type)
        {
        case GrapherBox_Options::DOTS:
            set_type_curve_DOTS();
            break;

        case GrapherBox_Options::LINES:
            set_type_curve_LINES();
            break;

        case GrapherBox_Options::SPLINE_LINES:
            set_type_curve_SPLINE_LINES();
            break;
        }

        axis_X_min = dlg->get_min_axis_x();
        axis_X_max = dlg->get_max_axis_x();
        axis_Y_min = dlg->get_min_axis_y();
        axis_Y_max = dlg->get_max_axis_y();
        ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
        ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   axis_Y_min, axis_Y_max);

        //---
        for(int n=0; n<curves.count(); n++)
        {
            QColor color;
            QString text;
            bool ok = dlg->get_color(n, &color);
            if(ok)
            {
                curves[n].color = color;
                curves[n].plot_curve->setPen(color);
            }
            ok = dlg->get_text(n, &text);
            if(ok)
            {
                curves[n].title = text;
                curves[n].plot_curve->setTitle(text);
                emit change_text(n, text);
                //FIXME надо доделать
            }
        }
        //---

        updateGraphics();
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::load_curves(void)
{
    QPointer<MyFileDialog> dlg;

    dlg = new MyFileDialog("dlg_grapherbox", "dlg_grapherbox");
    dlg->setNameFilter("CSV files (*.csv)");
    dlg->setDefaultSuffix("csv");
#ifdef Q_OS_LINUX
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    dlg->setFileMode(QFileDialog::ExistingFile);
    dlg->setDirectory(".");

    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        f_load_curves(filename);

        updateGraphics();
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::save_curves(void)
{
    if(curves.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Нет данных (curves)");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    for(int n=0; n<curves.count(); n++)
    {
        if(curves[n].view_curve->samples().size() == 0)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Ошибка");
            msgBox.setText("Нет данных (sample)");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
    }

    //записываем файл
    QPointer<MyFileDialog> dlg = new MyFileDialog("dlg_grapherbox", "dlg_grapherbox");
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter("CSV files (*.csv)");
    dlg->setDefaultSuffix("csv");
#ifdef Q_OS_LINUX
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
#endif
    dlg->setDirectory(".");
    dlg->selectFile(QString("%1")
                    .arg(curves[0].title));
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        f_save_curves(filename);
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::f_load_curves(QString filename)
{
    if(filename.isEmpty())
    {
        emit error("Filename is empty!");
        return;
    }
    QFile file(filename);
    CsvReader *csv = new CsvReader(nullptr ,filename);
    csv->set_new_separator('|');
    if(csv->Open())
    {
        for(int n=0; n<curves.count(); n++)
        {
            curves[n].real_data.clear();
            curves[n].view_curve->clear();
            curves[n].pos_x = 0;
        }
        QList<QStringList> str = csv->CSVRead();
        foreach (QStringList sl, str)
        {
#ifdef USE_SCALE_POINT_DATETIME
            if(sl.count() == 8)
            {
                bool ok = false;
                int channel = sl.at(0).toInt(&ok);

                int year    = sl.at(1).toInt(&ok);
                int month   = sl.at(2).toInt(&ok);
                int day     = sl.at(3).toInt(&ok);

                int hour    = sl.at(4).toInt(&ok);
                int minute  = sl.at(5).toInt(&ok);
                int second  = sl.at(6).toInt(&ok);

                qreal value = sl.at(7).toDouble(&ok);

                QDate date;
                date.setDate(year, month, day);

                QTime time;
                time.setHMS(hour, minute, second);

                QDateTime dt;
                dt.setDate(date);
                dt.setTime(time);

                curves[channel).real_data.append(QPointF(dt.toSecsSinceEpoch(), value));
                curves[channel).view_curve->append(QPointF(dt.toSecsSinceEpoch(), value));
                curves[channel).pos_x++;
#elif defined(USE_SCALE_POINT_TIME)
            if(sl.count() == 5)
            {
                bool ok = false;
                int channel = sl.at(0).toInt(&ok);

                int hour    = sl.at(1).toInt(&ok);
                int minute  = sl.at(2).toInt(&ok);
                int second  = sl.at(3).toInt(&ok);

                qreal value = sl.at(4).toDouble(&ok);

                QTime time;
                time.setHMS(hour, minute, second);

                curves[channel).real_data.append(QPointF(hour * 60 * 60 + minute * 60 + second, value));
                curves[channel).view_curve->append(QPointF(hour * 60 * 60 + minute * 60 + second, value));
                curves[channel).pos_x++;
#else
            if(sl.count() == 3)
            {
                bool ok = false;
                int channel = sl.at(0).toInt(&ok);
                int pos = sl.at(1).toInt(&ok);
                qreal value = sl.at(2).toDouble(&ok);

                curves[channel].real_data.append(QPointF(pos, value));
                curves[channel].view_curve->append(QPointF(pos, value));
                curves[channel].pos_x++;
#endif
            }
            else
            {
                emit error(QString("error data: cnt %1").arg(sl.count()));
                break;
            }

        }
    }
    file.close();
}
//--------------------------------------------------------------------------------
void GrapherBox::f_save_curves(QString filename)
{
    if(filename.isEmpty())
    {
        emit error("Filename is empty!");
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit error(QString(tr("file %1 not writed!"))
                   .arg(filename));
        return;
    }

    for(int channel=0; channel<curves.count(); channel++)
    {
        for(int x=0; x<curves[channel].view_curve->samples().size(); x++)
        {
#ifdef USE_SCALE_POINT_DATETIME
            quint32 pos_date = curves[channel).view_curve->sample(static_cast<size_t>(x)).x();
            qreal   value = curves[channel).view_curve->sample(static_cast<size_t>(x)).y();

            QDateTime dt;
            dt.setTime_t(pos_date);

            int year    = dt.date().year();
            int month   = dt.date().month();
            int day     = dt.date().day();

            int hour    = dt.time().hour();
            int minute  = dt.time().minute();
            int second  = dt.time().second();

            //TODO формат channel|year|month|day|hour|minute|second|value
            QString temp = QString("%1|%2|%3|%4|%5|%6|%7|%8\n")
                    .arg(channel)
                    .arg(year)
                    .arg(month)
                    .arg(day)
                    .arg(hour)
                    .arg(minute)
                    .arg(second)
                    .arg(value);
            file.write(temp.toLocal8Bit());
#elif defined(USE_SCALE_POINT_TIME)
            quint32 pos_time = curves[channel).view_curve->sample(static_cast<size_t>(x)).x();
            qreal   value = curves[channel).view_curve->sample(static_cast<size_t>(x)).y();

            QTime temp_t;
            temp_t.setHMS(0, 0, 0, 0);
            QTime t = temp_t.addSecs(pos_time);

            int hour    = t.hour();
            int minute  = t.minute();
            int second  = t.second();

            //TODO формат channel|minute|second|value
            QString temp = QString("%1|%2|%3|%4|%5\n")
                    .arg(channel)
                    .arg(hour)
                    .arg(minute)
                    .arg(second)
                    .arg(value);
            file.write(temp.toLocal8Bit());
#else
            QString temp = QString("%1|%2|%3\n")
                    .arg(channel)
                    .arg(curves[channel].view_curve->sample(static_cast<size_t>(x)).x())
                    .arg(curves[channel].view_curve->sample(static_cast<size_t>(x)).y());
            file.write(temp.toLocal8Bit());
#endif
        }
    }
    file.close();
    emit info(QString(tr("файл %1 записан успешно"))
              .arg(filename));

}
//--------------------------------------------------------------------------------
void GrapherBox::set_vertical_alignment(bool state)
{
    flag_vertical_alignment = state;
    if(flag_vertical_alignment)
    {
        f_vertical_alignment();
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::set_horizontal_alignment(bool state)
{
    flag_horizontal_alignment = state;
    if(flag_horizontal_alignment)
    {
        f_horizontal_alignment();
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::set_autoscroll(bool state)
{
    flag_autoscroll = state;
    axis_X_min += 1.0;
    axis_X_max += 1.0;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::autoscroll(void)
{
    axis_X_min ++;
    axis_X_max ++;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_vertical_alignment(void)
{
    return flag_vertical_alignment;
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_horizontal_alignment(void)
{
    return  flag_horizontal_alignment;
}
//--------------------------------------------------------------------------------
void GrapherBox::correct(int channel,
                         float mul)
{
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return;
    }

    CurveData temp;
    temp.clear();
    for(int x=0; x<curves[channel].view_curve->samples().size(); x++)
    {
        temp.append(QPointF(curves[channel].view_curve->sample(static_cast<size_t>(x)).x(),
                            curves[channel].view_curve->sample(static_cast<size_t>(x)).y() * static_cast<size_t>(mul)));
    }
    curves[channel].view_curve->clear();
    curves[channel].view_curve->setSamples(temp.samples());
    curves[channel].pos_x = temp.samples().count();
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::statistic(void)
{
    emit info(tr("Статистика:"));
    for(int n=0; n<curves.count(); n++)
    {
        if(curves[n].plot_curve->isVisible())
        {
            qreal max_value = INT_MIN;
            qreal min_value = INT_MAX;
            qreal value = 0;
            qreal average = 0;
            int cnt = curves[n].view_curve->samples().count();
            for(int x=0; x<cnt; x++)
            {
                value = curves[n].view_curve->sample(static_cast<size_t>(x)).y();
                average += value;
                if(value > max_value) max_value = value;
                if(value < min_value) min_value = value;
            }
            emit info(QString("   График %1, кол-во точек %2").arg(n).arg(cnt));
            if(cnt > 0)
            {
                average /= static_cast<qreal>(cnt);
                emit info(QString("     min %1").arg(min_value));
                emit info(QString("     max %1").arg(max_value));
                emit info(QString("     average %1").arg(average));
                emit info(QString("     max delta %1").arg(max_value - min_value));
            }
        }
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::set_title(const QString &title)
{
    if(title.isEmpty()) return;

    this->title = title;
    ui->qwtPlot->setTitle(title);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_title_axis_X(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_X = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_title_axis_Y(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_Y = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title);
}
//--------------------------------------------------------------------------------
void GrapherBox::check_visible_buttons(void)
{
    ui->btn_all_on->setVisible(flag_visible_btn_all_ON);
    ui->btn_all_off->setVisible(flag_visible_btn_all_OFF);
    ui->btn_Horizontal->setVisible(flag_visible_btn_Horizontal);
    ui->btn_Vertical->setVisible(flag_visible_btn_Vertical);
    ui->btn_Options->setVisible(flag_visible_btn_Options);
    ui->btn_Load->setVisible(flag_visible_btn_Load);
    ui->btn_Save->setVisible(flag_visible_btn_Save);
    ui->btn_Statistic->setVisible(flag_visible_btn_Statistic);
    ui->btn_Clear->setVisible(flag_visible_btn_Clear);
    ui->btn_autoscroll->setVisible(flag_visible_btn_Autoscroll);

    ui->frame_buttons->setVisible(
                flag_visible_btn_all_ON     ||
                flag_visible_btn_all_OFF    ||
                flag_visible_btn_Horizontal ||
                flag_visible_btn_Vertical   ||
                flag_visible_btn_Options    ||
                flag_visible_btn_Load       ||
                flag_visible_btn_Save       ||
                flag_visible_btn_Statistic  ||
                flag_visible_btn_Clear      ||
                flag_visible_btn_Autoscroll);
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_all_ON(bool state)
{
    flag_visible_btn_all_ON = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_all_OFF(bool state)
{
    flag_visible_btn_all_OFF = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Horizontal(bool state)
{
    flag_visible_btn_Horizontal = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Vertical(bool state)
{
    flag_visible_btn_Vertical = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Options(bool state)
{
    flag_visible_btn_Options = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Load(bool state)
{
    flag_visible_btn_Load = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Save(bool state)
{
    flag_visible_btn_Save = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Statistic(bool state)
{
    flag_visible_btn_Statistic = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Clear(bool state)
{
    flag_visible_btn_Clear = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::set_visible_btn_Autoscroll(bool state)
{
    flag_visible_btn_Autoscroll = state;
    check_visible_buttons();
}
//--------------------------------------------------------------------------------
void GrapherBox::push_btn_Horizontal(bool state)
{
    ui->btn_Horizontal->setChecked(state);
    set_horizontal_alignment(state);
}
//--------------------------------------------------------------------------------
void GrapherBox::push_btn_Vertical(bool state)
{
    ui->btn_Vertical->setChecked(state);
    set_vertical_alignment(state);
}
//--------------------------------------------------------------------------------
void GrapherBox::test(void)
{
    emit info("begin test");

#if 1
    axis_X_min--;
    axis_X_max--;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
    updateGraphics();
#endif

#if 0
    QwtScaleDiv div;
    div.setInterval(0, 10);
    for(int index=0; index<get_curves_count(); index++)
    {
        ui->qwtPlot->setAxisScaleDiv(index, div);
    }

    push_btn_Horizontal(false);
    push_btn_Vertical(false);
    for(int n=0; n<360; n++)
    {
        for(int index=0; index<get_curves_count(); index++)
        {
            int size_y = index*100;
            add_curve_data(index, qreal(308.0 + size_y)*qSin(qreal(n+(index * 10))*qreal(M_PI)/qreal(180.0)));
        }
    }
    push_btn_Horizontal(true);
    push_btn_Vertical(true);

    emit info(QString("curves_count %1").arg(get_curves_count()));
    emit info(QString("append %1 points").arg(360 * get_curves_count()));
#endif
    emit info("end test");
}
//--------------------------------------------------------------------------------
void GrapherBox::test2(void)
{
    emit info("begin test2");

#if 1
    axis_X_min++;
    axis_X_max++;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, axis_X_min, axis_X_max);
    updateGraphics();
#endif


#if 0
    int max_x = static_cast<int>(axis_X_max - axis_X_min);
    for(int channel=0; channel<get_curves_count(); channel++)
    {
        curves[channel).pos_x = 0;
        for(int n=0; n<max_x; n++)
        {
            curves[channel).view_curve->append(QPointF(n, channel));
            curves[channel).pos_x++;
        }
    }

    ui->btn_Horizontal->setChecked(true);
    ui->btn_Vertical->setChecked(true);
    set_horizontal_alignment(true);
    set_vertical_alignment(true);
#endif

    emit info("end test2");
}
//--------------------------------------------------------------------------------
void GrapherBox::test_get_info(void)
{
    //QwtPlotCanvas *canvas = new QwtPlotCanvas();
    //canvas->setFrameStyle(QFrame::Box | QFrame::Plain);
    //canvas->setLineWidth(1);
    //canvas->setPalette(Qt::white);
    //ui->qwtPlot->setCanvas(canvas);

    ui->qwtPlot->setCanvasBackground(QBrush(Qt::white));
    updateGraphics();

    QwtPicker::RubberBand temp = d_picker->rubberBand();
    switch(temp)
    {
    case QwtPicker::NoRubberBand:       emit info("NoRubberBand"); break;
    case QwtPicker::HLineRubberBand:    emit info("HLineRubberBand"); break;
    case QwtPicker::VLineRubberBand:    emit info("VLineRubberBand"); break;
    case QwtPicker::CrossRubberBand:    emit info("CrossRubberBand"); break;
    case QwtPicker::RectRubberBand:     emit info("RectRubberBand"); break;
    case QwtPicker::EllipseRubberBand:  emit info("EllipseRubberBand"); break;
    case QwtPicker::PolygonRubberBand:  emit info("PolygonRubberBand"); break;
    case QwtPicker::UserRubberBand:     emit info("UserRubberBand"); break;
    }

    emit info(QString("canvas %1:%2:%3")
              .arg(ui->qwtPlot->canvasBackground().color().red())
              .arg(ui->qwtPlot->canvasBackground().color().green())
              .arg(ui->qwtPlot->canvasBackground().color().blue()));
    foreach (GRAPHER_CURVE curve, curves)
    {
        //QColor color;
        //color.setRgb(255, 55, 55);
        //curve.plot_curve->setPen(color);
        emit info(QString("curve %1 %2:%3:%4")
                  .arg(curve.title)
                  .arg(curve.plot_curve->pen().color().red())
                  .arg(curve.plot_curve->pen().color().green())
                  .arg(curve.plot_curve->pen().color().blue()));
        emit info(QString("symbol %1:%2:%3")
                  .arg(curve.plot_curve->symbol()->pen().color().red())
                  .arg(curve.plot_curve->symbol()->pen().color().green())
                  .arg(curve.plot_curve->symbol()->pen().color().blue()));
    }
}
//--------------------------------------------------------------------------------
void GrapherBox::test_draw_circle(void)
{
    emit info("test_draw_circle begin");
    curves[0].view_curve->clear();
    curves[0].pos_x = 0;

    push_btn_Horizontal(false);
    push_btn_Vertical(false);
    for(int n=0; n<360; n++)
    {
        qreal x = qreal(154.0)*qCos(qreal(n)*qreal(M_PI)/qreal(180.0));
        qreal y = qreal(308.0)*qSin(qreal(n)*qreal(M_PI)/qreal(180.0));
        //qreal x = qCos(qreal(n)*qreal(M_PI)/qreal(180.0));
        //qreal y = qSin(qreal(n)*qreal(M_PI)/qreal(180.0));
        curves[0].view_curve->append(QPointF(x, y));
    }
    push_btn_Horizontal(true);
    push_btn_Vertical(true);

    updateGraphics();
    emit info("test_draw_circle end");
}
//--------------------------------------------------------------------------------
void GrapherBox::test_sinus(void)
{
    push_btn_Horizontal(false);
    push_btn_Vertical(false);
    for(int index=0; index<curves.count(); index++)
    {
        for(int n=0; n<360; n++)
        {
            if(!(n % 60)) QCoreApplication::processEvents();
            qreal height = qreal(axis_Y_max - axis_Y_min) * 0.5;
            qreal shift_x = index * 50;
            add_curve_data(index, height*qSin(qreal(n + shift_x)*qreal(M_PI)/qreal(180.0)));
            //qDebug() << n;
        }
    }
    push_btn_Horizontal(true);
    push_btn_Vertical(true);
}
//--------------------------------------------------------------------------------
void GrapherBox::test_single_sinus(int index, int offset)
{
    if(index < 0)
    {
        emit error("index too small");
        return;
    }
    if(index >= curves.count())
    {
        emit error("index too large");
        return;
    }
    push_btn_Horizontal(false);
    push_btn_Vertical(false);
    for(int n=0; n<360; n++)
    {
        add_curve_data(index, offset + n,  128 + 127*qSin(qreal(n)*qreal(M_PI)/qreal(180.0)));
    }
    push_btn_Horizontal(true);
    push_btn_Vertical(true);
}
//--------------------------------------------------------------------------------
void GrapherBox::test_random_data(int index, int offset)
{
    if(index < 0)
    {
        emit error("index too small");
        return;
    }
    if(index >= curves.count())
    {
        emit error("index too large");
        return;
    }
    push_btn_Horizontal(false);
    push_btn_Vertical(false);
    for(int n=0; n<360; n++)
    {
        add_curve_data(index, offset + n, rand() % 100 - 50);
    }
    push_btn_Horizontal(true);
    push_btn_Vertical(true);
}
//--------------------------------------------------------------------------------
void GrapherBox::f_vertical_alignment(void)
{
    if(curves.isEmpty())
    {
        //emit error(tr("Нет данных!"));
        return;
    }

    qreal max_y = INT_MIN;
    qreal min_y = INT_MAX;

    for(int n=0; n<curves.count(); n++)
    {
        for(int x=0; x<curves[n].view_curve->samples().size(); x++)
        {
            if(curves[n].plot_curve->isVisible())
            {
                qreal temp_y = curves[n].view_curve->sample(static_cast<size_t>(x)).y();
                if(temp_y > max_y) max_y = temp_y;
                if(temp_y < min_y) min_y = temp_y;
            }
        }
    }
    axis_Y_min = min_y;
    axis_Y_max = max_y;
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, min_y, max_y);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::f_horizontal_alignment(void)
{
    if(curves.isEmpty())
    {
        //emit error(tr("Нет данных!"));
        return;
    }

    qreal max_x = INT_MIN;
    qreal min_x = INT_MAX;

    for(int n=0; n<curves.count(); n++)
    {
        for(int x=0; x<curves[n].view_curve->samples().size(); x++)
        {
            qreal temp_x = curves[n].view_curve->sample(static_cast<size_t>(x)).x();
            if(temp_x > max_x) max_x = temp_x;
            if(temp_x < min_x) min_x = temp_x;
        }
    }
    axis_X_min = min_x;
    axis_X_max = max_x;
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, min_x, max_x);
    updateGraphics();
}
//--------------------------------------------------------------------------------
void GrapherBox::f_autoscroll(void)
{

}
//--------------------------------------------------------------------------------
void GrapherBox::set_silense(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
bool GrapherBox::get_silense(void)
{
    return is_silence;
}
//--------------------------------------------------------------------------------
void GrapherBox::set_legend_is_visible(bool state)
{
    legend_is_visible = state;
}
//--------------------------------------------------------------------------------
void GrapherBox::clicked(QVariant v, int i)
{
    qDebug() << "clicked" << v << i;
}
//--------------------------------------------------------------------------------
void GrapherBox::checked(QVariant v, bool b, int i)
{
    qDebug() << "checked" << v << b << i;
}
//--------------------------------------------------------------------------------
void GrapherBox::updateText()
{
    ui->retranslateUi(this);

    for(int n=0; n<curves.count(); n++)
    {
        curves[n].plot_curve->setTitle(curves[n].title);
    }
}
//--------------------------------------------------------------------------------
bool GrapherBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void GrapherBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void GrapherBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
