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
#ifndef HISTOGRAM_ENERGY_HPP
#define HISTOGRAM_ENERGY_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QList>
#include <QFile>
//--------------------------------------------------------------------------------
#include "mycombobox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class Histogram_energy;
}
//--------------------------------------------------------------------------------
class PET_event_flag;
class GrapherBox;
class Client;
class DAT3;
class QSplitter;
class MetaData_widget;
//--------------------------------------------------------------------------------
class Histogram_energy : public MyWidget
{
    Q_OBJECT

public:
    explicit Histogram_energy(QString file_auto_open, QWidget *parent);
    ~Histogram_energy();

signals:
    void signal_add_metadata_file(void);

public slots:
    void refresh_metadata(void);

private slots:
    void refresh(void);

    void refresh_click(void);

    void get_event(pet_event event);
    void set_event_flags(void);

    bool check_filter_address(uint8_t filter_address, uint8_t event_address);

    void grapher_refresh(void);

    void save_picture_to_file(void);
    void save_picture_to_file_old(void);

    //---
    void source_expanded(bool state);
    void energy_expanded(bool state);
    void events_flag_expanded(bool state);
    void param_expanded(bool state);
    //---

private:
    Ui::Histogram_energy *ui;

    MetaData_widget *mtw;
    bool convert_metadata(QString name, QString *result);

    float Emin = 0;
    float Emax = 0;
    float N = 0;
    float E = 0;
    float delta = 0;

    GrapherBox *grapher;
    int curve = 0;

    bool ignore_filter_address = false;
    uint8_t filter_address = 0;
    uint8_t event_flags = 0;

    //---
    DAT3 *dat3;

    int max_i = 0;

    QSplitter *main_splitter;

    quint64 buf_energy[MAX_SIZE_BUF_ENERGY];
    void clear_buf_energy(void);

    void init(void);

    void clear_all(void);

    void init_widgets(void);
    void init_dat3(void);
    void init_grapher(void);

    bool check_event_flags(uint8_t filter_flags, uint8_t event_flags);
    bool check_filter(float E, float Emin, float Emax);
};
//--------------------------------------------------------------------------------
#endif
