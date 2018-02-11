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
#ifndef MOUSE_HPP
#define MOUSE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Mouse;
}
//--------------------------------------------------------------------------------
class QGridLayout;
//--------------------------------------------------------------------------------
class Mouse : public MyWidget
{
    Q_OBJECT

public:
    Mouse(QWidget *parent = 0);
    ~Mouse();

    bool set(int x, int y, int id);
    bool get(int x, int y, int *id);

private:
    Ui::Mouse *ui = 0;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MOUSE_HPP
