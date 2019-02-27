/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef FRAME_8BIT_HPP
#define FRAME_8BIT_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QToolButton;
//--------------------------------------------------------------------------------
class Frame_8bit : public QFrame
{
    Q_OBJECT
public:
    explicit Frame_8bit(QWidget *parent = nullptr);
    void set_value(unsigned char value);
    unsigned char get_value(void);

private:
    QToolButton *btn7;
    QToolButton *btn6;
    QToolButton *btn5;
    QToolButton *btn4;
    QToolButton *btn3;
    QToolButton *btn2;
    QToolButton *btn1;
    QToolButton *btn0;

};
//--------------------------------------------------------------------------------
#endif // FRAME_8BIT_HPP
