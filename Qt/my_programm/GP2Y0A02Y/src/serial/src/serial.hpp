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
#ifndef SERIAL_HPP
#define SERIAL_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Serial;
}
//--------------------------------------------------------------------------------
class Serial : public MyWidget
{
    Q_OBJECT

public:
    Serial(QWidget *parent = 0);
    ~Serial();

signals:
    void set_value(int channel, int value);

private slots:
    void get_data(QByteArray data);

private:
    Ui::Serial  *ui;
    QByteArray  data_rs232;

    void init(void);
    void analize(char data);
    bool work(QByteArray clean_data);

    QString convert_data_to_ascii(uint8_t data);
    uint8_t convert_ascii_to_value(char hi, char lo);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // SERIAL_HPP
//--------------------------------------------------------------------------------
