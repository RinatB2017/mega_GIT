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
//--------------------------------------------------------------------------------
//#include "test_function.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MyMainWindow *>(qApp->activeWindow());
    QVERIFY(mw);
}
//--------------------------------------------------------------------------------
Test::~Test()
{
}
//--------------------------------------------------------------------------------
void Test::test_slider(void)
{
    QSlider *slider = mw->findChild<QSlider *>("verticalSlider");
    QVERIFY(slider);

    QSignalSpy spy(slider, SIGNAL(valueChanged(int)));
    QCOMPARE(spy.isValid(), true); // signal exists

    slider->setValue(10);

    QCOMPARE(spy.count(), 1);                    // fired exactly once
    QCOMPARE(spy.takeFirst().at(0).toInt(), 10); // with right arguments
}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{

}
//--------------------------------------------------------------------------------
void Test::test_mainbox(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    QCOMPARE(mb->test_0(), true);
    QCOMPARE(mb->test_1(), true);
    QCOMPARE(mb->test_2(), true);
    QCOMPARE(mb->test_3(), true);
    QCOMPARE(mb->test_4(), true);
    QCOMPARE(mb->test_5(), true);
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    //test_mainbox();

    //MainBox *mb = mw->findChild<MainBox *>("MainBox");
    //QVERIFY(mb);

    //QCOMPARE(mb->test(QByteArray::fromHex("000102030405")), 15);
    //QCOMPARE(mb->test(QByteArray::fromHex("1F1F1F1F1F1F")), 6*0x1F);

    //TEST test;
    //test.reserved = 0;
    //test.addr = 1;
    //test.cmd  = 2;
    //test.data = 3;

    //QCOMPARE(sizeof(TEST), 5);

    //QByteArray ba;
    //ba.append((char *)&test, sizeof(test));
    //QCOMPARE(mb->test(ba), 5);
}
//--------------------------------------------------------------------------------
