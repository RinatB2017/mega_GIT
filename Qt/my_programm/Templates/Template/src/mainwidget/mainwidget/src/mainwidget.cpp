/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "mainwidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainWidget::MainWidget(QObject *parent)
    : QObject(parent)
{
    setObjectName("MainWidget");
}
//--------------------------------------------------------------------------------
MainWidget::~MainWidget()
{
#ifdef QT_DEBUG
    qDebug() << "~MainWidget()";
#endif
}
//--------------------------------------------------------------------------------
void MainWidget::get(void)
{
    x++;
    emit set(QString("%1").arg(x));
}
//--------------------------------------------------------------------------------
bool MainWidget::test_0(void)
{
    emit info("Test_0()");

    //emit error(QString("%1 %2").arg(__FILE__).arg(__LINE__));
    //emit error(QString("%1 %2").arg(__DATE__).arg(__TIME__));

    return true;
}
//--------------------------------------------------------------------------------
bool MainWidget::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainWidget::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainWidget::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainWidget::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainWidget::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
