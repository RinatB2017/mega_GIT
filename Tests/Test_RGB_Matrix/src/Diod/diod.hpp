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
#ifndef DIOD_HPP
#define DIOD_HPP
//--------------------------------------------------------------------------------
#include <QToolButton>
//--------------------------------------------------------------------------------
enum {
    DIOD_NONE = 0,
    DIOD_R,
    DIOD_G,
    DIOD_B
};
//--------------------------------------------------------------------------------
#define WIDTH  32
#define HEIGHT 32
//--------------------------------------------------------------------------------
class Diod : public QToolButton
{
public:
    explicit Diod(QWidget *parent);
    void set_state(int state);

private:
    int state = 0;

    void draw_state(void);

protected:
    void mousePressEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
