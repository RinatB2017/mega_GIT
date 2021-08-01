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
#ifndef OM_WIDGET_HPP
#define OM_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class OM_widget;
}
//--------------------------------------------------------------------------------
class OM_widget : public QWidget
{
    Q_OBJECT

public:
    explicit OM_widget(QWidget *parent = nullptr);
    virtual ~OM_widget();

private:
    Ui::OM_widget *ui;

    void init(void);

protected:
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // OM_WIDGET_HPP
