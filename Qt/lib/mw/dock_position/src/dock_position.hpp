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
#ifndef DOCK_POSITION_HPP
#define DOCK_POSITION_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class Dock_position;
}
//--------------------------------------------------------------------------------
class Dock_position : public QWidget
{
    Q_OBJECT

public:
    explicit Dock_position(const QString &doc_name, QWidget *parent = nullptr);
    ~Dock_position();

private:
    Ui::Dock_position *ui;
    QString dock_name = "dock_name";

    void move_up(void);
    void move_down(void);
    void move_left(void);
    void move_right(void);
};
//--------------------------------------------------------------------------------
#endif // DOCK_POSITION_HPP
