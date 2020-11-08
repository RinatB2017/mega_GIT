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
#include <QSignalSpy>
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_SerialBox5_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "serialbox5_lite.hpp"
#include "serialbox5_fix_baudrate.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::test_signals(void)
{
    SerialBox5 *sb                              = mw->findChild<SerialBox5 *>("serial_widget");
    SerialBox5_lite *sb_lite                    = mw->findChild<SerialBox5_lite *>("serial_widget_lite");
    SerialBox5_fix_baudrate *sb_fix_br          = mw->findChild<SerialBox5_fix_baudrate *>("serial_widget_fix_baudrate");

    QVERIFY(sb);
    QVERIFY(sb_lite);
    QVERIFY(sb_fix_br);

    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    QSignalSpy spy_info(mb,  SIGNAL(info(const QString &)));
    QSignalSpy spy_debug(mb, SIGNAL(debug(const QString &)));
    QSignalSpy spy_error(mb, SIGNAL(error(const QString &)));
    QSignalSpy spy_trace(mb, SIGNAL(trace(const QString &)));
    QSignalSpy spy_color(mb, SIGNAL(colorLog(const QString &, const QColor, const QColor)));

    QCOMPARE(spy_info.isValid(),  true);
    QCOMPARE(spy_debug.isValid(), true);
    QCOMPARE(spy_error.isValid(), true);
    QCOMPARE(spy_trace.isValid(), true);
    QCOMPARE(spy_color.isValid(), true);
}
//--------------------------------------------------------------------------------
