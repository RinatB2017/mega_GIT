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
#include "test.hpp"
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "serialbox5_lite.hpp"
#include "serialbox5_fix_baudrate.hpp"
#include "serialbox5_fix_baudrate_win7.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::test_serial5(void)
{
    SerialBox5 *sb = mw->findChild<SerialBox5 *>("serial_widget");
    QVERIFY(sb);

    QCOMPARE(sb->isOpen(), false);
    QCOMPARE(sb->get_baudRate(), 9600);
}
//--------------------------------------------------------------------------------
void Test::test_serial5_lite(void)
{
    SerialBox5_lite *sb = mw->findChild<SerialBox5_lite *>("serial_widget_lite");
    QVERIFY(sb);

    QCOMPARE(sb->isOpen(), false);
    QCOMPARE(sb->get_baudRate(), 9600);
}
//--------------------------------------------------------------------------------
void Test::test_serial5_fix_baudrate(void)
{
    SerialBox5_fix_baudrate *sb = mw->findChild<SerialBox5_fix_baudrate *>("serial_widget_fix_baudrate");
    QVERIFY(sb);

    QCOMPARE(sb->isOpen(), false);
    QCOMPARE(sb->get_baudRate(), 57600);
}
//--------------------------------------------------------------------------------
void Test::test_serial5_fix_baudrate_win7(void)
{
    SerialBox5_fix_baudrate_win7 *sb = mw->findChild<SerialBox5_fix_baudrate_win7 *>("serial_widget_fix_baudrate_win7");
    QVERIFY(sb);

    QCOMPARE(sb->isOpen(), false);
    QCOMPARE(sb->get_baudRate(), 9600);
}
//--------------------------------------------------------------------------------
