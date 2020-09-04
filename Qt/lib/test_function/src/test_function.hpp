/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
class Test_function : public QObject
{
    Q_OBJECT

public:
    Test_function(void);

    void lineedit_set(QString name, const QString &value);
    void lineedit_set(QWidget *widget, const QString &name, const QString &value);

    QString lineedit_get(const QString &name);
    QString lineedit_get(QWidget *widget, const QString &name);

    void button_click(const QString &name);
    void button_click(QWidget *widget, const QString &name);

    void combobox_key_down_and_check_value(const QString &name, const QString &value);
    void combobox_key_down_and_check_value(QWidget *widget, const QString &name, const QString &value);

private:
    MyMainWindow *mw;
};
//--------------------------------------------------------------------------------
#endif
