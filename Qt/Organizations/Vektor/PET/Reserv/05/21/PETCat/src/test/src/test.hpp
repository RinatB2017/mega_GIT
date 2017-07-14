/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef TEST_HPP
#define TEST_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QTest>
//--------------------------------------------------------------------------------
#include "PET_device_bos.hpp"
#include "PET_device_brg.hpp"
#include "PET_device_sim.hpp"
#include "PET_device_td.hpp"
#include "PET_device_brg_sim.hpp"
//--------------------------------------------------------------------------------
#include "demo_device_bos.hpp"
#include "demo_device_brg.hpp"
#include "demo_device_sim.hpp"
#include "demo_device_td.hpp"
#include "demo_device_brg_sim.hpp"
#include "pet_demo.hpp"
//--------------------------------------------------------------------------------
class Test : public QObject {
    Q_OBJECT

public:
    Test();

private slots:
    void test_bos(void);
    void test_brg(void);
    void test_sim(void);
    void test_td(void);
    void test_brg_sim(void);

    void test_demo_pet(void);

private:
    QHostAddress get_eth_address(void);
    QStringList get_eth_addresses(void);

    void check_bos(void);
    void check_brg(void);
    void check_sim(void);
    void check_td(void);
    void check_brg_sim(void);

    void check_BOS_CMD_TEST(Device_BOS *bos);
    void check_BOS_CMD_RESET(Device_BOS *bos);
    void check_BOS_CMD_SHUTDOWN(Device_BOS *bos);
    void check_BOS_CMD_DISCOVERY(Device_BOS *bos);
    void check_BOS_CMD_CONFIG_SAVE(Device_BOS *bos);
    void check_BOS_CMD_CONFIG_DEFAULT(Device_BOS *bos);
    void check_BOS_CMD_GET_DEVICE_INFO(Device_BOS *bos);
    void check_BOS_CMD_WAVE(Device_BOS *bos);
    void check_BOS_CMD_RUN(Device_BOS *bos);
    void check_BOS_CMD_STOP(Device_BOS *bos);

    void check_BOS_CMD_CONTROL(Device_BOS *bos);
    void check_BOS_CMD_STATE(Device_BOS *bos);
    void check_BOS_CMD_CH_STATE(Device_BOS *bos);
    void check_BOS_CMD_CH_CONTROL(Device_BOS *bos);

    void check_BOS_CMD_UPDATE(Device_BOS *bos);

    void check_BRG_CMD_TEST(Device_BRG *brg);
    void check_BRG_CMD_RESET(Device_BRG *brg);
    void check_BRG_CMD_SHUTDOWN(Device_BRG *brg);
    void check_BRG_CMD_DISCOVERY(Device_BRG *brg);
    void check_BRG_CMD_CONFIG_SAVE(Device_BRG *brg);
    void check_BRG_CMD_CONFIG_DEFAULT(Device_BRG *brg);
    void check_BRG_CMD_GET_DEVICE_INFO(Device_BRG *brg);
    void check_BRG_CMD_WAVE(Device_BRG *brg);
    void check_BRG_CMD_RUN(Device_BRG *brg);
    void check_BRG_CMD_STOP(Device_BRG *brg);

    void check_BRG_CMD_CALIBRATION(Device_BRG *brg);

    void check_BRG_CMD_CONTROL(Device_BRG *brg);
    void check_BRG_CMD_STATE(Device_BRG *brg);
    void check_BRG_CMD_CH_STATE(Device_BRG *brg);
    void check_BRG_CMD_CH_CONTROL(Device_BRG *brg);

    void check_BRG_CMD_UPDATE(Device_BRG *brg);

    void check_SIM_CMD_TEST(Device_SIM *sim);
    void check_SIM_CMD_RESET(Device_SIM *sim);
    void check_SIM_CMD_SHUTDOWN(Device_SIM *sim);
    void check_SIM_CMD_DISCOVERY(Device_SIM *sim);
    void check_SIM_CMD_CONFIG_SAVE(Device_SIM *sim);
    void check_SIM_CMD_CONFIG_DEFAULT(Device_SIM *sim);
    void check_SIM_CMD_GET_DEVICE_INFO(Device_SIM *sim);
    void check_SIM_CMD_WAVE(Device_SIM *sim);
    void check_SIM_CMD_RUN(Device_SIM *sim);
    void check_SIM_CMD_STOP(Device_SIM *sim);

    void check_SIM_CMD_CONTROL(Device_SIM *sim);
    void check_SIM_CMD_STATE(Device_SIM *sim);
    void check_SIM_CMD_CH_STATE(Device_SIM *sim);
    void check_SIM_CMD_CH_CONTROL(Device_SIM *sim);

    void check_SIM_CMD_UPDATE(Device_SIM *sim);

    void check_TD_CMD_TEST(Device_TD *td);
    void check_TD_CMD_RESET(Device_TD *td);
    void check_TD_CMD_SHUTDOWN(Device_TD *td);
    void check_TD_CMD_DISCOVERY(Device_TD *td);
    void check_TD_CMD_CONFIG_SAVE(Device_TD *td);
    void check_TD_CMD_CONFIG_DEFAULT(Device_TD *td);
    void check_TD_CMD_GET_DEVICE_INFO(Device_TD *td);
    void check_TD_CMD_WAVE(Device_TD *td);
    void check_TD_CMD_RUN(Device_TD *td);
    void check_TD_CMD_STOP(Device_TD *td);

    void check_TD_CMD_CONTROL(Device_TD *td);
    void check_TD_CMD_STATE(Device_TD *td);
    void check_TD_CMD_CH_STATE(Device_TD *td);
    void check_TD_CMD_CH_CONTROL(Device_TD *td);

    void check_TD_CMD_UPDATE(Device_TD *td);

    void check_BRG_SIM_CMD_TEST(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_RESET(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_SHUTDOWN(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_DISCOVERY(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_CONFIG_SAVE(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_CONFIG_DEFAULT(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_GET_DEVICE_INFO(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_WAVE(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_RUN(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_STOP(Device_BRG_SIM *brg_sim);

    void check_BRG_SIM_CMD_CONTROL(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_STATE(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_CH_STATE(Device_BRG_SIM *brg_sim);
    void check_BRG_SIM_CMD_CH_CONTROL(Device_BRG_SIM *brg_sim);

    void check_BRG_SIM_CMD_UPDATE(Device_BRG_SIM *brg_sim);
};
//--------------------------------------------------------------------------------
#endif
