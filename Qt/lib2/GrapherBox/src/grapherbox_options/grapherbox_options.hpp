/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef GRAPHERBOX_OPTIONS_HPP
#define GRAPHERBOX_OPTIONS_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class GrapherBox_Options;
}
//--------------------------------------------------------------------------------
class ColorButton;
//--------------------------------------------------------------------------------
class GrapherBox_Options : public QDialog
{
    Q_OBJECT

public:
    enum Device {
        RECORDER = 0,
        OSCILLOSCOPE
    };
    enum TypeCurve {
        DOTS = 0,
        LINES,
        SPLINE_LINES
    };

    explicit GrapherBox_Options(QWidget *parent = nullptr);
    ~GrapherBox_Options();

    void set_type_device(GrapherBox_Options::Device device);
    GrapherBox_Options::Device get_type_device(void);
    void set_type_curve(GrapherBox_Options::TypeCurve type);
    GrapherBox_Options::TypeCurve get_type_curve(void);

    void set_min_axis_X(int value);
    void set_max_axis_X(int value);
    void set_min_axis_Y(int value);
    void set_max_axis_Y(int value);

    int get_min_axis_x(void);
    int get_max_axis_x(void);
    int get_min_axis_y(void);
    int get_max_axis_y(void);

    void add_color_button(QColor color, QString text);
    bool get_color(int channel, QColor *color);

private:
    Ui::GrapherBox_Options *ui;
    int cnt_channel = 0;
    QList<ColorButton *> color_buttons;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // GRAPHERBOX_OPTIONS_HPP
