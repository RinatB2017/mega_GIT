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
#ifndef TABLE_WIDGET_HPP
#define TABLE_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QStandardItemModel>
//--------------------------------------------------------------------------------
#include "default_widget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Table_widget;
}
//--------------------------------------------------------------------------------
class Table_widget : public Default_widget
{
    Q_OBJECT

public:
    explicit Table_widget(QWidget *parent = nullptr);
    virtual ~Table_widget();

private:
    Ui::Table_widget *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // TABLE_WIDGET_HPP
