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
#ifndef MYLISTWIDGET_HPP
#define MYLISTWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QListWidget>
#include <QKeyEvent>
#include <QMenu>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class MyListWidget;
}
//--------------------------------------------------------------------------------
class MyListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MyListWidget(QWidget *parent = nullptr);
    virtual ~MyListWidget();

private:
    void init(void);
    void popup(QPoint);

protected:
    void keyPressEvent(QKeyEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MYLISTWIDGET_HPP
