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
#ifndef CALIBRATION_CHANNEL_HPP
#define CALIBRATION_CHANNEL_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "pet_brg.h"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Calibration_channel;
}
//--------------------------------------------------------------------------------
class Calibration_channel : public MyWidget
{
    Q_OBJECT

public:
    explicit Calibration_channel(int num_channel, QWidget *parent = 0);
    ~Calibration_channel();

    bool set(pet_brg_ch_calibration data);
    bool get(pet_brg_ch_calibration *data);

signals:
    void run_calib_coord(int num_channel);
    void run_calib_adc_ref(void);
    void run_calib_ifmodule_temp(void);
    void run_calib_offset(void);
    void run_calib_pm_bias(void);
    void run_calib_readout_temp(void);
    void run_calib_threshold(void);
    void run_calib_ts_offset(void);

private slots:
    void run_calib_coord(void);

private:
    Ui::Calibration_channel *ui;

    int channel = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // CALIBRATION_CHANNEL_HPP
