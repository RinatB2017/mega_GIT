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
    QCOMPARE(proto->test_GGA(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_GSA(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_GSV(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_RMC(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_VTG(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_GLL(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_ZDA(), (int)Proto_NMEA_0183::E_NO_ERROR);

    QCOMPARE(proto->test_PIREA(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_PIRFV(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_PIRGK(), (int)Proto_NMEA_0183::E_NO_ERROR);
    QCOMPARE(proto->test_PIRRA(), (int)Proto_NMEA_0183::E_NO_ERROR);

    proto->deleteLater();
}
//--------------------------------------------------------------------------------
