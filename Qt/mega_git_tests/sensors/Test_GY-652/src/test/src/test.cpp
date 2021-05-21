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
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_GY-652_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
#   include "grapherbox.hpp"
#endif
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
void Test::test_grapher(void)
{
#ifdef NO_GRAPHER
    return;
#endif

    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    GrapherBox *gb = mb->findChild<GrapherBox *>("GrapherBox");
    QVERIFY(gb);

    gb->remove_all_curve();

    QCOMPARE(gb->add_curve("curve0"),   0);
    QCOMPARE(gb->add_curve("curve1"),   1);
    QCOMPARE(gb->add_curve("curve2"),   2);
    QCOMPARE(gb->add_curve("curve3"),   3);
    QCOMPARE(gb->add_curve("curve4"),   4);

    QCOMPARE(gb->get_curves_count(),    5);

    for(int n=0; n<1000; n++)
    {
        QCOMPARE(gb->add_curve_data(0, n),  true);
    }

    qreal x = 0;
    for(int n=0; n<1000; n++)
    {
        gb->get_curve_data(0, n, &x);
        QCOMPARE((int)x, n);
    }
}
#endif
//--------------------------------------------------------------------------------
