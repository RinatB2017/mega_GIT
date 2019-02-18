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
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class GrapherBox_Options;
}
//--------------------------------------------------------------------------------
class GrapherBox_Options : public QDialog
{
    Q_OBJECT

public:
    explicit GrapherBox_Options(double min_axis_X,
                                double max_axis_X,
                                double min_axis_Y,
                                double max_axis_Y,
                                QWidget *parent = nullptr);
    ~GrapherBox_Options();

    int get_min_axis_x(void);
    int get_max_axis_x(void);
    int get_min_axis_y(void);
    int get_max_axis_y(void);

private:
    Ui::GrapherBox_Options *ui;

    double min_axis_X = 0;
    double max_axis_X = 0;
    double min_axis_Y = 0;
    double max_axis_Y = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // GRAPHERBOX_OPTIONS_HPP
