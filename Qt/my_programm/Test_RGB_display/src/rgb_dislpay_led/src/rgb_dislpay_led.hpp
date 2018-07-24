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
#ifndef RGB_DISLPAY_LED_HPP
#define RGB_DISLPAY_LED_HPP
//--------------------------------------------------------------------------------
#include <QToolButton>
//--------------------------------------------------------------------------------
class RGB_dislpay_led : public QToolButton
{
    Q_OBJECT

public:
    explicit RGB_dislpay_led(QWidget *parent = nullptr);
    ~RGB_dislpay_led();

private:

};
//--------------------------------------------------------------------------------
#endif // RGB_DISLPAY_LED_HPP
