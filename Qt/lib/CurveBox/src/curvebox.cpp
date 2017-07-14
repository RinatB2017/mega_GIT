/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QPointF>
#include <QToolBar>
#include <QDialog>
#include <QLabel>

#include <QDateTime>
#include <QDate>
#include <QTime>

#include <QFile>
#include <QMenu>

#include <limits.h>
#include "defines.hpp"
//--------------------------------------------------------------------------------
#if defined(Q_CC_MSVC)
# define QT_STATIC_CONST static
# define QT_STATIC_CONST_IMPL
#else
# define QT_STATIC_CONST static const
# define QT_STATIC_CONST_IMPL const
#endif
//--------------------------------------------------------------------------------
#include <qwt_plot_renderer.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot.h>
#include <qwt_curve_fitter.h>
//--------------------------------------------------------------------------------
#include "ui_curvebox.h"
#include "mainwindow.hpp"
#include "curvedata.hpp"
#include "curvebox.hpp"
//--------------------------------------------------------------------------------
CurveBox::CurveBox(unsigned int max_axis_X,
                   double min_axis_Y,
                   double max_axis_Y,
                   const QString &title,
                   const QString &title_axis_X,
                   const QString &title_axis_Y,
                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveBox),
    is_silence(false),
    max_axis_X(max_axis_X),
    min_axis_Y(min_axis_Y),
    max_axis_Y(max_axis_Y),
    title(title),
    title_axis_X(title_axis_X),
    title_axis_Y(title_axis_Y)
{
    init();
}
//--------------------------------------------------------------------------------
QVariant CurveBox::itemToInfo(QwtPlotItem *plotItem) const
{
    QVariant itemInfo;
    qVariantSetValue(itemInfo, plotItem);

    return itemInfo;
}
//--------------------------------------------------------------------------------
void CurveBox::create_widgets(void)
{
    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->attach(ui->qwtPlot);

#if 0
    // panning with the left mouse button
    new QwtPlotPanner(ui->qwtPlot->canvas());

    // zoom in/out with the wheel
    new QwtPlotMagnifier(ui->qwtPlot->canvas());
#endif

    legend = new QwtLegend();
    legend->setDefaultItemMode(QwtLegendData::Checkable);

    ui->qwtPlot->insertLegend( legend, QwtPlot::RightLegend );
    connect(legend, SIGNAL(checked(QVariant, bool, int)), this, SLOT(legend_checked(QVariant, bool)));

    d_picker = new QwtPlotPicker(QwtPlot::xBottom,
                                 QwtPlot::yLeft,
                                 QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::AlwaysOn,
                                 ui->qwtPlot->canvas());

    d_picker->setRubberBandPen(QColor(Qt::green));
    //d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::white));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());

    if(title.isEmpty() == false)
        ui->qwtPlot->setTitle(title);

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, max_axis_X);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,   min_axis_Y, max_axis_Y);

    if(title_axis_X.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title_axis_X);

    if(title_axis_Y.isEmpty() == false)
        ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title_axis_Y);

    //---
    view_curve = new CurveData();
    plot_curve = new QwtPlotCurve();

    //---
    //TODO сглаживание
#if 0
    QwtSplineCurveFitter *fitter=new QwtSplineCurveFitter;
    fitter->setFitMode(QwtSplineCurveFitter::Spline);
    plot_curve->setStyle(QwtPlotCurve::Lines);
    plot_curve->setCurveAttribute(QwtPlotCurve::Fitted);
    plot_curve->setCurveFitter(fitter);
#endif
    //---

    plot_curve->setTitle("data");
    plot_curve->setPen(Qt::red);
    plot_curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    plot_curve->attach(ui->qwtPlot);
    plot_curve->setData(view_curve);
    //---

    installEventFilter(this);

    connect(ui->btn_Load, SIGNAL(clicked()), this, SLOT(load_curve()));
    connect(ui->btn_Save, SIGNAL(clicked()), this, SLOT(save_curve()));
}
//--------------------------------------------------------------------------------
void CurveBox::legend_checked(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *item = itemInfo.value<QwtPlotItem *>();
    item->setVisible(on);
    updateGraphics();
}
//--------------------------------------------------------------------------------
#ifndef NO_TESTBAR_CURVEBOX
void CurveBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(!mw)
    {
        qDebug() << "no mainwindow found!";
        return;
    }
}
#endif
//--------------------------------------------------------------------------------
void CurveBox::set_axis_label_rotation(int angle)
{
    ui->qwtPlot->setAxisLabelRotation(QwtPlot::xBottom, angle);
}
//--------------------------------------------------------------------------------
void CurveBox::set_axis_label_alignment(int axisId, Qt::Alignment alignment)
{
    ui->qwtPlot->setAxisLabelAlignment(axisId, alignment);
}
//--------------------------------------------------------------------------------
void CurveBox::connect_log(void)
{
    if(topLevelWidget())
    {
        connect(this, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), topLevelWidget(), SIGNAL(message(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void CurveBox::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
CurveBox::~CurveBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void CurveBox::init()
{
    ui->setupUi(this);

    flag_symbol = false;

    connect_log();
    create_widgets();
    add_data();
#ifndef NO_TESTBAR_CURVEBOX
    createTestBar();
#endif
    updateText();
    updateGraphics();
}
//--------------------------------------------------------------------------------
void CurveBox::updateGraphics(void)
{
    ui->qwtPlot->replot();
}
//--------------------------------------------------------------------------------
void CurveBox::updateText()
{
    //???
}
//--------------------------------------------------------------------------------
void CurveBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void CurveBox::load_curve(void)
{
    emit error("пока не сделано");
}
//--------------------------------------------------------------------------------
void CurveBox::save_curve(void)
{
    emit error("пока не сделано");
}
//--------------------------------------------------------------------------------
void CurveBox::add_data(void)
{
    for(int x=0; x<1024; x++)
    {
        view_curve->append(QPointF(x, 1024.0f));
    }
    view_curve->new_data(QPointF(500, 50000.0f));
    view_curve->new_data(QPointF(501, 50000.0f));
    view_curve->new_data(QPointF(502, 50000.0f));
    updateGraphics();
}
//--------------------------------------------------------------------------------
void CurveBox::set_title(const QString &title)
{
    if(title.isEmpty()) return;

    this->title = title;
    ui->qwtPlot->setTitle(title);
}
//--------------------------------------------------------------------------------
void CurveBox::set_title_axis_X(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_X = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, title);
}
//--------------------------------------------------------------------------------
void CurveBox::set_title_axis_Y(const QString &title)
{
    if(title.isEmpty()) return;

    this->title_axis_Y = title;
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, title);
}
//--------------------------------------------------------------------------------
void CurveBox::setVisible_btn_Load(bool state)
{
    ui->btn_Load->setVisible(state);
}
//--------------------------------------------------------------------------------
void CurveBox::setVisible_btn_Save(bool state)
{
    ui->btn_Save->setVisible(state);
}
//--------------------------------------------------------------------------------
void CurveBox::test(void)
{
    emit info("test");
    //test_draw_circle();
    //test_get_info();
}
//--------------------------------------------------------------------------------
void CurveBox::set_silense(bool state)
{
    is_silence = state;
}
//--------------------------------------------------------------------------------
void CurveBox::retranslateUi(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
QPainterPath CurveBox::createPath(QPointF cord, QList<QPointF> points)
{
    while (points.count() % 3 != 0)
        points << points.at(points.count() - 1);

    QPainterPath path;
    path.moveTo(cord);
    int i = 0;
    while (i + 2 < points.count()){
        path.cubicTo(
                    points.at(i),
                    points.at(i + 1),
                    points.at(i + 2)
                    );
        i += 3;
    }
    return path;
}
//--------------------------------------------------------------------------------
bool CurveBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e = (QMouseEvent*)event;
        QPoint viewPos = e->pos();
        emit info(QString("%1:%2").arg(viewPos.x()).arg(viewPos.y()));

        //???
        emit info(QString("X %1").arg(d_picker->invTransform2(d_picker->trackerPosition()).x()));
        emit info(QString("Y %1").arg(d_picker->invTransform2(d_picker->trackerPosition()).y()));

        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
