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
#ifndef PET_BRG_SIM_MAIN_PAGE_HPP
#define PET_BRG_SIM_MAIN_PAGE_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_brg_sim.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "pet_device_info_page.hpp"
#include "pet_brg_ch_page.hpp"
#include "pet_brg_debug_page.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class PET_brg_sim_main_page;
}
//--------------------------------------------------------------------------------
class Transceiver;
//--------------------------------------------------------------------------------
class PET_brg_sim_main_page : public PET_device_brg_sim
{
    Q_OBJECT

public:
    explicit PET_brg_sim_main_page(QWidget *parent, bool is_demo = false);
    ~PET_brg_sim_main_page();

    void all_refresh(void);
    void update_widgets(void);

public slots:
    void main_refresh(void);
    void main_apply(void);
    void main_close(void);

    void f_import(void);
    void f_export(void);

private slots:
    void get_packet(QByteArray packet);

    void btn_refresh_click(void);
    void btn_apply_click(void);
    void btn_close_click(void);

    void btn_import_click(void);
    void btn_export_click(void);

private:
    Ui::PET_brg_sim_main_page *ui;
    bool flag_is_demo = false;

    // параметры BRG
    // общие
    // PET_CMD_GET_DEVICE_INFO

    // control
    // PET_CMD_GET_CONTROL
    // PET_CMD_SET_CONTROL

    // state
    // PET_CMD_GET_STATE

    // каналы
    // PET_CMD_GET_CH_CONTROL
    // PET_CMD_SET_CH_CONTROL

    // state
    // PET_CMD_GET_CH_STATE

    PET_RESULT f_PET_CMD_GET_DEVICE_INFO(void);

    PET_RESULT f_PET_CMD_GET_CONTROL(void);
    PET_RESULT f_PET_CMD_SET_CONTROL(void);

    //PET_RESULT f_PET_CMD_GET_STATE(void);

    PET_RESULT f_PET_CMD_GET_CH_CONTROL(void);
    PET_RESULT f_PET_CMD_SET_CH_CONTROL(void);

    PET_RESULT f_PET_CMD_GET_CH_STATE(void);

    //---
    PET_device_info_page *p_brg_device_info;

    PET_brg_debug_page  *p_brg_debug;
    //---
    PET_brg_ch_page     *p_brg_ch_0;
    PET_brg_ch_page     *p_brg_ch_1;
    PET_brg_ch_page     *p_brg_ch_2;
    PET_brg_ch_page     *p_brg_ch_3;
    //---
    void add_page_device_info(void);
    void add_page_debug(void);
    void add_page_ch0(void);
    void add_page_ch1(void);
    void add_page_ch2(void);
    void add_page_ch3(void);
    //---

    bool is_ok = false;
    QByteArray clean_packet_buf;

    uint16_t channel;

    void init(void);

    void block_this_button(bool state);
    bool check_packet(uint8_t cmd, QByteArray packet);
};
//--------------------------------------------------------------------------------
#endif
