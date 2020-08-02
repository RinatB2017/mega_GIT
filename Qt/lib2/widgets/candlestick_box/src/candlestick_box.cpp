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

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
    axisY->setMin(axisY->min() * 0.99);
    axisY->setMax(axisY->max() * 1.01);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    connect(ui->chartView,              &MyQChartView::local_pos,   this,   &CandleStick_Box::show_volumes);

    ui->chartView->setRubberBand(QChartView::RectangleRubberBand);

    ui->chartView->setToolTip("chart");
}
//--------------------------------------------------------------------------------
void CandleStick_Box::append(QCandlestickSet *set, qreal volume)
{
    emit trace(Q_FUNC_INFO);

    candleSeries->append(set);

    emit debug(QString("cnt %1").arg(candleSeries->count()));

    //categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy.MM.dd");
    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString();
    volumes.append(volume);
    index_max++;
}
//--------------------------------------------------------------------------------
bool CandleStick_Box::get_ticket_data(int index, QCandlestickSet *set, qreal *volume)
{
    if(index < 0)
    {
        emit error("index < 0");
        return false;
    }
    if(index >= candleSeries->count())
    {
        emit error("index too big");
        return false;
    }
    QCandlestickSet *temp_set = candleSeries->sets().at(index);

    //TODO костыль, напрямую пока не получается
    (*set).setTimestamp((*temp_set).timestamp());
    (*set).setOpen((*temp_set).open());
    (*set).setClose((*temp_set).close());
    (*set).setHigh((*temp_set).high());
    (*set).setLow((*temp_set).low());

    *volume = volumes.at(index);
    return true;
}
//--------------------------------------------------------------------------------
int CandleStick_Box::get_max_index(void)
{
    return candleSeries->count();
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
    volumes.clear();
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

    emit debug(QString("v_min %1 v_max %2")
               .arg(v_min)
               .arg(v_max));

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
    axisY->setMin(v_min);
    axisY->setMax(v_max);
#endif

    create_volumes_grapher(ui->chartView->width());
}
//--------------------------------------------------------------------------------
void CandleStick_Box::create_volumes_grapher(int new_width)
{
    QPixmap pixmap(new_width, 50);
    pixmap.fill(Qt::white);

    qreal max_height = 0;
    for(int n=0; n<volumes.count(); n++)
    {
        qreal current_value = 0;
        current_value = volumes.at(n);
        if(current_value > max_height)
        {
            max_height = current_value;
        }
    }
    emit info(QString("max_height %1").arg(max_height));

    QPainter painter(&pixmap);
    painter.setPen(Qt::red);
    painter.drawLine(0, 0, pixmap.width(), pixmap.height());
}
//--------------------------------------------------------------------------------
void CandleStick_Box::set_ticket_name(const QString &new_ticket_name)
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
}
//--------------------------------------------------------------------------------
void CandleStick_Box::set_axesY_value(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_min_inc(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_max_inc(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_min_dec(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::axesX_max_dec(void)
{
    emit trace(Q_FUNC_INFO);
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
}
//--------------------------------------------------------------------------------
void CandleStick_Box::move_down(void)
{
    emit trace(Q_FUNC_INFO);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
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
void CandleStick_Box::resizeEvent(QResizeEvent *event)
{
    //ui->lbl_volumes_grapher->setPixmap(QPixmap(0, 0));
    QWidget::resizeEvent(event);
#if 1
    ui->chartView->update();
    //create_volumes_grapher(ui->chartView->width());
#else
    create_volumes_grapher(event->size().width());
#endif
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
