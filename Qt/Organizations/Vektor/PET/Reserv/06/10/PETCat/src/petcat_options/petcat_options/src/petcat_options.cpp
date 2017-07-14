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
#include <QNetworkInterface>
#include <QVariant>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "pet_defines.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_brg.h"
#include "pet.h"
//--------------------------------------------------------------------------------
PETCat_options::PETCat_options(QWidget *parent) :
    MyWidget(parent)
{
    l_floodmap_options.clear();
    l_floodmap_options.append( {PARAM_floodmap_sb_min_energy,           DEFAULT_PARAM_floodmap_sb_min_energy} );
    l_floodmap_options.append( {PARAM_floodmap_sb_max_energy,           DEFAULT_PARAM_floodmap_sb_max_energy} );
    l_floodmap_options.append( {PARAM_floodmap_events_flags,            DEFAULT_PARAM_floodmap_events_flags} );
    l_floodmap_options.append( {PARAM_floodmap_flag_min_energy,         DEFAULT_PARAM_floodmap_flag_min_energy});
    l_floodmap_options.append( {PARAM_floodmap_flag_max_energy,         DEFAULT_PARAM_floodmap_flag_max_energy});
    l_floodmap_options.append( {PARAM_floodmap_source_expanded,         DEFAULT_PARAM_floodmap_source_expanded});
    l_floodmap_options.append( {PARAM_floodmap_energy_expanded,         DEFAULT_PARAM_floodmap_energy_expanded});
    l_floodmap_options.append( {PARAM_floodmap_events_flag_expanded,    DEFAULT_PARAM_floodmap_event_flag_expanded});
    l_floodmap_options.append( {PARAM_floodmap_picture_expanded,        DEFAULT_PARAM_floodmap_picture_expanded});

    l_oscilloscope_options.clear();
    l_oscilloscope_options.append( {PARAM_oscilloscope_cb_sync,             DEFAULT_PARAM_oscilloscope_cb_sync} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_cb_auto,             DEFAULT_PARAM_oscilloscope_cb_auto} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_cb_type_curve,       DEFAULT_PARAM_oscilloscope_cb_type_curve} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_cb_channel,          DEFAULT_PARAM_oscilloscope_cb_channel} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_sb_time,             SB_TIME_DEFAULT} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_sb_vert_scale_min,   DEFAULT_PARAM_oscilloscope_sb_vert_scale_min} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_sb_vert_scale_max,   DEFAULT_PARAM_oscilloscope_sb_vert_scale_max} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_clr_x_plus,          DEFAULT_PARAM_oscilloscope_clr_x_plus} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_clr_x_minus,         DEFAULT_PARAM_oscilloscope_clr_x_minus} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_clr_y_plus,          DEFAULT_PARAM_oscilloscope_clr_y_plus} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_clr_y_minus,         DEFAULT_PARAM_oscilloscope_clr_y_minus} );
    l_oscilloscope_options.append( {PARAM_oscilloscope_clr_sum,             DEFAULT_PARAM_oscilloscope_clr_sum} );

    l_histogram_consulience_options.clear();
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_sb_nudChANumber,       DEFAULT_PARAM_histogram_consulience_sb_nudChANumber} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_sb_nudChBNumber,       DEFAULT_PARAM_histogram_consulience_sb_nudChANumber} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_dsb_nudChACor,         DEFAULT_PARAM_histogram_consulience_dsb_nudChACor} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_dsb_nudChBCor,         DEFAULT_PARAM_histogram_consulience_dsb_nudChBCor} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_dsb_nudTInt,           DEFAULT_PARAM_histogram_consulience_dsb_nudTInt} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_dsb_nudWindow,         DEFAULT_PARAM_histogram_consulience_dsb_nudWindow} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_dsb_nudN,              DEFAULT_PARAM_histogram_consulience_dsb_nudN} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_clr_p,                 DEFAULT_PARAM_histogram_consulience_clr_p} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_clr_gauss,             DEFAULT_PARAM_histogram_consulience_clr_gauss} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_events_flags,          DEFAULT_PARAM_histogram_consulience_events_flags} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_channel_a_expanded,    DEFAULT_PARAM_histogram_consulience_channel_a_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_channel_b_expanded,    DEFAULT_PARAM_histogram_consulience_channel_b_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_ts_expanded,           DEFAULT_PARAM_histogram_consulience_ts_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_consillence_expanded,  DEFAULT_PARAM_histogram_consulience_consillence_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_analiz_expanded,       DEFAULT_PARAM_histogram_consulience_analiz_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_events_flag_expanded,  DEFAULT_PARAM_histogram_consulience_events_flag_expanded} );
    l_histogram_consulience_options.append( {PARAM_histogram_consulience_param_expanded,        DEFAULT_PARAM_histogram_consulience_param_expanded} );

    l_histogram_energy_options.clear();
    l_histogram_energy_options.append( {PARAM_histogram_energy_sb_e_min,            DEFAULT_PARAM_histogram_energy_sb_e_min} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_sb_e_max,            DEFAULT_PARAM_histogram_energy_sb_e_max} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_sb_N,                DEFAULT_PARAM_histogram_energy_sb_N} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_color_histogramm,    DEFAULT_PARAM_histogram_energy_color_histogramm} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_flag_e_min,          DEFAULT_PARAM_histogram_energy_flag_e_min} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_flag_e_max,          DEFAULT_PARAM_histogram_energy_flag_e_max} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_source_expanded,     DEFAULT_PARAM_histogram_energy_source_expanded} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_energy_expanded,     DEFAULT_PARAM_histogram_energy_energy_expanded} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_events_flag_expanded, DEFAULT_PARAM_histogram_energy_events_flag_expanded} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_param_expanded,      DEFAULT_PARAM_histogram_energy_param_expanded} );
    l_histogram_energy_options.append( {PARAM_histogram_energy_events_flags,        DEFAULT_PARAM_histogram_energy_events_flags} );

    l_imitator_brg_options.clear();
    l_imitator_brg_options.append( {PARAM_imitator_brg_pid, IMITATOR_BRG_DEFAULT_PID} );
    l_imitator_brg_options.append( {PARAM_imitator_brg_vid, IMITATOR_BRG_DEFAULT_VID} );
}
//--------------------------------------------------------------------------------
quint16 PETCat_options::get_port_cmd(void)
{
    QVariant p_cmd;

    bool ok = get_param(PETCAT_OPTIONS_MAIN_GROUP,
                        PARAM_PORT_CMD_NAME,
                        PET_NET_CMD_PORT_DEFAULT,
                        &p_cmd);
    if(!ok)
    {
        return PET_NET_CMD_PORT_DEFAULT;
    }
    int cmd = p_cmd.toInt(&ok);
    if(!ok)
    {
        return PET_NET_CMD_PORT_DEFAULT;
    }
    return cmd;
}
//--------------------------------------------------------------------------------
quint16 PETCat_options::get_port_event(void)
{
    QVariant p_event;

    bool ok = get_param(PETCAT_OPTIONS_MAIN_GROUP,
                        PARAM_PORT_EVENT_NAME,
                        PET_NET_EVENT_PORT_DEFAULT,
                        &p_event);
    if(!ok)
    {
        return PET_NET_EVENT_PORT_DEFAULT;
    }
    int event = p_event.toInt(&ok);
    if(!ok)
    {
        return PET_NET_EVENT_PORT_DEFAULT;
    }
    return event;
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_max_net_timeout(QVariant *value)
{
    //FIXME magic value
    return get_param(PETCAT_OPTIONS_MAIN_GROUP, PARAM_MAX_NET_TIMEOUT, DEFAULT_PARAM_MAX_NET_TIMEOUT, value);
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_max_pet_timeout(QVariant *value)
{
    //FIXME magic value
    return get_param(PETCAT_OPTIONS_MAIN_GROUP, PARAM_MAX_PET_TIMEOUT, DEFAULT_PARAM_MAX_PET_TIMEOUT, value);
}
//--------------------------------------------------------------------------------
QStringList PETCat_options::get_eth_addresses(void)
{
    QStringList lists;

    lists.clear();
    QList<QNetworkInterface> myInterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface iface, myInterfaces)
    {
        if(!iface.name().contains("lo"))
        {
            QList<QNetworkAddressEntry> entry_list = iface.addressEntries();
            foreach (QNetworkAddressEntry entry, entry_list)
            {
                QAbstractSocket::NetworkLayerProtocol proto = QHostAddress(entry.ip()).protocol();
                if(proto == QAbstractSocket::IPv4Protocol)
                {
                    lists.append(entry.ip().toString());
                }
            }
        }
    }
    if(lists.isEmpty())
    {
        QHostAddress local = QHostAddress::LocalHost;
        lists.append(local.toString());
    }
    return lists;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool PETCat_options::get_floodmap_option(QString name, QVariant *result)
{
    for(int n=0; n<l_floodmap_options.count(); n++)
    {
        if(l_floodmap_options[n].name == name)
        {
            return get_param(PETCAT_OPTIONS_FLOODMAP_GROUP, name, l_floodmap_options[n].default_value, result);
        }
    }

    qDebug() << QString("get_floodmap_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::set_floodmap_option(QString name, QVariant value)
{
    for(int n=0; n<l_floodmap_options.count(); n++)
    {
        if(l_floodmap_options[n].name == name)
        {
            return set_param(PETCAT_OPTIONS_FLOODMAP_GROUP, name, value);
        }
    }

    qDebug() << QString("set_floodmap_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_oscilloscope_option(QString name, QVariant *result)
{
    for(int n=0; n<l_oscilloscope_options.count(); n++)
    {
        if(l_oscilloscope_options[n].name == name)
        {
            return get_param(PETCAT_OPTIONS_OSCILLOSCOPE_GROUP, name, l_oscilloscope_options[n].default_value, result);
        }
    }

    qDebug() << QString("get_oscilloscope_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::set_oscilloscope_option(QString name, QVariant value)
{
    for(int n=0; n<l_oscilloscope_options.count(); n++)
    {
        if(l_oscilloscope_options[n].name == name)
        {
            return set_param(PETCAT_OPTIONS_OSCILLOSCOPE_GROUP, name, value);
        }
    }

    qDebug() << QString("set_oscilloscope_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_histogram_consulience_option(QString name, QVariant *result)
{
    for(int n=0; n<l_histogram_consulience_options.count(); n++)
    {
        if(l_histogram_consulience_options[n].name == name)
        {
            return get_param(PETCAT_OPTIONS_HISTOGRAM_CONSULIENCE_GROUP, name, l_histogram_consulience_options[n].default_value, result);
        }
    }

    qDebug() << QString("get_histogram_consulience_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::set_histogram_consulience_option(QString name, QVariant value)
{
    for(int n=0; n<l_histogram_consulience_options.count(); n++)
    {
        if(l_histogram_consulience_options[n].name == name)
        {
            return set_param(PETCAT_OPTIONS_HISTOGRAM_CONSULIENCE_GROUP, name, value);
        }
    }

    qDebug() << QString("set_histogram_consulience_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_histogram_energy_option(QString name, QVariant *result)
{
    for(int n=0; n<l_histogram_energy_options.count(); n++)
    {
        if(l_histogram_energy_options[n].name == name)
        {
            return get_param(PETCAT_OPTIONS_HISTOGRAM_ENERGY_GROUP, name, l_histogram_energy_options[n].default_value, result);
        }
    }

    qDebug() << QString("get_histogram_energy_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::set_histogram_energy_option(QString name, QVariant value)
{
    for(int n=0; n<l_histogram_energy_options.count(); n++)
    {
        if(l_histogram_energy_options[n].name == name)
        {
            return set_param(PETCAT_OPTIONS_HISTOGRAM_ENERGY_GROUP, name, value);
        }
    }

    qDebug() << QString("set_histogram_energy_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::get_imitator_brg_option(QString name, QVariant *result)
{
    for(int n=0; n<l_imitator_brg_options.count(); n++)
    {
        if(l_imitator_brg_options[n].name == name)
        {
            return get_param(PETCAT_OPTIONS_IMITATOR_BRG_GROUP, name, l_imitator_brg_options[n].default_value, result);
        }
    }

    qDebug() << QString("get_imitator_brg_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
bool PETCat_options::set_imitator_brg_option(QString name, QVariant value)
{
    for(int n=0; n<l_imitator_brg_options.count(); n++)
    {
        if(l_imitator_brg_options[n].name == name)
        {
            return set_param(PETCAT_OPTIONS_IMITATOR_BRG_GROUP, name, value);
        }
    }

    qDebug() << QString("set_imitator_brg_option: unknown name [%1]").arg(name);
    return false;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
