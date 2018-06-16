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
    explicit CandleStick_Box(QWidget *parent = 0);
    ~CandleStick_Box();

    void append(QCandlestickSet *set);
    void update_data(void);

private:
    Ui::CandleStick_Box *ui;
    QCandlestickSeries *acmeSeries = 0;
    QChart *chart = 0;
    QStringList categories;
    QBarCategoryAxis *axisX = 0;

};
//--------------------------------------------------------------------------------
#endif // CANDLESTICK_BOX_HPP
