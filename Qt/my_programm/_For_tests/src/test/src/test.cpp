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
#include "test_function.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
#include "mainbox.hpp"
#include "safe.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MyMainWindow *>(qApp->activeWindow());
    QVERIFY(mw);

    tf = new Test_function;
    QVERIFY(tf);
}
//--------------------------------------------------------------------------------
Test::~Test()
{
    if(tf)
    {
        tf->deleteLater();
    }
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
    for(int n=1; n<7; n++)
    {
        tf->combobox_key_down_and_check_value("cb_test", QString("test %1").arg(n));
    }

    tf->lineedit_set("le_0", "Text 0");
    tf->lineedit_set("le_1", "Text 1");

    QCOMPARE(tf->lineedit_get("le_0"), "Text 0");
    QCOMPARE(tf->lineedit_get("le_1"), "Text 1");

    tf->pushbutton_click("btn_0");
    tf->pushbutton_click("btn_0");
    tf->pushbutton_click("btn_0");
    tf->pushbutton_click("btn_0");
    tf->pushbutton_click("btn_0");

    for(int n=1; n<10; n++)
    {
        tf->toolbutton_click(QString("btn_dock_%1").arg(n));
    }
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
    test_mainbox();
    test_slider();

    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    QCOMPARE(mb->test(QByteArray::fromHex("000102030405")), 15);
    QCOMPARE(mb->test(QByteArray::fromHex("1F1F1F1F1F1F")), 6*0x1F);

    TEST test;
    test.reserved = 0;
    test.addr = 1;
    test.cmd  = 2;
    test.data = 3;

    QCOMPARE(sizeof(TEST), 5);

    QByteArray ba;
    ba.append((char *)&test, sizeof(test));
    QCOMPARE(mb->test(ba), 6);

    int a = 5;
    int b = 6;
    int c = 0;
    int d = 0;
    QCOMPARE(mb->test2(a, b, &c, &d), true);
    QCOMPARE(a, c);
    QCOMPARE(b, d);
}
//--------------------------------------------------------------------------------
void Test::test_safe(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    Safe *safe = mb->findChild<Safe *>("safe");
    QVERIFY(safe);

    for(int n=0; n<10; n++)
    {
        safe->s_clear();
        tf->toolbutton_click(QString("btn_%1").arg(n));
        QCOMPARE(safe->get_value(), n);
    }

    tf->toolbutton_click("btn_back");
    QCOMPARE(safe->get_value(), 0);
    tf->toolbutton_click("btn_clear");
    QCOMPARE(safe->get_value(), 0);

    tf->toolbutton_click("btn_1");
    tf->toolbutton_click("btn_2");
    tf->toolbutton_click("btn_3");
    tf->toolbutton_click("btn_4");
    tf->toolbutton_click("btn_5");
    QCOMPARE(safe->get_value(), 12345);

    tf->toolbutton_click("btn_clear");
    QCOMPARE(safe->get_value(), 0);

    tf->toolbutton_click("btn_9");
    tf->toolbutton_click("btn_9");
    tf->toolbutton_click("btn_9");
    tf->toolbutton_click("btn_9");
    tf->toolbutton_click("btn_9");
    QCOMPARE(safe->get_value(), 99999);

    QSignalSpy spy(safe, SIGNAL(info(QString)));
    emit safe->info("test info");
    QList<QVariant> firstCallArgs = spy.at(0);
    QCOMPARE(firstCallArgs.length(), 1);
}
//--------------------------------------------------------------------------------
void Test::test_safe_gui(void)
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    Safe *safe = mb->findChild<Safe *>("safe");
    QVERIFY(safe);

    QLCDNumber *display = mw->findChild<QLCDNumber *>("lcdNumber");
    QCOMPARE(display->value(),  99999);
}
//--------------------------------------------------------------------------------
void Test::simple_test(void)
{
    float x = 0;
    QBENCHMARK {
        for(int n=0; n<1e6; n++) x+=sin(n);
    }
}
//--------------------------------------------------------------------------------
