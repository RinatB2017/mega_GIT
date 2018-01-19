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
#ifndef OSCILLOSCOPE_CONTROLS_HPP
#define OSCILLOSCOPE_CONTROLS_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class Oscilloscope_controls;
}
//--------------------------------------------------------------------------------
class Oscilloscope_controls : public QWidget
{
    Q_OBJECT

public:
    Oscilloscope_controls(QWidget *parent = 0);
    ~Oscilloscope_controls();

signals:
    void click_channel(int channel);

    void click_RUN(void);

    void knob_position_changed(double);
    void knob_multiply_changed(double);

public slots:
    void set_position(double value);
    void set_multiply(double value);

    void set_max_position(double max_value);
    void set_max_multiply(double max_value);

    void set_background_channel(int channel, bool state);

    void set_state_RUN(bool state);

private slots:
    void click_CH1(void);
    void click_CH2(void);
    void click_CH3(void);
    void click_CH4(void);

private:
    Ui::Oscilloscope_controls *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // OSCILLOSCOPE_CONTROLS_HPP
