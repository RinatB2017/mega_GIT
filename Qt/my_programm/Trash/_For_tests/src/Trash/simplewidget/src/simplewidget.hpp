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
#ifndef SIMPLEWIDGET_HPP
#define SIMPLEWIDGET_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class SimpleWidget;
}
//--------------------------------------------------------------------------------
class SimpleWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int x_value READ read_x WRITE write_x NOTIFY x_value_changed)
    Q_CLASSINFO("version", "666")

signals:
    void x_value_changed(int);

public:
    explicit SimpleWidget(QWidget *parent = nullptr);
    ~SimpleWidget();

    int getValue() const;
    void setValue(int value);

private:
    Ui::SimpleWidget *ui;

    int x_value = 0;
    int read_x(void)
    {
        return x_value;
    }
    void write_x(int new_value)
    {
        if(x_value == new_value)
        {
            return;
        }
        x_value = new_value;
        emit x_value_changed(x_value);
    }

    int value = 0;

    void init(void);
    void click(void);

    void value_changed(void);
};
//--------------------------------------------------------------------------------
#endif // SIMPLEWIDGET_HPP
