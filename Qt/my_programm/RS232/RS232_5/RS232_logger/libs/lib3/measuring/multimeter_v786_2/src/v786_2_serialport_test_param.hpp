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
#ifndef V786_2_SERIALPORT_TEST_PARAM_HPP
#define V786_2_SERIALPORT_TEST_PARAM_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class SerialPort_V786_2_test_param;
}
//--------------------------------------------------------------------------------
class SerialPort_V786_2_test_param : public QWidget
{
    Q_OBJECT

signals:
    void s_get(void);
    void s_set(void);

public:
    explicit SerialPort_V786_2_test_param(QWidget *parent = nullptr);
    ~SerialPort_V786_2_test_param();

    void set_serno(int value);
    void set_year(int value);
    void set_address(int value);
    void set_channel(int value);

    int get_serno(void);
    int get_year(void);
    int get_address(void);
    int get_channel(void);

private:
    Ui::SerialPort_V786_2_test_param *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // V786_2_SERIALPORT_TEST_PARAM_HPP
