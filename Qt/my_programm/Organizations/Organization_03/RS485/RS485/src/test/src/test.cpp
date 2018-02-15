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
#include <QtWidgets>
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "test.hpp"

#include "packet.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
    QVERIFY(mw);

    mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);
}
//--------------------------------------------------------------------------------
void Test::check_test(void)
{
    QByteArray data;
    ANSWER_TEST test;

    test.body.header.prefix_16 = 0xBBAA;
    test.body.header.addr_8 = 0;
    test.body.header.cmd_8 = CMD_TEST;
    test.body.header.len_16 = sizeof(test.body.data);
    test.body.data.result = 0;

    test.body.crc16 = CRC::crc16((uint8_t *)&test.buf, sizeof(test) - sizeof(test.body.crc16));

    data.clear();
    data.append((char *)&test.buf, sizeof(test));
    QCOMPARE(mb->check_answer_test(data),   MainBox::NO_ERROR);
}
//--------------------------------------------------------------------------------
void Test::check_reset(void)
{
    QByteArray data;
    ANSWER_RESET test;
    test.body.header.prefix_16 = 0xBBAA;
    test.body.header.addr_8 = 0;
    test.body.header.cmd_8 = CMD_RESET;
    test.body.header.len_16 = sizeof(test.body.data);

    test.body.data.addr_cam_32 = 0;
    test.body.data.time_interval_16 = 0;
    test.body.data.time_washout_32 = 0;
    test.body.data.time_pause_washout_32 = 0;
    test.body.data.preset_washout_32 = 0;

    test.body.crc16 = CRC::crc16((uint8_t *)&test.buf, sizeof(test) - sizeof(test.body.crc16));

    data.clear();
    data.append((char *)&test.buf, sizeof(test));

    QCOMPARE(mb->check_answer_reset(data),  MainBox::NO_ERROR);
}
//--------------------------------------------------------------------------------
void Test::check_read(void)
{
    QByteArray data;
    ANSWER_READ test;
    test.body.header.prefix_16 = 0xBBAA;
    test.body.header.addr_8 = 0;
    test.body.header.cmd_8 = CMD_READ;
    test.body.header.len_16 = sizeof(test.body.data);

    test.body.data.addr_cam_32 = 0;
    test.body.data.time_interval_16 = 0;
    test.body.data.time_washout_32 = 0;
    test.body.data.time_pause_washout_32 = 0;
    test.body.data.preset_washout_32 = 0;

    test.body.crc16 = CRC::crc16((uint8_t *)&test.buf, sizeof(test) - sizeof(test.body.crc16));

    data.clear();
    data.append((char *)&test.buf, sizeof(test));

    QCOMPARE(mb->check_answer_read(data),   MainBox::NO_ERROR);
}
//--------------------------------------------------------------------------------
void Test::check_write(void)
{
    QByteArray data;
    ANSWER_WRITE test;
    test.body.header.prefix_16 = 0xBBAA;
    test.body.header.addr_8 = 0;
    test.body.header.cmd_8 = CMD_WRITE;
    test.body.header.len_16 = sizeof(test.body.data);

    test.body.data.addr_cam_32 = 0;
    test.body.data.time_interval_16 = 0;
    test.body.data.time_washout_32 = 0;
    test.body.data.time_pause_washout_32 = 0;
    test.body.data.preset_washout_32 = 0;

    test.body.crc16 = CRC::crc16((uint8_t *)&test.buf, sizeof(test) - sizeof(test.body.crc16));

    data.clear();
    data.append((char *)&test.buf, sizeof(test));

    QCOMPARE(mb->check_answer_write(data),  MainBox::NO_ERROR);
}
//--------------------------------------------------------------------------------
