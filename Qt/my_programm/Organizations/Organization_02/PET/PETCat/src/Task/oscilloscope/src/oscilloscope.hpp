/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef OSCILLOSCOPE_HPP
#define OSCILLOSCOPE_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
#include <QFile>
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class OscilloscopeBox;
}
//--------------------------------------------------------------------------------
class Packet_analizer;
class GrapherBox;
class QTimer;
class QSplitter;
//--------------------------------------------------------------------------------
class OscilloscopeBox : public MyWidget
{
    Q_OBJECT

public:
    explicit OscilloscopeBox(QList<pet_discovery_data> pet_config,
                             QWidget *parent);
    ~OscilloscopeBox();

public slots:
    void update(void);
    void show_data(pet_wave_point sample, int index);
    void set_auto_mode(int index);
    void run(void);

private slots:
    void get_packet(QHostAddress host, quint16 port, QByteArray packet);
    void redraw_grapher_axis_x(int value);

    void grapher_refresh(void);
    void set_grapher_axes_y(void);

    void save_picture_to_file(void);
    void save_picture_to_file_old(void);

private:
    Ui::OscilloscopeBox *ui;

    QTimer *timer;

    PET_device_brg *pet_brg;
    QList<pet_discovery_data> p_pet_config;

    QSplitter *main_splitter;

    GrapherBox *grapher;
    int curve_xp = 0;
    int curve_xn = 0;
    int curve_yp = 0;
    int curve_yn = 0;

    int curve_sum = 0;

    bool is_ok = false;
    int cnt_samples = 0;

    bool cmd_WAVE(void);

    void init(void);
    void block_interface_widgets(bool state);
};
//--------------------------------------------------------------------------------
#endif
