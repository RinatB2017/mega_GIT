/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef INVENTORY_WIDGET_HPP
#define INVENTORY_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QSound>
#include <QDebug>

#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Inventory_widget;
}
//--------------------------------------------------------------------------------
class Inventory_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Inventory_widget(QWidget *parent = nullptr);
    ~Inventory_widget();

private:
    Ui::Inventory_widget *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // INVENTORY_WIDGET_HPP
