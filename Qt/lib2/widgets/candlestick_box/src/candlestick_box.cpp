/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QVBoxLayout>
#include <QDebug>

#include "candlestick_box.hpp"
#include "ui_candlestick_box.h"

#include "candlestickdatareader.h"
//--------------------------------------------------------------------------------
CandleStick_Box::CandleStick_Box(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CandleStick_Box)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
CandleStick_Box::~CandleStick_Box()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void CandleStick_Box::init(void)
{
    setWindowTitle(ticket_name);
    //---
    candleSeries = new QCandlestickSeries();
    candleSeries->setName(ticket_name);
    candleSeries->setIncreasingColor(QColor(Qt::green));
    candleSeries->setDecreasingColor(QColor(Qt::red));
    //---

    chart = new QChart();

    chart->addSeries(candleSeries);
    chart->setTitle(ticket_name);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();

    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);

    ui->btn_clear_data->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
    axisY->setMin(axisY->min() * 0.99);
    axisY->setMax(axisY->max() * 1.01);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    connect(ui->btn_axesX_min_inc,      &QPushButton::clicked,  this,   &CandleStick_Box::axesX_min_inc);
    connect(ui->btn_axesX_max_inc,      &QPushButton::clicked,  this,   &CandleStick_Box::axesX_max_inc);
    connect(ui->btn_axesX_min_dec,      &QPushButton::clicked,  this,   &CandleStick_Box::axesX_min_dec);
    connect(ui->btn_axesX_max_dec,      &QPushButton::clicked,  this,   &CandleStick_Box::axesX_max_dec);

    connect(ui->btn_move_left,          &QPushButton::clicked,  this,   &CandleStick_Box::move_left);
    connect(ui->btn_move_right,         &QPushButton::clicked,  this,   &CandleStick_Box::move_right);
    connect(ui->btn_move_up,            &QPushButton::clicked,  this,   &CandleStick_Box::move_up);
    connect(ui->btn_move_down,          &QPushButton::clicked,  this,   &CandleStick_Box::move_down);

    connect(ui->btn_get_axesY_value,    &QPushButton::clicked,  this,   &CandleStick_Box::get_axesY_value);
    connect(ui->btn_set_axesY_value,    &QPushButton::clicked,  this,   &CandleStick_Box::set_axesY_value);
    connect(ui->btn_test,               &QPushButton::clicked,  this,   &CandleStick_Box::test);

    connect(ui->btn_clear_data,         &QPushButton::clicked,  this,   &CandleStick_Box::clear_data);

    connect(ui->chartView,              &MyQChartView::local_pos,   this,   &CandleStick_Box::show_volumes);

    ui->dsb_axesY_min_value->setRange(-100000, 100000);
    ui->dsb_axesY_max_value->setRange(-100000, 100000);

    ui->sb_axesX_min_delta->setRange(1, 1000);
    ui->sb_axesX_max_delta->setRange(1, 1000);

    ui->sb_posx_delta->setRange(1, 1000);
    ui->dsb_posy_delta->setRange(-1000, 1000);

    ui->dsb_axesY_min_value->setDecimals(8);
    ui->dsb_axesY_max_value->setDecimals(8);

    ui->dsb_posy_delta->setDecimals(8);

    ui->dsb_axesY_min_value->setSingleStep(0.1);
    ui->dsb_axesY_max_value->setSingleStep(0.1);

    ui->chartView->setRubberBand(QChartView::RectangleRubberBand);

    ui->chartView->setToolTip("chart");

    qDebug() << "horiz:" << chart->axes(Qt::Horizontal).at(0);
    qDebug() << "vert:"  << chart->axes(Qt::Vertical).at(0);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::append(QCandlestickSet *set)
{
    candleSeries->append(set);

    qDebug() << "cnt" << candleSeries->count();

    //categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy.MM.dd");
    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString();
    index_max++;
}
//--------------------------------------------------------------------------------
void CandleStick_Box::clear_data(void)
{
    Q_CHECK_PTR(candleSeries);
    while(candleSeries->count() > 0)
    {
        candleSeries->remove(candleSeries->sets().at(0));
    }
    categories.clear();
}
//--------------------------------------------------------------------------------
void CandleStick_Box::update_data(void)
{
    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);

#if 1
    qreal v_max = -100;
    qreal v_min = 100;
    for(int n=0; n<candleSeries->count(); n++)
    {
        qreal v_open  = candleSeries->sets().at(n)->open();
        qreal v_close = candleSeries->sets().at(n)->close();
        qreal v_high  = candleSeries->sets().at(n)->high();
        qreal v_low   = candleSeries->sets().at(n)->low();

        if(v_open > v_max)  v_max = v_open;
        if(v_close > v_max) v_max = v_close;
        if(v_high > v_max)  v_max = v_high;
        if(v_low > v_max)   v_max = v_low;

        if(v_open < v_min)  v_min = v_open;
        if(v_close < v_min) v_min = v_close;
        if(v_high < v_min)  v_min = v_high;
        if(v_low < v_min)   v_min = v_low;
    }

    qDebug() << v_min << v_max;

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
    axisY->setMin(v_min);
    axisY->setMax(v_max);
#endif
}
//--------------------------------------------------------------------------------
void CandleStick_Box::set_ticket_name(QString new_ticket_name)
{
    ticket_name = new_ticket_name;
    setWindowTitle(ticket_name);
    candleSeries->setName(ticket_name);
    chart->setTitle(ticket_name);
}
//--------------------------------------------------------------------------------
QString CandleStick_Box::get_ticket_name(void)
{
    return ticket_name;
}
//--------------------------------------------------------------------------------
void CandleStick_Box::get_axesY_value(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);

    qreal min_value = axisY->min();
    qreal max_value = axisY->max();
    ui->dsb_axesY_min_value->setValue(min_value);
    ui->dsb_axesY_max_value->setValue(max_value);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::set_axesY_value(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);

    axisY->setMin(ui->dsb_axesY_min_value->value());
    axisY->setMax(ui->dsb_axesY_max_value->value());
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_min_inc(void)
{
    emit trace(Q_FUNC_INFO);

    int delta = ui->sb_axesX_min_delta->value();
    if(index_min < (categories.count() - delta - 1))
    {
        index_min+=delta;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_max_inc(void)
{
    emit trace(Q_FUNC_INFO);

    int delta = ui->sb_axesX_max_delta->value();
    if(index_max < (categories.count() - delta - 1))
    {
        index_max+=delta;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_min_dec(void)
{
    emit trace(Q_FUNC_INFO);

    int delta = ui->sb_axesX_min_delta->value();
    if(index_min > delta)
    {
        index_min-=delta;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_max_dec(void)
{
    emit trace(Q_FUNC_INFO);

    int delta = ui->sb_axesX_max_delta->value();
    if(index_max > delta)
    {
        index_max-=delta;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::move_left(void)
{
    emit trace(Q_FUNC_INFO);

    if(index_min > 0)
    {
        index_min--;
        index_max--;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::move_right(void)
{
    emit trace(Q_FUNC_INFO);

    if(index_max < (categories.count() - 1))
    {
        index_min++;
        index_max++;

        axisX->setMin(categories.at(index_min));
        axisX->setMax(categories.at(index_max-1));
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box::move_up(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);

    axisY->setMin(axisY->min()+ui->dsb_posy_delta->value());
    axisY->setMax(axisY->max()+ui->dsb_posy_delta->value());
}
//--------------------------------------------------------------------------------
void CandleStick_Box::move_down(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);

    axisY->setMin(axisY->min()-ui->dsb_posy_delta->value());
    axisY->setMax(axisY->max()-ui->dsb_posy_delta->value());
}
//--------------------------------------------------------------------------------
bool CandleStick_Box::get_index(QString key, int *index)
{
    int temp = categories.indexOf(key);
    if(temp == -1)
    {
        return false;
    }

    *index = temp;
    return true;
}
//--------------------------------------------------------------------------------
void CandleStick_Box::show_volumes(QMouseEvent *event)
{
    //FIXME надо сделать
#if 0
    QString pos = QString("pos: %1 %2")
            .arg(event->localPos().x())
            .arg(event->localPos().y());
    ui->chartView->setToolTip(pos);
#else
    Q_UNUSED(event)
#endif
}
//--------------------------------------------------------------------------------
void CandleStick_Box::test2(void)
{
    emit info(QString("min %1").arg(axisX->min()));
    emit info(QString("max %1").arg(axisX->max()));
    emit info(QString("cnt %1").arg(axisX->count()));

    QList<QCandlestickSet *> l_sets = candleSeries->sets();
    if(l_sets.isEmpty())
    {
        return;
    }

    QCandlestickSet *set = l_sets.at(1);
//    emit info(QString("%1").arg(->open()));
    emit info(QString("%1\t%2\t%3\t%4\t%5")
              .arg(set->open())
              .arg(set->high())
              .arg(set->low())
              .arg(set->close())
              .arg((quint64)set->timestamp()));

    emit info(QString("min index %1").arg(categories.indexOf(axisX->min())));
    emit info(QString("max index %1").arg(categories.indexOf(axisX->max())));
}
//--------------------------------------------------------------------------------
void CandleStick_Box::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    emit info(QString("index_min %1").arg(index_min));
    emit info(QString("index_max %1").arg(index_max));
    emit info(QString("categories cnt %1").arg(categories.count()));
#endif

#if 0
    bool ok = false;

    int index = 0;
    ok = get_index(axisX->min(), &index);
    if(ok)
        emit info(QString("min %1").arg(index));
    else
        emit error("min not found");

    ok = get_index(axisX->max(), &index);
    if(ok)
        emit info(QString("max %1").arg(index));
    else
        emit error("max not found");
#endif

#if 0
    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    Q_CHECK_PTR(axisX);
    foreach(QString cat, axisX->categories())
    {
        emit info(cat);
    }
#endif
}
//--------------------------------------------------------------------------------
// https://stackoverflow.com/questions/48623595/scale-x-axis-of-qchartview-using-mouse-wheel
void CandleStick_Box::wheelEvent(QWheelEvent *event)
{
    ui->chartView->chart()->zoomReset();

    mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

    QRectF rect = ui->chartView->chart()->plotArea();
    QPointF c = ui->chartView->chart()->plotArea().center();
    rect.setWidth(mFactor*rect.width());
    rect.moveCenter(c);
    ui->chartView->chart()->zoomIn(rect);

    //    QChartView::wheelEvent(event);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool CandleStick_Box::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void CandleStick_Box::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void CandleStick_Box::save_setting(void)
{

}
//--------------------------------------------------------------------------------
