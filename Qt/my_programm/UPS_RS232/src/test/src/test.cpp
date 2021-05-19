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
#include "serialbox5_fix_baudrate.hpp"
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
    SerialBox5_fix_baudrate *sb1 = mw->findChild<SerialBox5_fix_baudrate *>("RS232_5");
    QVERIFY(sb1);

    QPushButton *pb1 = sb1->findChild<QPushButton *>("btn_power");
    QVERIFY(pb1);

    QTest::mouseClick(pb1, Qt::LeftButton);
    QCOMPARE(sb1->isOpen(),  true);

    QSignalSpy spy_port_open(sb1,           SIGNAL(port_open()));
    QSignalSpy spy_port_close(sb1,          SIGNAL(port_close()));
    QSignalSpy spy_readyRead(sb1,           SIGNAL(readyRead()));
    QSignalSpy spy_readChannelFinished(sb1, SIGNAL(readChannelFinished()));
    QSignalSpy spy_output(sb1,              SIGNAL(output(QByteArray)));
    QSignalSpy spy_port_is_active(sb1,      SIGNAL(port_is_active(bool)));

    QSignalSpy spy_baudRateChanged(sb1,         SIGNAL(baudRateChanged(qint32, QSerialPort::Directions)));
    QSignalSpy spy_breakEnabledChanged(sb1,     SIGNAL(breakEnabledChanged(bool)));
    QSignalSpy spy_dataBitsChanged(sb1,         SIGNAL(dataBitsChanged(QSerialPort::DataBits)));
    QSignalSpy spy_dataTerminalReadyChanged(sb1,SIGNAL(dataTerminalReadyChanged(bool)));
    QSignalSpy spy_flowControlChanged(sb1,      SIGNAL(flowControlChanged(QSerialPort::FlowControl)));
    QSignalSpy spy_parityChanged(sb1,           SIGNAL(parityChanged(QSerialPort::Parity)));
    QSignalSpy spy_requestToSendChanged(sb1,    SIGNAL(requestToSendChanged(bool)));
    QSignalSpy spy_stopBitsChanged(sb1,         SIGNAL(stopBitsChanged(QSerialPort::StopBits)));

    QCOMPARE(spy_port_open.isValid(),           true); // signal exists
    QCOMPARE(spy_port_close.isValid(),          true); // signal exists
    QCOMPARE(spy_readyRead.isValid(),           true); // signal exists
    QCOMPARE(spy_readChannelFinished.isValid(), true); // signal exists
    QCOMPARE(spy_output.isValid(),              true); // signal exists
    QCOMPARE(spy_port_is_active.isValid(),      true); // signal exists

    QCOMPARE(spy_baudRateChanged.isValid(),             true); // signal exists
    QCOMPARE(spy_breakEnabledChanged.isValid(),         true); // signal exists
    QCOMPARE(spy_dataBitsChanged.isValid(),             true); // signal exists
    QCOMPARE(spy_dataTerminalReadyChanged.isValid(),    true); // signal exists
    QCOMPARE(spy_flowControlChanged.isValid(),          true); // signal exists
    QCOMPARE(spy_parityChanged.isValid(),               true); // signal exists
    QCOMPARE(spy_requestToSendChanged.isValid(),        true); // signal exists
    QCOMPARE(spy_stopBitsChanged.isValid(),             true); // signal exists

    QTest::qWait(1000);

    QTest::mouseClick(pb1, Qt::LeftButton);
    QCOMPARE(sb1->isOpen(),  false);
}
//--------------------------------------------------------------------------------
