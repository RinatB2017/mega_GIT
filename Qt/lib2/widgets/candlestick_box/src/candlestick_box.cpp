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
    QWidget(parent),
    ui(new Ui::CandleStick_Box)
{
    setWindowTitle(ticket_name);

    ui->setupUi(this);
    
    //---
    acmeSeries = new QCandlestickSeries();
    acmeSeries->setName(ticket_name);
    acmeSeries->setIncreasingColor(QColor(Qt::green));
    acmeSeries->setDecreasingColor(QColor(Qt::red));

#if 0
    QFile acmeData(":acme");
    if (acmeData.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        CandlestickDataReader dataReader(&acmeData);
        while (!dataReader.atEnd())
        {
            QCandlestickSet *set = dataReader.readCandlestickSet();
            if (set)
            {
                acmeSeries->append(set);
                categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("dd");
            }
        }
    }
#endif
    //---

    chart = new QChart();
    chart->addSeries(acmeSeries);
    chart->setTitle(ticket_name);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();

    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);

//    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
//    Q_CHECK_PTR(axisY);
//    axisY->setMin(axisY->min() * 0.99);
//    axisY->setMax(axisY->max() * 1.01);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(chartView);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::append(QCandlestickSet *set)
{
    qDebug() << set->timestamp();

    acmeSeries->append(set);

    qDebug() << "cnt" << acmeSeries->count();

//    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("yyyy-MM-dd");
    categories << QDateTime::fromMSecsSinceEpoch(set->timestamp()).toString("dd");

    QDateTime timestamp;
    timestamp.setTime_t(set->timestamp());
    qDebug() << QString("Unixtime: %1").arg(timestamp.toString(Qt::SystemLocaleLongDate));
}
//--------------------------------------------------------------------------------
void CandleStick_Box::update_data(void)
{
    axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);
}
//--------------------------------------------------------------------------------
void CandleStick_Box::set_ticket_name(QString new_ticket_name)
{
    ticket_name = new_ticket_name;
    setWindowTitle(ticket_name);
    acmeSeries->setName(ticket_name);
    chart->setTitle(ticket_name);
}
//--------------------------------------------------------------------------------
QString CandleStick_Box::get_ticket_name(void)
{
    return ticket_name;
}
//--------------------------------------------------------------------------------
CandleStick_Box::~CandleStick_Box()
{
    delete ui;
}
//--------------------------------------------------------------------------------
