/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef OSCILLOSCOPE_OPTIONS_HPP
#define OSCILLOSCOPE_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Oscilloscope_options;
}
//--------------------------------------------------------------------------------
class Oscilloscope_options : public MyWidget
{
    Q_OBJECT

public:
    explicit Oscilloscope_options(QWidget *parent = 0);
    ~Oscilloscope_options();

private slots:
    void block_wheel(void);

    void t_accept(void);

    void color_X_plus(void);
    void color_X_minus(void);
    void color_Y_plus(void);
    void color_Y_minus(void);
    void color_SUM(void);

private:
    Ui::Oscilloscope_options *ui;

    void init(void);

protected:
    bool eventFilter(QObject*, QEvent* event);

};
//--------------------------------------------------------------------------------
#endif
