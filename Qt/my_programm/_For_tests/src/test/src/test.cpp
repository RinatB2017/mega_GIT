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
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QList>
//--------------------------------------------------------------------------------
#include <QSignalSpy>
#include <QTest>
#include "test.hpp"
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "safe.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);

    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);
    QTest::keyClick(cb, Qt::Key_Down);
    QTest::keyClick(cb, Qt::Key_Down);

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
    QTest::mouseClick(tb, Qt::LeftButton);
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    MainBox *mb = new MainBox(0, 0);
    QCOMPARE(mb->test_0(), true);
    QCOMPARE(mb->test_1(), true);
    QCOMPARE(mb->test_2(), true);
    QCOMPARE(mb->test_3(), true);
    QCOMPARE(mb->test_4(), true);
    QCOMPARE(mb->test_5(), true);

    mb->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_safe(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);

    Safe *safe = mw->findChild<Safe *>("safe");
    QVERIFY(safe);

    QToolButton *tb0 = mw->findChild<QToolButton *>("btn_0");
    QToolButton *tb1 = mw->findChild<QToolButton *>("btn_1");
    QToolButton *tb2 = mw->findChild<QToolButton *>("btn_2");
    QToolButton *tb3 = mw->findChild<QToolButton *>("btn_3");
    QToolButton *tb4 = mw->findChild<QToolButton *>("btn_4");
    QToolButton *tb5 = mw->findChild<QToolButton *>("btn_5");
    QToolButton *tb6 = mw->findChild<QToolButton *>("btn_6");
    QToolButton *tb7 = mw->findChild<QToolButton *>("btn_7");
    QToolButton *tb8 = mw->findChild<QToolButton *>("btn_8");
    QToolButton *tb9 = mw->findChild<QToolButton *>("btn_9");
    QToolButton *tb_back = mw->findChild<QToolButton *>("btn_back");
    QToolButton *tb_clear = mw->findChild<QToolButton *>("btn_clear");
    QVERIFY(tb0);
    QVERIFY(tb1);
    QVERIFY(tb2);
    QVERIFY(tb3);
    QVERIFY(tb4);
    QVERIFY(tb5);
    QVERIFY(tb6);
    QVERIFY(tb7);
    QVERIFY(tb8);
    QVERIFY(tb9);

    QVERIFY(tb_back);
    QVERIFY(tb_clear);

    safe->s_clear();
    QTest::mouseClick(tb0, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 0);
    safe->s_clear();
    QTest::mouseClick(tb1, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 1);
    safe->s_clear();
    QTest::mouseClick(tb2, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 2);
    safe->s_clear();
    QTest::mouseClick(tb3, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 3);
    safe->s_clear();
    QTest::mouseClick(tb4, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 4);
    safe->s_clear();
    QTest::mouseClick(tb5, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 5);
    safe->s_clear();
    QTest::mouseClick(tb6, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 6);
    safe->s_clear();
    QTest::mouseClick(tb7, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 7);
    safe->s_clear();
    QTest::mouseClick(tb8, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 8);
    safe->s_clear();
    QTest::mouseClick(tb9, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 9);

    QTest::mouseClick(tb_clear, Qt::LeftButton);
    QCOMPARE(safe->get_value(), 0);


    QSignalSpy spy(safe, SIGNAL(info(QString)));
    emit safe->info("test info");
    QList<QVariant> firstCallArgs = spy.at(0);
    QCOMPARE(firstCallArgs.length(), 1);
}
//--------------------------------------------------------------------------------
