/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef SERIALPORT_V764_2_TEST_PARAM_HPP
#define SERIALPORT_V764_2_TEST_PARAM_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class SerialPort_H4_X_test_param;
}
//--------------------------------------------------------------------------------
class SerialPort_H4_X_test_param : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPort_H4_X_test_param(QWidget *parent = nullptr);
    ~SerialPort_H4_X_test_param();

    void f_info(const QString  &message);
    void f_debug(const QString &message);
    void f_error(const QString &message);
    void f_trace(const QString &message);

private:
    Ui::SerialPort_H4_X_test_param *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALPORT_V764_2_TEST_PARAM_HPP
