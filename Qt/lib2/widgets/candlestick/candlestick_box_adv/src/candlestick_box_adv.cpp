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
    init_candle_series();
    init_chart();
    init_rubberband();
    init_axis();
    init_icons();
    init_tooltips();
    connect_widgets();

    //---
#ifdef SHOW_VOLUMES
    volume_series = new QBarSeries();

    volume_chart = new QChart();
    volume_chart->addSeries(volume_series);
    volume_chart->createDefaultAxes();
    volume_chart->axisY()->setMax(110);
    //volume_chart->axisX()->setMin(-100);
    ui->volumes_widget->setChart(volume_chart);
    ui->volumes_widget->setRenderHint(QPainter::Antialiasing);
#else
    ui->volumes_widget->setVisible(false);
#endif
    //---

    ui->sb_pos->setRange(1, 1000);
    ui->dsb_zoom->setRange(0, 10);

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

    ui->btn_show_frame->setFixedSize(24, 24);
    ui->frame->setVisible(false);

    load_widgets();
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_candle_series(void)
{
    candleSeries = new QCandlestickSeries();
    candleSeries->setName(ticket_name);
    candleSeries->setIncreasingColor(QColor(Qt::green));
    candleSeries->setDecreasingColor(QColor(Qt::red));

    connect(candleSeries,   &QCandlestickSeries::hovered,   this,   &CandleStick_Box_adv::hovered_candle);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_chart(void)
{
    chart = new QChart();
    chart->addSeries(candleSeries);
    //chart->setTitle(ticket_name);
    //chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_rubberband(void)
{
    //ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
    ui->cb_rubberband->addItem("NoRubberBand", 0);
    ui->cb_rubberband->addItem("VerticalRubberBand", 0x1);
    ui->cb_rubberband->addItem("HorizontalRubberBand", 0x2);
    ui->cb_rubberband->addItem("RectangleRubberBand", 0x3);
    connect(ui->cb_rubberband,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),    this,   &CandleStick_Box_adv::set_rubber_band);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_axis(void)
{
    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);
    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    Q_CHECK_PTR(axisY);
    axisY->setMin(axisY->min() * 0.99);
    axisY->setMax(axisY->max() * 1.01);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_icons(void)
{
    ui->btn_clear_data->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_move_reset->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_zoom_reset->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::init_tooltips(void)
{
    ui->chartView->setToolTip("chart");
    ui->btn_theme_light->setToolTip("Light theme");
    ui->btn_theme_dark->setToolTip("Dark theme");

    ui->btn_move_up->setToolTip("move up");
    ui->btn_move_down->setToolTip("move down");
    ui->btn_move_left->setToolTip("move left");
    ui->btn_move_right->setToolTip("move right");
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::connect_widgets(void)
{
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

    connect(ui->btn_test,       &QPushButton::clicked,  this,   &CandleStick_Box_adv::test);

    connect(ui->btn_show_frame, &QToolButton::clicked,  ui->frame,   &QFrame::setVisible);
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::set_rubber_band(int index)
{
    bool ok = false;
    int cmd = ui->cb_rubberband->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;

    switch (cmd)
    {
    case QChartView::NoRubberBand:
        ui->chartView->setRubberBand(QChartView::NoRubberBand);
        break;
    case QChartView::VerticalRubberBand:
        ui->chartView->setRubberBand(QChartView::VerticalRubberBand);
        break;
    case QChartView::HorizontalRubberBand:
        ui->chartView->setRubberBand(QChartView::HorizontalRubberBand);
        break;
    case QChartView::RectangleRubberBand:
        ui->chartView->setRubberBand(QChartView::RectangleRubberBand);
        break;
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::hovered_candle(bool state, QCandlestickSet *set)
{
    if(state)
    {
        QString temp;
        temp.append(QString("open\t%1\n").arg(set->open()));
        temp.append(QString("hi\t%1\n").arg(set->high()));
        temp.append(QString("lo\t%1\n").arg(set->low()));
        temp.append(QString("close\t%1\n").arg(set->close()));

        for(int n=0; n<candleSeries->count(); n++)
        {
            if(candleSeries->sets().at(n) == set)
            {
                temp.append(QString("volume\t%1").arg(volumes.at(n)));
                break;
            }
        }

        ui->chartView->setToolTip(temp);
    }
    else
    {
        ui->chartView->setToolTip(nullptr);
    }
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::append(QCandlestickSet *set, qreal volume)
{
    emit trace(Q_FUNC_INFO);

    candleSeries->append(set);

    //---
#ifdef SHOW_VOLUMES
    QBarSet *volume_bar = new QBarSet("volume");
    volume_bar->append(temp_volume+=10.0);
    volume_series->append(volume_bar);

    volumes_categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString();
#endif
    //---

    qDebug() << "cnt"  << candleSeries->count();

    //categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy.MM.dd");
    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString();
    volumes.append(volume);
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
    //volumes_categories.clear();
    volumes.clear();
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::update_data(void)
{
    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);

#ifdef SHOW_VOLUMES
    QBarCategoryAxis *ax = qobject_cast<QBarCategoryAxis *>(volume_chart->axes(Qt::Horizontal).at(0));
    ax->setCategories(volumes_categories);
#endif

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

#ifdef SHOW_VOLUMES
    volume_chart->scroll(10, 0);
#endif
    //qDebug() << volume_chart->axisX();
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
    chart->setTheme(static_cast<QChart::ChartTheme>(load_int("chart_theme")));
}
//--------------------------------------------------------------------------------
void CandleStick_Box_adv::save_setting(void)
{
    save_int("chart_theme", chart->theme());
}
//--------------------------------------------------------------------------------
