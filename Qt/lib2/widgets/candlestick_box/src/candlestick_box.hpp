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
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QPointer>
#include <QWidget>

QT_CHARTS_USE_NAMESPACE
//--------------------------------------------------------------------------------
namespace Ui {
    class CandleStick_Box;
}
//--------------------------------------------------------------------------------
class CandleStick_Box : public QWidget
{
    Q_OBJECT

public:
    explicit CandleStick_Box(QWidget *parent = nullptr);
    ~CandleStick_Box();

    void append(QCandlestickSet *set);
    void update_data(void);

    void set_ticket_name(QString new_ticket_name);
    QString get_ticket_name(void);

private:
    Ui::CandleStick_Box *ui;
    QPointer<QCandlestickSeries> acmeSeries;
    QPointer<QChart> chart;
    QStringList categories;
    QPointer<QBarCategoryAxis> axisX;
    QString ticket_name = "unknown";
};
//--------------------------------------------------------------------------------
#endif // CANDLESTICK_BOX_HPP
