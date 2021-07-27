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
#ifndef WIDGETONWIDGET_HPP
#define WIDGETONWIDGET_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class WidgetOnWidget;
}
//--------------------------------------------------------------------------------
class WidgetOnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetOnWidget(QWidget *parent = nullptr);
    ~WidgetOnWidget();

private:
    Ui::WidgetOnWidget *ui;
    QPointer<QPushButton> my_button;

    void init(void);

protected:
    void resizeEvent (QResizeEvent * event);
};
//--------------------------------------------------------------------------------
#endif // WIDGETONWIDGET_HPP
