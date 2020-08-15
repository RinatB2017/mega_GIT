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

#include "candlestick_box_adv.hpp"
#include "ui_candlestick_box_adv.h"

#include "candlestickdatareader.hpp"
//--------------------------------------------------------------------------------
CandleStick_Box_adv::CandleStick_Box_adv(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::CandleStick_Box_adv)
{
    init();
}
//--------------------------------------------------------------------------------
CandleStick_Box_adv::~CandleStick_Box_adv()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init(void)
{
    ui->setupUi(this);

    setWindowTitle(ticket_name);
    //---
    candleSeries = new QCandlestickSeries();
    candleSeries->setName(ticket_name);
    candleSeries->setIncreasingColor(QColor(Qt::green));
    candleSeries->setDecreasingColor(QColor(Qt::red));
    //---

    //---
    //---

    chart = new QChart();

    chart->addSeries(candleSeries);
    //chart->setTitle(ticket_name);
    //chart->setAnimationOptions(QChart::SeriesAnimations);

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

    connect(ui->btn_move_left,  &QPushButton::clicked,  this,   &CandleStick_Box_adv::move_left);
    connect(ui->btn_move_right, &QPushButton::clicked,  this,   &CandleStick_Box_adv::move_right);
    connect(ui->btn_move_up,    &QPushButton::clicked,  this,   &CandleStick_Box_adv::move_up);
    connect(ui->btn_move_down,  &QPushButton::clicked,  this,   &CandleStick_Box_adv::move_down);
    connect(ui->btn_move_reset, &QPushButton::clicked,  this,   &CandleStick_Box_adv::move_reset);

    connect(ui->btn_zoom_in,    &QPushButton::clicked,  this,   &CandleStick_Box_adv::zoom_in);
    connect(ui->btn_zoom_out,   &QPushButton::clicked,  this,   &CandleStick_Box_adv::zoom_out);
    connect(ui->btn_zoom_reset, &QPushButton::clicked,  this,   &CandleStick_Box_adv::zoom_reset);

    connect(ui->btn_theme_light,    &QPushButton::clicked,  this,   &CandleStick_Box_adv::set_theme_light);
    connect(ui->btn_theme_dark,     &QPushButton::clicked,  this,   &CandleStick_Box_adv::set_theme_dark);

    connect(ui->btn_clear_data, &QPushButton::clicked,  this,   &CandleStick_Box_adv::clear_data);

    connect(ui->chartView,      &MyQChartView::local_pos,   this,   &CandleStick_Box_adv::show_volumes);

    ui->sb_pos->setRange(1, 1000);

    ui->dsb_zoom->setRange(0, 10);

    ui->chartView->setRubberBand(QChartView::RectangleRubberBand);

    ui->chartView->setToolTip("chart");

    ui->btn_theme_light->setToolTip("Light theme");
    ui->btn_theme_dark->setToolTip("Dark theme");

    //---
    //ui->lbl_volumes_grapher->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->lbl_volumes_grapher->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    ui->lbl_volumes_grapher->setScaledContents(true);
    //---

    QList<QAbstractButton *> btns = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *btn, btns)
    {
        btn->setProperty(NO_SAVE, true);
    }

    qDebug() << "horiz:" << chart->axes(Qt::Horizontal).at(0);
    qDebug() << "vert:"  << chart->axes(Qt::Vertical).at(0);

    load_widgets();
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::append(QCandlestickSet *set, qreal volume)
{
    emit trace(Q_FUNC_INFO);

    candleSeries->append(set);

    qDebug() << "cnt" << candleSeries->count();

    //categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy.MM.dd");
    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString();
    volumes.append(volume);
}
//--------------------------------------------------------------------------------
bool CandleStick_Box_adv::get_ticket_data(int index, QCandlestickSet *set, qreal *volume)
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
int CandleStick_Box_adv::get_max_index(void)
{
    return candleSeries->count();
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::clear_data(void)
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
void CandleStick_Box_adv::update_data(void)
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

    create_volumes_grapher(ui->chartView->width());
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::create_volumes_grapher(int new_width)
{
    QPixmap pixmap(new_width, 50);
    pixmap.fill(Qt::white);

    qreal max_height = 0;
    for(int n=0; n<volumes.count(); n++)
    {
        qreal current_value = volumes.at(n);
        if(current_value > max_height)
        {
            max_height = current_value;
        }
    }
    emit info(QString("max_height %1").arg(max_height));

    QPainter painter(&pixmap);
    painter.setPen(Qt::red);
    painter.drawLine(0, 0, pixmap.width(), pixmap.height());

    ui->lbl_volumes_grapher->setPixmap(pixmap);

    //ui->lbl_volumes_grapher->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    //ui->lbl_volumes_grapher->setScaledContents(true);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::set_ticket_name(const QString &new_ticket_name)
{
    ticket_name = new_ticket_name;
    setWindowTitle(ticket_name);
    candleSeries->setName(ticket_name);
    chart->setTitle(ticket_name);
}
//--------------------------------------------------------------------------------
QString CandleStick_Box_adv::get_ticket_name(void)
{
    return ticket_name;
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::zoom_in(void)
{
    emit trace(Q_FUNC_INFO);
    qreal k = ui->dsb_zoom->value();
    chart->zoom(1.0 - k);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::zoom_out(void)
{
    emit trace(Q_FUNC_INFO);
    qreal k = ui->dsb_zoom->value();
    chart->zoom(1.0 + k);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::zoom_reset(void)
{
    chart->zoomReset();
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::move_left(void)
{
    emit trace(Q_FUNC_INFO);
    int delta = ui->sb_pos->value();
    dx += delta;
    chart->scroll(delta, 0);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::move_right(void)
{
    emit trace(Q_FUNC_INFO);
    int delta = ui->sb_pos->value();
    dx -= delta;
    chart->scroll(-delta, 0);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::move_up(void)
{
    emit trace(Q_FUNC_INFO);
    int delta = ui->sb_pos->value();
    dy -= delta;
    chart->scroll(0, -delta);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::move_down(void)
{
    emit trace(Q_FUNC_INFO);
    int delta = ui->sb_pos->value();
    dy += delta;
    chart->scroll(0, delta);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::move_reset(void)
{
    chart->scroll(-dx, -dy);
    dx = 0;
    dy = 0;
}
//--------------------------------------------------------------------------------
bool CandleStick_Box_adv::get_index(QString key, int *index)
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
void CandleStick_Box_adv::show_volumes(QMouseEvent *event)
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
void CandleStick_Box_adv::set_theme_light(void)
{
    chart->setTheme(QChart::ChartThemeLight);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::set_theme_dark(void)
{
    chart->setTheme(QChart::ChartThemeDark);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::test(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::resizeEvent(QResizeEvent *event)
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
void CandleStick_Box_adv::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool CandleStick_Box_adv::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::save_setting(void)
{

}
//--------------------------------------------------------------------------------
