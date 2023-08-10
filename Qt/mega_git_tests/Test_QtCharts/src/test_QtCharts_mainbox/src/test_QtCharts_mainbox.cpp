/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
//--------------------------------------------------------------------------------
#include "ui_test_QtCharts_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QtCharts_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
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

#ifdef QT_DEBUG
    createTestBar();
#endif

    //---
    QLineSeries *series = new QLineSeries;
    series->setName("Channel A");
    //series->setUseOpenGL(true);
    for (int i=0;i<100; i++)
    {
        series->append(i, i*i);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("QT Charts example");

#if 0
    //add axis to the chart
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
#else
    //add axis to the chart
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 100);
    axisX->setTitleText(tr("Time"));
    axisX->setTitleBrush(Qt::magenta);
    axisX->setLabelsColor(Qt::magenta);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
#endif

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Money");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(chartView);
    setLayout(vbox);
    //---

    setMinimumHeight(sizeHint().height());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        CMD command;
        command.cmd = id++; command.cmd_text = "test";   command.func = &MainBox::test;  commands.append(command);

        QToolBar *testbar = new QToolBar(tr("testbar"));
        testbar->setObjectName("testbar");

        mw->addToolBar(Qt::TopToolBarArea, testbar);

        QCheckBox *cb_block = new QCheckBox("block");
        testbar->addWidget(cb_block);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, commands)
        {
            cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
        }

        testbar->addWidget(cb_test);
        QToolButton *frm_choice_test = add_button(testbar,
                                                  new QToolButton(this),
                                                  qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                  "choice_test",
                                                  "choice_test");
        frm_choice_test->setObjectName("frm_choice_test");

        connect(frm_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

        connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
        connect(cb_block, SIGNAL(clicked(bool)), frm_choice_test,   SLOT(setDisabled(bool)));
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef void (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
int MainBox::xxx(int x)
{
    //Q_ASSERT(x < 5);
    return x+5;
}
//--------------------------------------------------------------------------------
