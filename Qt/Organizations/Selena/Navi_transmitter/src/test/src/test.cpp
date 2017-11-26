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
#include <QTest>
#include "test.hpp"
#include "proto_NMEA_0183.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    Proto_NMEA_0183 *proto = new Proto_NMEA_0183();
    QCOMPARE(proto->test_GGA(), 0);    //E_NO_ERROR
    QCOMPARE(proto->test_GSA(), 0);
    QCOMPARE(proto->test_GSV(), 0);
    QCOMPARE(proto->test_RMC(), 0);
    QCOMPARE(proto->test_VTG(), 0);
    QCOMPARE(proto->test_GLL(), 0);
    QCOMPARE(proto->test_ZDA(), 0);

    QCOMPARE(proto->test_PIREA(), 0);
    QCOMPARE(proto->test_PIRFV(), 0);
    QCOMPARE(proto->test_PIRGK(), 0);
    QCOMPARE(proto->test_PIRRA(), 0);

    proto->deleteLater();
}
//--------------------------------------------------------------------------------
