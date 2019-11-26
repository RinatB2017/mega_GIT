/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef LCD_WIDGET_HPP
#define LCD_WIDGET_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class LCD_widget;
}
//--------------------------------------------------------------------------------
class LCD_widget : public QWidget
{
    Q_OBJECT

public:
    explicit LCD_widget(QWidget *parent = nullptr);
    ~LCD_widget();

    bool display(QString object_name, qreal value);
    bool display(QString object_name, QString value);

private:
    Ui::LCD_widget *ui;
    QList<QLCDNumber *> l_lcd;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // LCD_WIDGET_HPP
