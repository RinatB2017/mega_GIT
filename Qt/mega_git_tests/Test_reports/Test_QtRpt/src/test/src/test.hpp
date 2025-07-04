/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#ifndef TEST_HPP
#define TEST_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QTest>
//--------------------------------------------------------------------------------
class MainWindow;
//--------------------------------------------------------------------------------
class Test : public QObject {
    Q_OBJECT

public:
    Test();

    void setMainWindow(MainWindow *mainWindow);

private slots:
    void initTestCase(void);
    void test_GUI(void);
    void test_func(void);
    void test_signals(void);

private:
    MainWindow *mw = nullptr;
};
//--------------------------------------------------------------------------------
#endif
