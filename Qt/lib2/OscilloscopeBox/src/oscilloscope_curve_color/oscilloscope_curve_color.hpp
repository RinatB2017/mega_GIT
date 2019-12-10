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
#ifndef OSCILLOSCOPE_CURVE_HPP
#define OSCILLOSCOPE_CURVE_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class Oscilloscope_curve_color;
}
//--------------------------------------------------------------------------------
class Oscilloscope_curve_color : public QWidget
{
    Q_OBJECT

signals:

public:
    explicit Oscilloscope_curve_color(int ID, QWidget *parent = nullptr);
    ~Oscilloscope_curve_color();

    void set_color(QColor color);
    void set_button_text(QString text);

private slots:
    void set_color(void);

private:
    Ui::Oscilloscope_curve_color *ui;
    QColor color = Qt::black;
    int id = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // COLOR_CURVE_HPP
