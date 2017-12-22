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

#include <QTest>
//--------------------------------------------------------------------------------
#ifdef USE_B588
    #include "b588.hpp"
#endif
#ifdef USE_B590
    #include "b590.hpp"
#endif
//--------------------------------------------------------------------------------
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
void Test::test_GUI(void)
{
    QList<QWidget *> tlw = qApp->allWidgets();

    foreach (QWidget *w, tlw)
    {
        if (w->objectName() == "cb_test")
        {
            QTest::keyClick(w, Qt::Key_Down);
        }
        if (w->objectName() == "btn_choice_test")
        {
            QTest::mouseClick(w, Qt::LeftButton);
        }
    }
}
//--------------------------------------------------------------------------------
void Test::test_func(void)
{
#ifdef USE_B590
    B590 *mb = new B590(0);
    QCOMPARE(mb->xxx(5), 10);
    QCOMPARE(mb->xxx(10), 15);
#endif

#ifdef USE_B588
    B588 *mb = new B588(0);
    QCOMPARE(mb->xxx(5), 10);
    QCOMPARE(mb->xxx(10), 15);
#endif

    //QTest::qWait(5000);
}
//--------------------------------------------------------------------------------
