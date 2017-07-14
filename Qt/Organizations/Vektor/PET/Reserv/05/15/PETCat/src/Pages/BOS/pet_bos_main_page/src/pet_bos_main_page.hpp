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
#ifndef PET_BOS_MAIN_PAGE_HPP
#define PET_BOS_MAIN_PAGE_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_bos.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "pet_device_info_page.hpp"
#include "pet_net_config_page.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_bos_main_page;
}
//--------------------------------------------------------------------------------
class Transceiver;
//--------------------------------------------------------------------------------
class PET_bos_main_page : public PET_device_bos
{
    Q_OBJECT

public:
    explicit PET_bos_main_page(QWidget *parent,
                               bool is_demo = false);
    ~PET_bos_main_page();

    void all_refresh(void);
    void update_widgets(void);

public slots:
    void main_refresh(void);
    void main_apply(void);
    void main_close(void);

private slots:
    void get_packet(QByteArray packet);

    void f_import(void);
    void f_export(void);

private:
    Ui::PET_bos_main_page *ui = 0;
    bool flag_is_demo;

    // параметры BOS
    // общие
    // PET_CMD_GET_DEVICE_INFO

    // сеть
    // PET_CMD_GET_NET_CONFIG
    // PET_CMD_SET_NET_CONFIG

    // control
    // PET_CMD_GET_CONTROL
    // PET_CMD_SET_CONTROL

    // state
    // PET_CMD_GET_STATE

    PET_RESULT f_PET_CMD_GET_DEVICE_INFO(void);
    PET_RESULT f_PET_CMD_GET_CONTROL(void);
    //PET_RESULT f_PET_CMD_GET_STATE(void);

    PET_RESULT f_PET_CMD_SET_CONTROL(void);

    //---
    PET_device_info_page *p_device_info = 0;
    PET_net_config_page  *p_net_config = 0;
    //---
    void add_page_p_device_info(void);
    void add_page_p_net_config(void);
    //---

    bool is_ok;
    QByteArray clean_packet_buf;

    void init(void);

    void block_this_button(bool state);
    void block_interface(QWidget *widget, bool state);
    bool check_packet(uint8_t cmd, QByteArray packet);
};
//--------------------------------------------------------------------------------
#endif
