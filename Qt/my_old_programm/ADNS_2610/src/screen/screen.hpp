/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef SCREEN_HPP
#define SCREEN_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
class Screen : public MyWidget
{
    Q_OBJECT

public:
    Screen(QWidget *parent);

    void draw(QByteArray data);

private:
    uint8_t buf[MAX_SIZE];

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
