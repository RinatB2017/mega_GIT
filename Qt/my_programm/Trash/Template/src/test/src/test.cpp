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
#include <QTest>
//--------------------------------------------------------------------------------
#define private public
//--------------------------------------------------------------------------------
#include "mainwidget_gui.hpp"
#include "mainwindow.hpp"
#include "mainwidget.hpp"
//--------------------------------------------------------------------------------
#include "test_function.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{
    mw = dynamic_cast<MainWindow *>(qApp->activeWindow());
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
void Test::test_GUI(void)
{
    tf->combobox_key_down_and_check_value("cb_test", "test");
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
    MainWidget *mb = mw->findChild<MainWidget *>("MainWidget");
    QVERIFY(mb);

    QCOMPARE(mb->test(), true);
}
//--------------------------------------------------------------------------------
