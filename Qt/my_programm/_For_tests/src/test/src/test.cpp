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
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "for_tests_mainbox.hpp"
#include "test.hpp"

#include "logbox.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
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
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);
}
//--------------------------------------------------------------------------------
void Test::test_signals(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    QSignalSpy spy_info(mb,  SIGNAL(info(const QString &)));
    QSignalSpy spy_debug(mb, SIGNAL(debug(const QString &)));
    QSignalSpy spy_error(mb, SIGNAL(error(const QString &)));
    QSignalSpy spy_trace(mb, SIGNAL(trace(const QString &)));
    QSignalSpy spy_clear_log(mb, SIGNAL(clear_log()));
    QSignalSpy spy_color(mb, SIGNAL(colorLog(const QString &, const QColor, const QColor)));

    QCOMPARE(spy_info.isValid(),  true);
    QCOMPARE(spy_debug.isValid(), true);
    QCOMPARE(spy_error.isValid(), true);
    QCOMPARE(spy_trace.isValid(), true);
    QCOMPARE(spy_clear_log.isValid(), true);
    QCOMPARE(spy_color.isValid(), true);

    //mb->clear_log();
    mb->info("test_info");
    mb->debug("test_debug");
    mb->error("test_error");
    mb->trace("test_trace");

    LogBox *lb = mw->findChild<LogBox *>("LogBox");
    QVERIFY(lb);

    QTextEdit *te = lb->findChild<QTextEdit *>("te_LogBox");
    QVERIFY(te);

    QTest::qWait(1000);

    QCOMPARE(te->toPlainText().contains("test_info"), true);
    QCOMPARE(te->toPlainText().contains("test_debug"), true);
    QCOMPARE(te->toPlainText().contains("test_error"), true);
    QCOMPARE(te->toPlainText().contains("test_trace"), true);

    mb->clear_log();
}
//--------------------------------------------------------------------------------
