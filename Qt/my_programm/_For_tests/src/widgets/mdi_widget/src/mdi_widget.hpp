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
#ifndef MDI_WIDGET_HPP
#define MDI_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QMdiSubWindow>
//--------------------------------------------------------------------------------
#include "default_widget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MDI_widget;
}
//--------------------------------------------------------------------------------
class MDI_widget : public Default_widget
{
    Q_OBJECT

public:
    explicit MDI_widget(QWidget *parent = nullptr);
    virtual ~MDI_widget();

private:
    Ui::MDI_widget *ui;
    QPointer <QLineEdit> mdi_widget_0;
    QPointer <QLineEdit> mdi_widget_1;
    QPointer <QLineEdit> mdi_widget_2;

    QPointer <QMdiSubWindow> w0;
    QPointer <QMdiSubWindow> w1;
    QPointer <QMdiSubWindow> w2;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // MDI_WIDGET_HPP
