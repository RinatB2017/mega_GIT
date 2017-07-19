/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QMouseEvent>
#include <QVariant>
//--------------------------------------------------------------------------------
#include "diod.hpp"
//--------------------------------------------------------------------------------
Diod::Diod(QWidget *parent) :
    QToolButton(parent)
{
    state = DIOD_NONE;

    setProperty("ID", QVariant(state));
    setFixedSize(WIDTH, HEIGHT);
}
//--------------------------------------------------------------------------------
void Diod::set_state(int state)
{
    this->state = state;
    setProperty("ID", QVariant(state));
    draw_state();
}
//--------------------------------------------------------------------------------
void Diod::draw_state(void)
{
    switch(state)
    {
    case DIOD_R:
        setStyleSheet("background:red;");
        setText("R");
        break;

    case DIOD_G:
        setStyleSheet("background:green;");
        setText("G");
        break;

    case DIOD_B:
        setStyleSheet("background:blue;");
        setText("B");
        break;

    default:
        setStyleSheet("");
        setText("");
        break;
    }
}
//--------------------------------------------------------------------------------
void Diod::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        switch(state)
        {
        case DIOD_R:
            state = DIOD_G;
            break;

        case DIOD_G:
            state = DIOD_B;
            break;

        case DIOD_B:
            state = DIOD_NONE;
            break;

        default:
            state = DIOD_R;
            break;
        }

        setProperty("ID", QVariant(state));
        draw_state();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
