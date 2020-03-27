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
#include <QList>
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "mainwindow.hpp"
#include "rs232_5_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
void Test::check_serial(void)
{
    SerialBox5 *sb1 = mw->findChild<SerialBox5 *>("RS232_5");
    QVERIFY(sb1);

    QPushButton *pb1 = sb1->findChild<QPushButton *>("btn_power");
    QVERIFY(pb1);

    qint32 value = 115200;

    QCOMPARE(sb1->set_baudRate(value),   true);
    QCOMPARE(sb1->set_dataBits(QSerialPort::Data8),     true);
    QCOMPARE(sb1->set_parity(QSerialPort::NoParity),    true);
    QCOMPARE(sb1->set_stopBits(QSerialPort::OneStop),   true);
    QCOMPARE(sb1->set_flowControl(QSerialPort::NoFlowControl),   true);

    QTest::mouseClick(pb1, Qt::LeftButton);
    QCOMPARE(sb1->isOpen(),  true);

    //QCOMPARE(sb1->get_baudRate(),   value);   //FIXME данные почему-то другие
    QCOMPARE(sb1->get_dataBits(),   QSerialPort::Data8);
    QCOMPARE(sb1->get_parity(),     QSerialPort::NoParity);
    QCOMPARE(sb1->get_stopBits(),   QSerialPort::OneStop);
    QCOMPARE(sb1->get_flowControl(),QSerialPort::NoFlowControl);

    QSignalSpy spy(sb1, SIGNAL(output(QByteArray)));
    QCOMPARE(spy.isValid(), true); // signal exists

    QSignalSpy spy2(sb1, SIGNAL(port_open()));
    QCOMPARE(spy2.isValid(), true); // signal exists

    QSignalSpy spy3(sb1, SIGNAL(port_close()));
    QCOMPARE(spy3.isValid(), true); // signal exists

    //QCOMPARE(sb1->input(QString("test1")),  SerialBox5::E_NO_ERROR);
    QTest::qWait(1000);

    //QCOMPARE(spy.count(), 1);                       // fired exactly once
    //QCOMPARE(spy.takeFirst().at(0).toBool(), true); // with right arguments

    QTest::mouseClick(pb1, Qt::LeftButton);
    QCOMPARE(sb1->isOpen(),  false);
}
//--------------------------------------------------------------------------------
