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
#include <QSignalSpy>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QList>
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "mainwindow.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::check_serial_1(void)
{
    SerialBox5 *sb1 = mw->findChild<SerialBox5 *>("RS-232_1");
    QVERIFY(sb1);

    QPushButton *pb1 = sb1->findChild<QPushButton *>("btn_power");
    QVERIFY(pb1);
    QTest::mouseClick(pb1, Qt::LeftButton);

    QCOMPARE(sb1->isOpen(),  true);

    QSignalSpy spy(sb1, SIGNAL(output(QByteArray)));
    QCOMPARE(spy.isValid(), true); // signal exists

    QSignalSpy spy2(sb1, SIGNAL(not_working()));
    QCOMPARE(spy2.isValid(), true); // signal exists

    QCOMPARE(sb1->input(QString("test1")),  SerialBox5::E_NO_ERROR);
    QTest::qWait(1000);

    QCOMPARE(spy.count(), 1);                       // fired exactly once
    QCOMPARE(spy.takeFirst().at(0).toBool(), true); // with right arguments

    QTest::mouseClick(pb1, Qt::LeftButton);
    QCOMPARE(sb1->isOpen(),  false);
    QTest::qWait(1000);
}
//--------------------------------------------------------------------------------
void Test::check_serial_2(void)
{
    SerialBox5 *sb2 = mw->findChild<SerialBox5 *>("RS-232_2");
    QVERIFY(sb2);

    QPushButton *pb2 = sb2->findChild<QPushButton *>("btn_power");
    QVERIFY(pb2);
    QTest::mouseClick(pb2, Qt::LeftButton);

    QCOMPARE(sb2->isOpen(),  true);

    QSignalSpy spy(sb2, SIGNAL(output(QByteArray)));
    QCOMPARE(spy.isValid(), true); // signal exists

    QSignalSpy spy2(sb2, SIGNAL(not_working()));
    QCOMPARE(spy2.isValid(), true); // signal exists

    QCOMPARE(sb2->input(QString("test2")),  SerialBox5::E_NO_ERROR);
    QTest::qWait(1000);

    QCOMPARE(spy.count(), 1);                       // fired exactly once
    QCOMPARE(spy.takeFirst().at(0).toBool(), true); // with right arguments

    QTest::mouseClick(pb2, Qt::LeftButton);
    QCOMPARE(sb2->isOpen(),  false);
    QTest::qWait(1000);
}
//--------------------------------------------------------------------------------
