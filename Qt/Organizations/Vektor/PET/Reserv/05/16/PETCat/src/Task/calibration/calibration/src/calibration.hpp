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
#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP
//--------------------------------------------------------------------------------
#include <QUdpSocket>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
namespace Ui {
class Calibration;
}
//--------------------------------------------------------------------------------
class Calibration_channel;
class Calibration_param;
class Imitator_BRG;
//--------------------------------------------------------------------------------
class Calibration : public MyWidget
{
    Q_OBJECT

public:
    explicit Calibration(QList<pet_discovery_data> pet_config,
                         QWidget *parent);
    ~Calibration();

private slots:
    void test(void);

    void start(void);
    void stop(void);

    void select(void);
    void refresh(void);
    void apply(void);

    void run_calib_coords(int num_channel);

private:
    Ui::Calibration *ui = 0;

    //---
#ifdef Q_OS_LINUX
    uint8_t output_buf[128];
#endif
#ifdef Q_OS_WIN
    uint8_t output_buf[256];
#endif

    Calibration_channel *channel_1 = 0;
    Calibration_channel *channel_2 = 0;
    Calibration_channel *channel_3 = 0;
    Calibration_channel *channel_4 = 0;

    Calibration_param *c_param = 0;

    Imitator_BRG *imitator_brg = 0;

    int curve_data = 0;
    int curve_ideal = 0;

    PET_comm *comm = 0;
    PET_device_brg *pet_brg = 0;
    QList<pet_discovery_data> p_pet_config;

    int channel = 0;
    int max_point = 0;
    int max_cnt_read = 0;
    int max_cnt_calib = 0;
    int timeout = 0;
    double min_A = 0;
    double max_A = 0;
    double max_delta_B = 0;
    double max_delta_K = 0;
    double freq = 0;

    void init(void);
    bool brg_correct(double value_K, double value_B);
    bool get_k_b(double *value_K, double *value_B);
    bool set_voltage(double value);
    bool get_voltage(double *value);
    bool get_raw_voltage(double *value);

    void getApprox(double **x,
                   double *a,
                   double *b,
                   int n);

};
//--------------------------------------------------------------------------------
#endif
