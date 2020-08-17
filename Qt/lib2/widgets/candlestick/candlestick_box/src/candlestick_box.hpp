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
#ifndef CANDLESTICK_BOX_HPP
#define CANDLESTICK_BOX_HPP
//--------------------------------------------------------------------------------
#include <QBarCategoryAxis>
#include <QCandlestickSeries>
#include <QChartView>
#include <QValueAxis>
#include <QDateTime>
#include <QPointer>
#include <QWidget>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
QT_CHARTS_USE_NAMESPACE
//--------------------------------------------------------------------------------
namespace Ui {
    class CandleStick_Box;
}
//--------------------------------------------------------------------------------
class CandleStick_Box : public MyWidget
{
    Q_OBJECT

public:
    explicit CandleStick_Box(QWidget *parent = nullptr);
    ~CandleStick_Box();

    void append(QCandlestickSet *set, qreal volume);
    void update_data(void);
    void clear_data(void);

    bool get_ticket_data(int index, QCandlestickSet *set, qreal *volume);

    void set_ticket_name(const QString &new_ticket_name);
    QString get_ticket_name(void);
    int get_max_index(void);

    void scroll_left(void);
    void scroll_right(void);
    void scroll_up(void);
    void scroll_down(void);

    void move_left(void);
    void move_right(void);
    void move_up(void);
    void move_down(void);

    void zoom(qreal factor);

    void test2(void);

private:
    Ui::CandleStick_Box *ui;
    QPointer<QCandlestickSeries> candleSeries;
    QStringList categories;
    QPointer<QBarCategoryAxis> axisX;

    QPointer<QChart> chart;

    QString ticket_name = "unknown";

    void create_volumes_grapher(int new_width);
    QList<qreal> volumes;

    int index_min = 0;
    int index_max = 0;

    void init(void);

    bool get_index(QString key, int *index);

    void test(void);

    void hovered_candle(bool state, QCandlestickSet *set);
//    void pressed_candle(QCandlestickSet *set);
//    void released_candle(QCandlestickSet *set);

    void chart_test(void);

    void set_theme_light(void);
    void set_theme_dark(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

    qreal mFactor=1.0;

protected:
#if 0
    void wheelEvent(QWheelEvent *event);
#endif
#if 0
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#endif

    void resizeEvent (QResizeEvent * event);
};
//--------------------------------------------------------------------------------
#endif // CANDLESTICK_BOX_HPP
