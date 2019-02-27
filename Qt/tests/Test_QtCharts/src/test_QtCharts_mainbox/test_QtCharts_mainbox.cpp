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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
    createTestBar();

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

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(chartView);
    setLayout(vbox);
    //---

    setMinimumHeight(sizeHint().height());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    CMD command;
    command.cmd = ID_TEST_0; command.cmd_text = "test 0";   command.func = &MainBox::test_0;  commands.append(command);
    command.cmd = ID_TEST_1; command.cmd_text = "test 1";   command.func = &MainBox::test_1;  commands.append(command);
    command.cmd = ID_TEST_2; command.cmd_text = "test 2";   command.func = &MainBox::test_2;  commands.append(command);
    command.cmd = ID_TEST_3; command.cmd_text = "test 3";   command.func = &MainBox::test_3;  commands.append(command);
    command.cmd = ID_TEST_4; command.cmd_text = "test 4";   command.func = &MainBox::test_4;  commands.append(command);
    command.cmd = ID_TEST_5; command.cmd_text = "test 5";   command.func = &MainBox::test_5;  commands.append(command);
    command.cmd = ID_TEST_6; command.cmd_text = "test 6";   command.func = 0;    commands.append(command);

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
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    emit info("Test_0()");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
int MainBox::xxx(int x)
{
    //Q_ASSERT(x < 5);
    return x+5;
}
//--------------------------------------------------------------------------------
