/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_QCustomPlot_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_QCustomPlot_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "qcustomplot.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    plot = new QCustomPlot(this);
    ui->vbox->addWidget(plot);

    // plot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));

    plot->setAxisBackgroundScaledMode(Qt::KeepAspectRatio);
    plot->setAxisBackgroundScaled(true);

    plot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    plot->setInteraction(QCustomPlot::iSelectPlottables); // allow selection of graphs via mouse click

    plot->addGraph();
    plot->addGraph();
    plot->addGraph();

    plot->xAxis->setSubGrid(true);
    plot->yAxis->setSubGrid(true);

    plot->xAxis->setRange(0, 10);
    plot->yAxis->setRange(0, 2.5);

    plot->graph(0)->setName("A");
    plot->graph(0)->setPen(QPen(QColor(Qt::red)));

    plot->graph(1)->setName("B");
    plot->graph(1)->setPen(QPen(QColor(Qt::green)));

    plot->graph(2)->setName("C");
    plot->graph(2)->setPen(QPen(QColor(Qt::blue)));

    plot->legend->setVisible(true);
    plot->legend->setPositionStyle(QCPLegend::psTopRight);

    load("data.txt");
    updateText();

    setMinimumSize(640, 480);
}
//--------------------------------------------------------------------------------
int MainBox::load(const QString &filename)
{
    QString line;
    double phase_A;
    double phase_B;
    double phase_C;
    bool ok;

    emit debug(QString("load %1").arg(filename));

    QFile data(filename);
    if (data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        index = 0;
        while(1)
        {
            line = in.readLine();
            if(line.isNull()) break;
            phase_A = line.toDouble(&ok);
            if(!ok) phase_A = 0.0f;

            line = in.readLine();
            if(line.isNull()) break;
            phase_B = line.toDouble(&ok);
            if(!ok) phase_B = 0.0f;

            line = in.readLine();
            if(line.isNull()) break;
            phase_C = line.toDouble(&ok);
            if(!ok) phase_C = 0.0f;

            qDebug() << "phase_A" << phase_A << "phase_B" << phase_B << "phase_C" << phase_C;
            plot->graph(0)->addData(index, phase_A);
            plot->graph(1)->addData(index, phase_B);
            plot->graph(2)->addData(index, phase_C);
            index++;
        }
    }
    else
    {
        emit error(QString("failed open %1").arg(filename));
        return E_ERROR_FILE_NOT_OPEN;
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
    plot->setTitle(QObject::tr("generator"));
}
//--------------------------------------------------------------------------------
