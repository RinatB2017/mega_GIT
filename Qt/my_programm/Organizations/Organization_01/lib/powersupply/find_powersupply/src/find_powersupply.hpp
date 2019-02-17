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
#ifndef FIND_POWERSUPPLY_HPP
#define FIND_POWERSUPPLY_HPP
//--------------------------------------------------------------------------------
#include <QFrame>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Powersupply_B588;
class Powersupply_B590;

class QPushButton;
class QComboBox;
//--------------------------------------------------------------------------------
class Find_powersupply : public MyWidget
{
    Q_OBJECT
public:
    Find_powersupply(Powersupply_B590 *powersupply, QFrame *parent = nullptr);
    ~Find_powersupply();

private slots:
    void find(void);
    void update(void);

private:
    QComboBox   *cb_port_list;
    QPushButton *pb_find;
    QPushButton *pb_update_port_list;

    Powersupply_B590 *powersupply;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
