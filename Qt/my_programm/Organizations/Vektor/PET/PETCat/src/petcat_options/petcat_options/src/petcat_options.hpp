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
#ifndef PETCAT_OPTIONS_HPP
#define PETCAT_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
typedef struct PETCat_options_data
{
    QString  name;
    QVariant default_value;
} PETCat_options_data_t;
//--------------------------------------------------------------------------------
class PETCat_options : public MyWidget
{
    Q_OBJECT
public:
    explicit PETCat_options(QWidget *parent = nullptr);

    static quint16 get_port_cmd(void);
    static quint16 get_port_event(void);

    static bool get_max_net_timeout(QVariant *value);
    static bool get_max_pet_timeout(QVariant *value);

    static QStringList get_eth_addresses(void);

    //---
    bool get_floodmap_option(QString name, QVariant *result);
    bool set_floodmap_option(QString name, QVariant value);

    bool get_oscilloscope_option(QString name, QVariant *result);
    bool set_oscilloscope_option(QString name, QVariant value);

    bool get_histogram_consulience_option(QString name, QVariant *result);
    bool set_histogram_consulience_option(QString name, QVariant value);

    bool get_histogram_energy_option(QString name, QVariant *result);
    bool set_histogram_energy_option(QString name, QVariant value);

    bool get_imitator_brg_option(QString name, QVariant *result);
    bool set_imitator_brg_option(QString name, QVariant value);
    //---

private:
    QList<PETCat_options_data> l_floodmap_options;
    QList<PETCat_options_data> l_oscilloscope_options;
    QList<PETCat_options_data> l_histogram_consulience_options;
    QList<PETCat_options_data> l_histogram_energy_options;
    QList<PETCat_options_data> l_imitator_brg_options;
};
//--------------------------------------------------------------------------------
#endif // PETCAT_OPTIONS_HPP
