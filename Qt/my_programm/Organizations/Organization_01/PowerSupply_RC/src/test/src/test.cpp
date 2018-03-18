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
#include "mainwindow.hpp"
#include "b590.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
#if 0
    MainWidget *mb = mw->findChild<MainWidget *>("MainWidget");
    QVERIFY(mb);

    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);
    QTest::keyClick(cb, Qt::Key_Down);
    QTest::keyClick(cb, Qt::Key_Down);

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
    QTest::mouseClick(tb, Qt::LeftButton);
    
    QCOMPARE(mb->test_0(), true);
    QCOMPARE(mb->test_1(), true);
    QCOMPARE(mb->test_2(), true);
    QCOMPARE(mb->test_3(), true);
    QCOMPARE(mb->test_4(), true);
    QCOMPARE(mb->test_5(), true);
#endif
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
    
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    B590 *mb = mw->findChild<B590 *>("B590");
    QVERIFY(mb);

//    QCOMPARE(mb->search_power_supply(), true);
//    QCOMPARE(mb->rc_on(),   true);
//    QCOMPARE(mb->rc_off(),  true);

//    QCOMPARE(mb->test_U(),  0);
//    QCOMPARE(mb->test_I(),  0);

//    QCOMPARE(mb->send_0_0(),    E_B590_NO_ERROR);

//    QCOMPARE(mb->set_vent_speed(),      E_B590_NO_ERROR);
//    QCOMPARE(mb->set_vent_speed_0(),    E_B590_NO_ERROR);
//    QCOMPARE(mb->set_vent_speed_max(),  E_B590_NO_ERROR);
//    QCOMPARE(mb->set_vent_speed_auto(), E_B590_NO_ERROR);

//    QCOMPARE(mb->set_UI_parrot(0, 0),   E_B590_NO_ERROR);
}
//--------------------------------------------------------------------------------
