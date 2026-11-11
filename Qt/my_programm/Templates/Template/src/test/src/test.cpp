/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#include "mainwindow.hpp"
#include "template_mainbox.hpp"
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
void Test::initTestCase()
{
    QVERIFY(mw != nullptr);
    QVERIFY(QTest::qWaitForWindowExposed(mw, 1000));

    l_btn = mw->findChildren<QAbstractButton*>();
    QVERIFY2(!l_btn.isEmpty(), "Список кнопок пуст");
}
//--------------------------------------------------------------------------------
void Test::setMainWindow(MainWindow *mainWindow)
{
    mw = mainWindow;
}
//--------------------------------------------------------------------------------
void Test::check_pause(int delay_msec)
{
#ifdef DEF_NO_WAIT
    Q_UNUSED(delay_msec);
#else
    QTest::qWait(delay_msec);
#endif
}
//--------------------------------------------------------------------------------
void Test::check_btn_click(const QString &name)
{
    for (QAbstractButton *btn : l_btn)
    {
        if (btn->text() == name)
        {
            QPoint center = btn->rect().center();
            QTest::mousePress(btn, Qt::LeftButton, Qt::NoModifier, center);
            QTest::qSleep(delay_ms);
            QTest::mouseRelease(btn, Qt::LeftButton, Qt::NoModifier, center);
            QTest::qSleep(delay_ms);
            check_pause(300);
            return;
        }
    }
    QString msg = QString("Кнопка '%1' НЕ найдена!").arg(name);
    QFAIL(qPrintable(msg));
}
//--------------------------------------------------------------------------------
void Test::test_GUI()
{
    QComboBox *cb = mw->findChild<QComboBox *>("cb_test");
    QVERIFY(cb);
    QTest::keyClick(cb, Qt::Key_Down);
    QTest::keyClick(cb, Qt::Key_Down);

    QToolButton *tb = mw->findChild<QToolButton *>("btn_choice_test");
    QVERIFY(tb);
    QTest::mouseClick(tb, Qt::LeftButton);
}
//--------------------------------------------------------------------------------
void Test::test_func()
{
    MainBox *mb = mw->findChild<MainBox *>("MainBox");
    QVERIFY(mb);

    int max_click = 100;
    for(int n=0; n<max_click; n++)
    {
        check_btn_click("test");
    }
    QString msg = QString("Кнопка нажата %1 раз (%2)")
                      .arg(mb->test_cnt)
                      .arg(max_click);
    QVERIFY2(mb->test_cnt == (max_click + 1), qPrintable(msg));
}
//--------------------------------------------------------------------------------
void Test::test_signals()
{

}
//--------------------------------------------------------------------------------
