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
#include "ui_test_HistogramBox_mainbox.h"
//--------------------------------------------------------------------------------
#include "qwt_symbol.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "histogrambox.hpp"
#include "test_HistogramBox_mainbox.hpp"
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
    all_break = true;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    all_break = false;

    createTestBar();

    histogram = new HistogramBox(this);
    histogram->setObjectName("HistogramBox");
    histogram->set_title("тест");
    histogram->set_title_axis_X("тест X");
    histogram->set_title_axis_Y("тест Y");

#if 1
    histogram->add_histogram(0, tr("red"), Qt::red);
    histogram->add_histogram(1, tr("green"), Qt::green);
    histogram->add_histogram(2, tr("blue"), Qt::blue);
#else
    for(int n=0; n<2; n++)
    {
        QColor color = QColor(rand() % 255, rand() % 255, rand() % 255);
        histogram->add_histogram(n, QString(tr("histogram %1")).arg(n), color);
    }
#endif

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(histogram);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::load(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
#if 0
    histogram->test();
#endif

#if 1
    emit info(tr("test begin"));
    //histogram->clear();
    histogram->push_btn_Horizontal(true);
    histogram->push_btn_Vertical(true);
    unsigned int pos_x = 0;
    for(int x=0; x<histogram->get_histograms_count(); x++)
    {
        if(all_break) return;
        QCoreApplication::processEvents();
        histogram->add_histogram_data(x, pos_x, 5, qrand() % 100);
        pos_x += 7;
    }
    emit info(tr("test end"));
#endif
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
