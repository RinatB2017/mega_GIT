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
#include "mainwindow.hpp"
#include "grapherbox.hpp"
#include "test_GrapherBox_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = reinterpret_cast<MainWindow *>(qApp->activeWindow());
    Q_ASSERT(mw);

    mb = mw->findChild<MainBox *>();
    Q_ASSERT(mb);

    gb = mw->findChild<GrapherBox *>();
    Q_ASSERT(gb);
}
//--------------------------------------------------------------------------------
void Test::test_grapherbox(void)
{
    QTimer::singleShot(0, [this] {
        const int cnt_buf = 1024;
        qreal test_buf[cnt_buf] = { 0 };
        for(int n=0; n<cnt_buf; n++)
        {
            test_buf[n] = static_cast<qreal>(rand() % 1000) + 0.12345;
            gb->add_curve_data(0, n, test_buf[n]);
        }

        qreal data;
        bool ok;
        for(int n=0; n<cnt_buf; n++)
        {
            ok = gb->get_curve_data(0, n, &data);
            QCOMPARE(ok, true);
            QCOMPARE(data, test_buf[n]);
        }
        ok = gb->get_silense();
        gb->set_silense(true);
        gb->clear();
        gb->set_silense(ok);
    });
}
//--------------------------------------------------------------------------------
