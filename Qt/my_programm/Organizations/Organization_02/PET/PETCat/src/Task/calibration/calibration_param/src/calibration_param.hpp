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
#ifndef CALIBRATION_PARAM_HPP
#define CALIBRATION_PARAM_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Calibration_param;
}
//--------------------------------------------------------------------------------
class Calibration_param : public MyWidget
{
    Q_OBJECT

public:
    explicit Calibration_param(QWidget *parent = nullptr);
    ~Calibration_param();

public slots:
    double get_A_min(void);
    double get_A_max(void);
    double get_B(void);
    double get_F(void);
    double get_K(void);
    int get_N(void);
    int get_max_cnt_read(void);
    int get_max_cnt_calib(void);
    int get_timeout(void);

    void set_A_min(double value);
    void set_A_max(double value);
    void set_B(double value);
    void set_F(double value);
    void set_K(double value);
    void set_N(int value);
    void set_max_cnt_read(int value);
    void set_max_cnt_calib(int value);
    void set_timeout(int value);

private:
    Ui::Calibration_param *ui;

    void init(void);

};
//--------------------------------------------------------------------------------
#endif // CALIBRATION_PARAM_HPP
