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
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "histogrambox.hpp"
#include "mainwindow.hpp"
#include "test_HistogramBox_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::test_grapher(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    HistogramBox *gb = mb->findChild<HistogramBox *>("HistogramBox");
    QVERIFY(gb);

    gb->remove_all_histogram();

    QCOMPARE(gb->add_histogram(0,   "curve0",   QColor(Qt::red)),       0);
    QCOMPARE(gb->add_histogram(1,   "curve1",   QColor(Qt::green)),     1);
    QCOMPARE(gb->add_histogram(2,   "curve2",   QColor(Qt::blue)),      2);
    QCOMPARE(gb->add_histogram(3,   "curve3",   QColor(Qt::cyan)),      3);
    QCOMPARE(gb->add_histogram(4,   "curve4",   QColor(Qt::magenta)),   4);

    QCOMPARE(gb->get_histograms_count(),    5);

    for(int n=0; n<1000; n++)
    {
        QCOMPARE(gb->add_histogram_data(0, 10, n),  true);
    }

    float x = 0;
    for(int n=0; n<1000; n++)
    {
        gb->get_histogram_data(0, n, &x);
        QCOMPARE((int)x, n);
    }
}
//--------------------------------------------------------------------------------
