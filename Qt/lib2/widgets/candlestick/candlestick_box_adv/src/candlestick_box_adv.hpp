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
#ifndef CANDLESTICK_BOX_ADV_HPP
#define CANDLESTICK_BOX_ADV_HPP
//--------------------------------------------------------------------------------
#include <QBarCategoryAxis>
#include <QCandlestickSeries>
#include <QChartView>
#include <QValueAxis>
#include <QDateTime>
#include <QPointer>
#include <QWidget>

#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
QT_CHARTS_USE_NAMESPACE
//--------------------------------------------------------------------------------
namespace Ui {
    class CandleStick_Box_adv;
}
//--------------------------------------------------------------------------------
class CandleStick_Box_adv : public MyWidget
{
    Q_OBJECT

public:
    explicit CandleStick_Box_adv(QWidget *parent = nullptr);
    ~CandleStick_Box_adv();

    void append(QCandlestickSet *set, qreal volume);
    void update_data(void);
    void clear_data(void);

    bool get_ticket_data(int index, QCandlestickSet *set, qreal *volume);

    void set_ticket_name(const QString &new_ticket_name);
    QString get_ticket_name(void);
    int get_max_index(void);

    void test(void);

private:
    Ui::CandleStick_Box_adv *ui;
    QPointer<QCandlestickSeries> candleSeries;
    QStringList categories;
    QPointer<QBarCategoryAxis> axisX;

    QPointer<QChart> chart;

    QString ticket_name = "unknown";

    void create_volumes_grapher(int new_width);
    QList<qreal> volumes;

    qreal dx = 0;
    qreal dy = 0;

#ifdef SHOW_VOLUMES
    QPointer<QBarSeries> volume_series;
    QPointer<QChart> volume_chart;
    QStringList volumes_categories;
    qreal temp_volume = 0;
#endif

    void init(void);
    void init_candle_series(void);
    void init_chart(void);
    void init_rubberband(void);
    void init_axis(void);
    void init_icons(void);
    void init_tooltips(void);
    void connect_widgets(void);

    void zoom_in(void);
    void zoom_out(void);
    void zoom_reset(void);

    void move_left(void);
    void move_right(void);
    void move_up(void);
    void move_down(void);
    void move_reset(void);

    void set_theme_light(void);
    void set_theme_dark(void);

    bool get_index(QString key, int *index);

    void show_volumes(QMouseEvent *event);

    void hovered_candle(bool state, QCandlestickSet *set);
    void set_rubber_band(int index);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void resizeEvent (QResizeEvent * event);
};
//--------------------------------------------------------------------------------
#endif // CANDLESTICK_BOX_ADV_HPP
