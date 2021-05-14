/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef MYQDOCKWIDGET_HPP
#define MYQDOCKWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QResizeEvent>
#include <QDockWidget>
//--------------------------------------------------------------------------------
class MyQDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MyQDockWidget(QWidget *parent = nullptr);
    ~MyQDockWidget();

    void set_dock_position_widget(QWidget *widget);

private:
    QWidget *dock_position_widget = nullptr;

protected:
    void resizeEvent (QResizeEvent * event);
};
//--------------------------------------------------------------------------------
#endif // MYQDOCKWIDGET_HPP
