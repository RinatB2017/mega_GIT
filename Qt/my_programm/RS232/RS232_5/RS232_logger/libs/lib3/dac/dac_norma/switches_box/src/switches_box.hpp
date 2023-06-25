/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef SWITCHES_BOX_HPP
#define SWITCHES_BOX_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
#include "packets.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class Switches_box;
}
//--------------------------------------------------------------------------------
class Switches_box : public QDialog
{
    Q_OBJECT

public:
    explicit Switches_box(QWidget *parent = nullptr);
    ~Switches_box();

    void set(SWITCH data);
    SWITCH get(void);

private:
    Ui::Switches_box *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // SWITCHES_BOX_HPP
