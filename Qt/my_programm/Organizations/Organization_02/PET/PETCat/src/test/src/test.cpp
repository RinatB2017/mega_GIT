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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include <QTest>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_td.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "test.hpp"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
QHostAddress Test::get_eth_address(void)
{
    QStringList sl = get_eth_addresses();
    foreach (QString addr, sl)
    {
        return QHostAddress(addr);
    }
    return QHostAddress(DEFAULT_BROADCAT_ADDR);
}
//--------------------------------------------------------------------------------
QStringList Test::get_eth_addresses(void)
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
void Test::test_bos(void)
{
    Device_BOS *bos = new Device_BOS(0);
    bos->set_pet_address(0);

    check_BOS_CMD_TEST(bos);
    check_BOS_CMD_RESET(bos);
    check_BOS_CMD_SHUTDOWN(bos);
    check_BOS_CMD_DISCOVERY(bos);
    check_BOS_CMD_CONFIG_SAVE(bos);
    check_BOS_CMD_CONFIG_DEFAULT(bos);
    check_BOS_CMD_GET_DEVICE_INFO(bos);
    check_BOS_CMD_WAVE(bos);
    check_BOS_CMD_RUN(bos);
    check_BOS_CMD_STOP(bos);

    check_BOS_CMD_CONTROL(bos);
    check_BOS_CMD_STATE(bos);
    check_BOS_CMD_CH_STATE(bos);
    check_BOS_CMD_CH_CONTROL(bos);

    check_BOS_CMD_UPDATE(bos);

    bos->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_brg(void)
{
    Device_BRG *brg = new Device_BRG(0);
    brg->set_pet_address(1);

    check_BRG_CMD_TEST(brg);
    check_BRG_CMD_RESET(brg);
    check_BRG_CMD_SHUTDOWN(brg);
    check_BRG_CMD_DISCOVERY(brg);
    check_BRG_CMD_CONFIG_SAVE(brg);
    check_BRG_CMD_CONFIG_DEFAULT(brg);
    check_BRG_CMD_GET_DEVICE_INFO(brg);
    check_BRG_CMD_WAVE(brg);
    check_BRG_CMD_RUN(brg);
    check_BRG_CMD_STOP(brg);

    check_BRG_CMD_CALIBRATION(brg);

    check_BRG_CMD_CONTROL(brg);
    check_BRG_CMD_STATE(brg);
    check_BRG_CMD_CH_STATE(brg);
    check_BRG_CMD_CH_CONTROL(brg);
    check_BRG_CMD_UPDATE(brg);

    brg->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_sim(void)
{
    Device_SIM *sim = new Device_SIM(0);
    sim->set_pet_address(0);

    check_SIM_CMD_TEST(sim);
    check_SIM_CMD_RESET(sim);
    check_SIM_CMD_SHUTDOWN(sim);
    check_SIM_CMD_DISCOVERY(sim);
    check_SIM_CMD_CONFIG_SAVE(sim);
    check_SIM_CMD_CONFIG_DEFAULT(sim);
    check_SIM_CMD_GET_DEVICE_INFO(sim);
    check_SIM_CMD_WAVE(sim);
    check_SIM_CMD_RUN(sim);
    check_SIM_CMD_STOP(sim);
    check_SIM_CMD_CONTROL(sim);
    check_SIM_CMD_STATE(sim);
    check_SIM_CMD_CH_STATE(sim);
    check_SIM_CMD_CH_CONTROL(sim);
    check_SIM_CMD_UPDATE(sim);

    sim->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_td(void)
{
    Device_TD *td = new Device_TD(0);
    td->set_pet_address(0);

    check_TD_CMD_TEST(td);
    check_TD_CMD_RESET(td);
    check_TD_CMD_SHUTDOWN(td);
    check_TD_CMD_DISCOVERY(td);
    check_TD_CMD_CONFIG_SAVE(td);
    check_TD_CMD_CONFIG_DEFAULT(td);
    check_TD_CMD_GET_DEVICE_INFO(td);
    check_TD_CMD_WAVE(td);
    check_TD_CMD_RUN(td);
    check_TD_CMD_STOP(td);
    check_TD_CMD_CONTROL(td);
    check_TD_CMD_STATE(td);
    check_TD_CMD_CH_STATE(td);
    check_TD_CMD_CH_CONTROL(td);
    check_TD_CMD_UPDATE(td);

    td->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_brg_sim(void)
{
    Device_BRG_SIM *brg_sim = new Device_BRG_SIM(0);
    brg_sim->set_pet_address(0);

    check_BRG_SIM_CMD_TEST(brg_sim);
    check_BRG_SIM_CMD_RESET(brg_sim);
    check_BRG_SIM_CMD_SHUTDOWN(brg_sim);
    check_BRG_SIM_CMD_DISCOVERY(brg_sim);
    check_BRG_SIM_CMD_CONFIG_SAVE(brg_sim);
    check_BRG_SIM_CMD_CONFIG_DEFAULT(brg_sim);
    check_BRG_SIM_CMD_GET_DEVICE_INFO(brg_sim);
    check_BRG_SIM_CMD_WAVE(brg_sim);
    check_BRG_SIM_CMD_RUN(brg_sim);
    check_BRG_SIM_CMD_STOP(brg_sim);
    check_BRG_SIM_CMD_CONTROL(brg_sim);
    check_BRG_SIM_CMD_STATE(brg_sim);
    check_BRG_SIM_CMD_CH_STATE(brg_sim);
    check_BRG_SIM_CMD_CH_CONTROL(brg_sim);
    check_BRG_SIM_CMD_UPDATE(brg_sim);

    brg_sim->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::test_demo_pet(void)
{
    PET_demo *demo = new PET_demo(NAME_DEMO, 0);
    if(demo == nullptr)
    {
        qCritical("demo not created");
        return;
    }

    demo->set_flag_test(true);
    demo->add_device_bos(PET_VID_RDCVECTOR, PET_PID_BOS, 0);
    demo->add_device_brg(PET_VID_RDCVECTOR, PET_PID_BRG, 1);
    demo->add_device_sim(PET_VID_RDCVECTOR, PET_PID_SIM, 0);
    demo->add_device_td(PET_VID_RDCVECTOR, PET_PID_TD, 0);
    demo->add_device_brg_sim(PET_VID_RDCVECTOR, PET_PID_BRG_SIM, 1);
    bool ok = demo->f_connect(50000);

    if(ok)
    {
        check_bos();
        check_brg();
        check_sim();
        check_td();
        check_brg_sim();
    }

    demo->f_disconnect();
    demo->deleteLater();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_TEST(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_TEST(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_RESET(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RESET;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_RESET(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_SHUTDOWN(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_SHUTDOWN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_SHUTDOWN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_DISCOVERY(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_discovery_info data;

    frame.addr = 0;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_DISCOVERY(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(data)));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_CONFIG_SAVE(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_SAVE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_CONFIG_SAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_CONFIG_DEFAULT(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_DEFAULT;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_CONFIG_DEFAULT(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_GET_DEVICE_INFO(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_DEVICE_INFO;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_GET_DEVICE_INFO(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_device_info)));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_WAVE(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_wave_t wave;
    wave.channel = 0;
    wave.mode = 0;
    wave.count = 1;

    pet_wave_point_t point;

    frame.addr = 0;
    frame.cmd = PET_CMD_WAVE;
    frame.len = sizeof(wave) + sizeof(point);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&wave,     sizeof(wave));
    input.append((char *)&point,    sizeof(point));

    QCOMPARE(bos->f_PET_CMD_WAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_wave_t) + sizeof(pet_wave_point_t)));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_RUN(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RUN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_RUN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_STOP(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_STOP;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_STOP(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_CONTROL(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_bos_ctrl ctrl;

    //---
    for(unsigned int n=0; n<sizeof(ctrl); n++)
    {
        ((char *)&ctrl)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = sizeof(ctrl);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,  sizeof(frame));
    ba_test.append((char *)&ctrl,   sizeof(ctrl));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = sizeof(ctrl);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&ctrl,     sizeof(ctrl));

    QCOMPARE(bos->f_PET_CMD_SET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_GET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_STATE(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_bos_state state;
    pet_bos_state test_state;

    //---
    test_state.state        = 0;    /* Состояние БОС (одно из значений PET_BOS_STATE) */
    test_state.flags        = 1;    /* Флаги состояния */
    test_state.sync_cycle   = 2;    /* Номер текущего цикла синхронизации */
    test_state.utilisation  = 3;    /* Общаяя загрузка по всем каналам регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_STATE;
    test_frame.len = sizeof(state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame, sizeof(test_frame));
    ba_test.append((char *)&test_state, sizeof(test_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_STATE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(bos->f_PET_CMD_GET_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_CH_STATE(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_bos_ch_state ch_state;
    pet_bos_ch_state test_ch_state;

    uint16_t channel;
    channel = 0;

    //---
    test_ch_state.channel        = 0;    /* Номер канала */
    test_ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BOS_CH_STATE)*/
    test_ch_state.flags          = 2;    /* Флаги состояния */
    test_ch_state.sync_cycle     = 3;    /* Номер цикла синхронизации */
    test_ch_state.utilisation    = 4;    /* Загрузка канала регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_CH_STATE;
    test_frame.len = sizeof(test_ch_state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,     sizeof(test_frame));
    ba_test.append((char *)&test_ch_state,  sizeof(test_ch_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_STATE;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(bos->f_PET_CMD_GET_CH_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_CH_CONTROL(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_bos_ch_ctrl ch_control;

    uint16_t channel;
    channel = 0;

    //---
    for(unsigned int n=0; n<sizeof(ch_control); n++)
    {
        ((char *)&ch_control)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,      sizeof(frame));
    ba_test.append((char *)&ch_control, sizeof(ch_control));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    input.clear();
    input.append((char *)&frame,        sizeof(frame));
    input.append((char *)&ch_control,   sizeof(ch_control));

    QCOMPARE(bos->f_PET_CMD_SET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(bos->f_PET_CMD_GET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BOS_CMD_UPDATE(Device_BOS *bos)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_update_data data;

    frame.addr = 0;
    frame.cmd = PET_CMD_UPDATE;
    frame.len = sizeof(data);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&data,     sizeof(data));

    QCOMPARE(bos->f_PET_CMD_UPDATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_TEST(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_TEST(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_RESET(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_RESET;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_RESET(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_SHUTDOWN(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_SHUTDOWN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_SHUTDOWN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_DISCOVERY(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_discovery_info data;

    frame.addr = 1;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_DISCOVERY(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(data)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CONFIG_SAVE(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_CONFIG_SAVE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_CONFIG_SAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CONFIG_DEFAULT(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_CONFIG_DEFAULT;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_CONFIG_DEFAULT(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_GET_DEVICE_INFO(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_GET_DEVICE_INFO;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_GET_DEVICE_INFO(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_device_info)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_WAVE(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_wave_t wave;
    wave.channel = 0;
    wave.mode = 0;
    wave.count = 1;

    pet_wave_point_t point;

    frame.addr = 1;
    frame.cmd = PET_CMD_WAVE;
    frame.len = sizeof(wave) + sizeof(point);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&wave,     sizeof(wave));
    input.append((char *)&point,    sizeof(point));

    QCOMPARE(brg->f_PET_CMD_WAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_wave_t) + sizeof(pet_wave_point_t)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_RUN(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_RUN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_RUN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_STOP(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 1;
    frame.cmd = PET_CMD_STOP;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_STOP(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CALIBRATION(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_brg_calibration_t calibration_data;
    pet_brg_calibration_t test_calibration_data;

    uint16_t channel;
    channel = 0;

    //---
    for(int n=0; n<PET_BRG_CH_NUM; n++)
    {
        calibration_data.ch_calibration[n].channel                   = n;    /* Номер канала */
        calibration_data.ch_calibration[n].flags                     = 0;    /* Флаги */
        calibration_data.ch_calibration[n].timestamp                 = 1;    /* Календарное время выполнения последней калибровки*/
        calibration_data.ch_calibration[n].xp_offset                 = 2;    /* Смещение сигнала X+ (мВ) */
        calibration_data.ch_calibration[n].xp_gain                   = 3;    /* Наклон сигнала X+ (1/мВ) */
        calibration_data.ch_calibration[n].xn_offset                 = 4;    /* Смещение сигнала X- (мВ) */
        calibration_data.ch_calibration[n].xn_gain                   = 5;    /* Наклон сигнала X- (1/мВ) */
        calibration_data.ch_calibration[n].yp_offset                 = 6;    /* Смещение сигнала Y+ (мВ) */
        calibration_data.ch_calibration[n].yp_gain                   = 7;    /* Наклон сигнала Y+ (1/мВ) */
        calibration_data.ch_calibration[n].yn_offset                 = 8;    /* Смещение сигнала Y- (мВ) */
        calibration_data.ch_calibration[n].yn_gain                   = 9;    /* Наклон сигнала Y- (1/мВ) */
        calibration_data.ch_calibration[n].ts_offset                 = 10;   /* Поправка метки времени (пс) */
        calibration_data.ch_calibration[n].in_offset_set_offset      = 11;   /* Смещение сигнала ФЭУ, смещение установки (мВ) */
        calibration_data.ch_calibration[n].in_offset_set_gain        = 12;   /* Смещение сигнала ФЭУ, наклон установки (1/мВ) */
        calibration_data.ch_calibration[n].in_offset_get_offset      = 13;   /* Смещение сигнала ФЭУ, смещение измерения (мВ) */
        calibration_data.ch_calibration[n].in_offset_get_gain        = 14;   /* Смещение сигнала ФЭУ, наклон измерения (1/мВ) */
        calibration_data.ch_calibration[n].in_threshold_set_offset   = 15;   /* Порог дискриминации, смещение установки (мВ) */
        calibration_data.ch_calibration[n].in_threshold_set_gain     = 16;   /* Порог дискриминации, наклон установки (1/мВ) */
        calibration_data.ch_calibration[n].in_threshold_get_offset   = 17;   /* Порог дискриминации, смещение измерения (мВ) */
        calibration_data.ch_calibration[n].in_thresholdga_get_gain   = 18;   /* Порог дискриминации, наклон измерения (1/мВ) */
        calibration_data.ch_calibration[n].adc_ref_set_offset        = 19;   /* Опорное напряжение АЦП, смещение установки (мВ) */
        calibration_data.ch_calibration[n].adc_ref_set_gain          = 20;   /* Опорное напряжение АЦП, наклон установки (1/мВ) */
        calibration_data.ch_calibration[n].adc_ref_get_offset        = 21;   /* Опорное напряжение АЦП, смещение измерения (мВ) */
        calibration_data.ch_calibration[n].adc_ref_get_gain          = 22;   /* Опорное напряжение АЦП, наклон измерения (1/мВ) */
        calibration_data.ch_calibration[n].pm_bias_set_offset        = 23;   /* Напряжение смещения ФЭУ, смещение установки (мВ) */
        calibration_data.ch_calibration[n].pm_bias_set_gain          = 24;   /* Напряжение смещения ФЭУ, наклон установки (1/мВ) */
        calibration_data.ch_calibration[n].pm_bias_get_offset        = 25;   /* Напряжение смещения ФЭУ, смещение измерения (мВ) */
        calibration_data.ch_calibration[n].pm_bias_get_gain          = 26;   /* Напряжение смещения ФЭУ, наклон измерения (1/мВ) */
        calibration_data.ch_calibration[n].readout_temp_get_offset   = 27;   /* Температура платы считывания, смещение измерения (х0,01 °C) */
        calibration_data.ch_calibration[n].readout_temp_get_gain     = 28;   /* Температура платы считывания, наклон измерения (х0,01 1/°C) */
        calibration_data.ch_calibration[n].ifmodule_temp_get_offset  = 29;   /* Температура модуля сопряжения, смещение измерения (х0,01 °C) */
        calibration_data.ch_calibration[n].ifmodule_temp_get_gain    = 30;   /* Температура модуля сопряжения, наклон измерения (х0,01 1/°C) */
    }
    //---
    memcpy((char *)&test_calibration_data, (char *)&calibration_data, sizeof(calibration_data));
    //---

    test_frame.addr = 1;
    test_frame.cmd = PET_CMD_GET_CALIBRATION;
    test_frame.len = sizeof(test_calibration_data);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,             sizeof(test_frame));
    ba_test.append((char *)&test_calibration_data,  sizeof(test_calibration_data));
    //---

    //set
    frame.addr = 1;
    frame.cmd = PET_CMD_SET_CALIBRATION;
    frame.len = sizeof(calibration_data);

    input.clear();
    input.append((char *)&frame,            sizeof(frame));
    input.append((char *)&calibration_data, sizeof(calibration_data));

    QCOMPARE(brg->f_PET_CMD_SET_CALIBRATION(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(calibration_data)));

    // get
    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CALIBRATION;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(brg->f_PET_CMD_GET_CALIBRATION(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(calibration_data)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CONTROL(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_brg_ctrl ctrl;

    //---
    for(unsigned int n=0; n<sizeof(ctrl); n++)
    {
        ((char *)&ctrl)[n] = n;
    }

    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = sizeof(ctrl);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,  sizeof(frame));
    ba_test.append((char *)&ctrl,   sizeof(ctrl));
    //---

    // set
    frame.addr = 1;
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = sizeof(ctrl);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&ctrl,     sizeof(ctrl));

    QCOMPARE(brg->f_PET_CMD_SET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    // get
    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_GET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_STATE(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_brg_state state;
    pet_brg_state test_state;

    //---
    test_state.state             = 0;    /* Состояние БРГ (одно из значений PET_BRG_STATE) */
    test_state.flags             = 1;    /* Флаги состояния */
    test_state.sync_cycle        = 2;    /* Номер текущего цикла синхронизации */
    test_state.utilisation       = 3;    /* Общая загрузка по всем каналам регистрации */

    test_frame.addr = 1;
    test_frame.cmd = PET_CMD_GET_STATE;
    test_frame.len = sizeof(state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame, sizeof(test_frame));
    ba_test.append((char *)&test_state, sizeof(test_state));
    //---

    frame.addr = 1;
    frame.cmd = PET_CMD_GET_STATE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg->f_PET_CMD_GET_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CH_STATE(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_brg_ch_state_t ch_state;
    pet_brg_ch_state_t test_ch_state;

    uint16_t channel;
    channel = 0;

    //---
    test_ch_state.channel        = 0;    /* Номер канала */
    test_ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BRG_CH_STATE)*/
    test_ch_state.flags          = 2;    /* Флаги состояния канала обработки */
    test_ch_state.in_offset      = 3;    /* Смещение сигнала ФЭУ (мВ) */
    test_ch_state.in_threshold   = 4;    /* Порог дискриминации (мВ) */
    test_ch_state.adc_reference  = 5;    /* Опорное напряжение АЦП (мВ) */
    test_ch_state.pm_bias        = 6;    /* Напряжение смещения ФЭУ (мВ) */
    test_ch_state.readout_temp   = 7;    /* Температура платы считывания (х0,01 °C) */
    test_ch_state.ifmodule_temp  = 8;    /* Температура модуля сопряжения (х0,01 °C) */
    test_ch_state.sync_cycle     = 9;    /* Номер цикла синхронизации */
    test_ch_state.utilisation    = 10;   /* Загрузка канала регистрации */
    test_ch_state.adc_underflow  = 11;   /* Число событий с признаком опустошения (потери значимости) данных АЦП */
    test_ch_state.adc_overflow   = 12;   /* Число событий с признаком переполнения (потери значимости) данных АЦП */
    test_ch_state.qdc_underflow  = 13;   /* Число событий с признаком опустошения (потери значимости) координатных сигналов */
    test_ch_state.qdc_overflow   = 14;   /* Число событий с признаком переполнения (потери значимости) координатных сигналов */
    test_ch_state.gate_overlay   = 15;   /* Число событий с признаком наложения событий */
    test_ch_state.ts_invalid     = 16;   /* Число событий с признаком не корректности метки времени */

    test_frame.addr = 1;
    test_frame.cmd = PET_CMD_GET_CH_STATE;
    test_frame.len = sizeof(test_ch_state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,     sizeof(test_frame));
    ba_test.append((char *)&test_ch_state,  sizeof(test_ch_state));
    //---

    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CH_STATE;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(brg->f_PET_CMD_GET_CH_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_CH_CONTROL(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_brg_ch_ctrl ch_control;

    uint16_t channel;
    channel = 0;

    //---
    for(unsigned int n=0; n<sizeof(ch_control); n++)
    {
        ((char *)&ch_control)[n] = n;
    }

    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,      sizeof(frame));
    ba_test.append((char *)&ch_control, sizeof(ch_control));
    //---

    // set
    frame.addr = 1;
    frame.cmd = PET_CMD_SET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    input.clear();
    input.append((char *)&frame,        sizeof(frame));
    input.append((char *)&ch_control,   sizeof(ch_control));

    QCOMPARE(brg->f_PET_CMD_SET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    // get
    frame.addr = 1;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(brg->f_PET_CMD_GET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_CMD_UPDATE(Device_BRG *brg)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_update_data data;
    data.length = 0;

    frame.addr = 1;
    frame.cmd = PET_CMD_UPDATE;
    frame.len = sizeof(data);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&data,     sizeof(data));

    QCOMPARE(brg->f_PET_CMD_UPDATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_TEST(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_TEST(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_RESET(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RESET;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_RESET(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_SHUTDOWN(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_SHUTDOWN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_SHUTDOWN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_DISCOVERY(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_discovery_info data;

    frame.addr = 0;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_DISCOVERY(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(data)));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_CONFIG_SAVE(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_SAVE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_CONFIG_SAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_CONFIG_DEFAULT(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_DEFAULT;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_CONFIG_DEFAULT(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_GET_DEVICE_INFO(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_DEVICE_INFO;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_GET_DEVICE_INFO(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_device_info)));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_WAVE(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_wave_t wave;
    wave.channel = 0;
    wave.mode = 0;
    wave.count = 1;

    pet_wave_point_t point;

    frame.addr = 0;
    frame.cmd = PET_CMD_WAVE;
    frame.len = sizeof(wave) + sizeof(point);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&wave,     sizeof(wave));
    input.append((char *)&point,    sizeof(point));

    QCOMPARE(sim->f_PET_CMD_WAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_wave_t) + sizeof(pet_wave_point_t)));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_RUN(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RUN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_RUN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_STOP(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_STOP;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_STOP(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_CONTROL(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_bos_ctrl ctrl;

    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = sizeof(ctrl);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&ctrl,     sizeof(ctrl));

    QCOMPARE(sim->f_PET_CMD_SET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_GET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_STATE(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_sim_state_t state;
    pet_sim_state_t test_state;

    //---
    test_state.state         = 0;    /* Состояние БОС (одно из значений PET_BOS_STATE) */
    test_state.flags         = 1;    /* Флаги состояния */
    test_state.sync_cycle    = 2;    /* Номер текущего цикла синхронизации */
    test_state.utilisation   = 3;    /* Общаяя загрузка по всем каналам регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_STATE;
    test_frame.len = sizeof(state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame, sizeof(test_frame));
    ba_test.append((char *)&test_state, sizeof(test_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_STATE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(sim->f_PET_CMD_GET_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_CH_STATE(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_sim_ch_state_t ch_state;
    pet_sim_ch_state_t test_ch_state;

    uint16_t channel;
    channel = 0;

    //---
    test_ch_state.channel        = 0;    /* Номер канала */
    test_ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BOS_CH_STATE)*/
    test_ch_state.flags          = 2;    /* Флаги состояния */
    test_ch_state.sync_cycle     = 3;    /* Номер цикла синхронизации */
    test_ch_state.utilisation    = 4;    /* Загрузка канала регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_CH_STATE;
    test_frame.len = sizeof(test_ch_state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,     sizeof(test_frame));
    ba_test.append((char *)&test_ch_state,  sizeof(test_ch_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_STATE;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(sim->f_PET_CMD_GET_CH_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_CH_CONTROL(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_sim_ch_ctrl_t ch_control;

    uint16_t channel;
    channel = 0;

    //---
    for(unsigned int n=0; n<sizeof(ch_control); n++)
    {
        ((char *)&ch_control)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,      sizeof(frame));
    ba_test.append((char *)&ch_control, sizeof(ch_control));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    input.clear();
    input.append((char *)&frame,        sizeof(frame));
    input.append((char *)&ch_control,   sizeof(ch_control));

    QCOMPARE(sim->f_PET_CMD_SET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(sim->f_PET_CMD_GET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_SIM_CMD_UPDATE(Device_SIM *sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_update_data data;

    frame.addr = 0;
    frame.cmd = PET_CMD_UPDATE;
    frame.len = sizeof(data);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&data,     sizeof(data));

    QCOMPARE(sim->f_PET_CMD_UPDATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_TEST(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_TEST(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_RESET(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RESET;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_RESET(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_SHUTDOWN(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_SHUTDOWN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_SHUTDOWN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_DISCOVERY(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_discovery_info data;

    frame.addr = 0;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_DISCOVERY(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(data)));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_CONFIG_SAVE(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_SAVE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_CONFIG_SAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_CONFIG_DEFAULT(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_DEFAULT;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_CONFIG_DEFAULT(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_GET_DEVICE_INFO(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_DEVICE_INFO;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_GET_DEVICE_INFO(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_device_info)));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_WAVE(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_wave_t wave;
    wave.channel = 0;
    wave.mode = 0;
    wave.count = 1;

    pet_wave_point_t point;

    frame.addr = 0;
    frame.cmd = PET_CMD_WAVE;
    frame.len = sizeof(wave) + sizeof(point);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&wave,     sizeof(wave));
    input.append((char *)&point,    sizeof(point));

    QCOMPARE(td->f_PET_CMD_WAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_wave_t) + sizeof(pet_wave_point_t)));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_RUN(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RUN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_RUN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_STOP(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_STOP;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_STOP(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_CONTROL(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_td_ctrl_t ctrl;

    //---
    for(unsigned int n=0; n<sizeof(ctrl); n++)
    {
        ((char *)&ctrl)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = sizeof(ctrl);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,  sizeof(frame));
    ba_test.append((char *)&ctrl,   sizeof(ctrl));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = sizeof(ctrl);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&ctrl,     sizeof(ctrl));

    QCOMPARE(td->f_PET_CMD_SET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_GET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_STATE(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_td_state_t state;
    pet_td_state_t test_state;

    //---
    test_state.state        = 0;    /* Состояние БОС (одно из значений PET_BOS_STATE) */
    test_state.flags        = 1;    /* Флаги состояния */
    test_state.sync_cycle   = 2;    /* Номер текущего цикла синхронизации */
    test_state.utilisation  = 3;    /* Общаяя загрузка по всем каналам регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_STATE;
    test_frame.len = sizeof(state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame, sizeof(test_frame));
    ba_test.append((char *)&test_state, sizeof(test_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_STATE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(td->f_PET_CMD_GET_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_CH_STATE(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_td_ch_state_t ch_state;
    pet_td_ch_state_t test_ch_state;

    uint16_t channel;
    channel = 0;

    //---
    test_ch_state.channel        = 0;    /* Номер канала */
    test_ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BOS_CH_STATE)*/
    test_ch_state.flags          = 2;    /* Флаги состояния */
    test_ch_state.sync_cycle     = 3;    /* Номер цикла синхронизации */
    test_ch_state.utilisation    = 4;    /* Загрузка канала регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_CH_STATE;
    test_frame.len = sizeof(test_ch_state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,     sizeof(test_frame));
    ba_test.append((char *)&test_ch_state,  sizeof(test_ch_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_STATE;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(td->f_PET_CMD_GET_CH_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_CH_CONTROL(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_td_ch_ctrl_t ch_control;

    uint16_t channel;
    channel = 0;

    //---
    for(unsigned int n=0; n<sizeof(ch_control); n++)
    {
        ((char *)&ch_control)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,      sizeof(frame));
    ba_test.append((char *)&ch_control, sizeof(ch_control));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    input.clear();
    input.append((char *)&frame,        sizeof(frame));
    input.append((char *)&ch_control,   sizeof(ch_control));

    QCOMPARE(td->f_PET_CMD_SET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(td->f_PET_CMD_GET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_TD_CMD_UPDATE(Device_TD *td)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_update_data data;

    frame.addr = 0;
    frame.cmd = PET_CMD_UPDATE;
    frame.len = sizeof(data);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&data,     sizeof(data));

    QCOMPARE(td->f_PET_CMD_UPDATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_TEST(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_TEST(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_RESET(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RESET;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_RESET(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_SHUTDOWN(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_SHUTDOWN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_SHUTDOWN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_DISCOVERY(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_discovery_info data;

    frame.addr = 0;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_DISCOVERY(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(data)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_CONFIG_SAVE(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_SAVE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_CONFIG_SAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_CONFIG_DEFAULT(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_CONFIG_DEFAULT;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_CONFIG_DEFAULT(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_GET_DEVICE_INFO(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_DEVICE_INFO;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_GET_DEVICE_INFO(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_device_info)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_WAVE(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_wave_t wave;
    wave.channel = 0;
    wave.mode = 0;
    wave.count = 1;

    pet_wave_point_t point;

    frame.addr = 0;
    frame.cmd = PET_CMD_WAVE;
    frame.len = sizeof(wave) + sizeof(point);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&wave,     sizeof(wave));
    input.append((char *)&point,    sizeof(point));

    QCOMPARE(brg_sim->f_PET_CMD_WAVE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(pet_wave_t) + sizeof(pet_wave_point_t)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_RUN(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_RUN;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_RUN(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_STOP(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    frame.addr = 0;
    frame.cmd = PET_CMD_STOP;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_STOP(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_CONTROL(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_brg_sim_ctrl_t ctrl;

    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = sizeof(ctrl);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&ctrl,     sizeof(ctrl));

    QCOMPARE(brg_sim->f_PET_CMD_SET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CONTROL;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_GET_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ctrl)));
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_STATE(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_brg_sim_state_t state;
    pet_brg_sim_state_t test_state;

    //---
    test_state.state             = 0;    /* Состояние БРГ (одно из значений PET_BRG_STATE) */
    test_state.flags             = 1;    /* Флаги состояния */
    test_state.sync_cycle        = 2;    /* Номер текущего цикла синхронизации */
    test_state.utilisation       = 3;    /* Общаяя загрузка по всем каналам регистрации */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_STATE;
    test_frame.len = sizeof(state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame, sizeof(test_frame));
    ba_test.append((char *)&test_state, sizeof(test_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_STATE;
    frame.len = 0;

    input.clear();
    input.append((char *)&frame,    sizeof(frame));

    QCOMPARE(brg_sim->f_PET_CMD_GET_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_CH_STATE(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;
    pet_frame test_frame;

    pet_brg_sim_ch_state_t ch_state;
    pet_brg_sim_ch_state_t test_ch_state;

    uint16_t channel;
    channel = 0;

    //---
    test_ch_state.channel        = 0;    /* Номер канала */
    test_ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BRG_CH_STATE)*/
    test_ch_state.flags          = 2;    /* Флаги состояния канала обработки */
    test_ch_state.in_offset      = 3;    /* Смещение сигнала ФЭУ (мВ) */
    test_ch_state.in_threshold   = 4;    /* Порог дискриминации (мВ) */
    test_ch_state.adc_reference  = 5;    /* Опорное напряжение АЦП (мВ) */
    test_ch_state.pm_bias        = 6;    /* Напряжение смещения ФЭУ (мВ) */
    test_ch_state.readout_temp   = 7;    /* Температура платы считывания (х0,01 °C) */
    test_ch_state.ifmodule_temp  = 8;    /* Температура модуля сопряжения (х0,01 °C) */
    test_ch_state.sync_cycle     = 9;    /* Номер цикла синхронизации */
    test_ch_state.utilisation    = 10;   /* Загрузка канала регистрации */
    test_ch_state.adc_underflow  = 11;   /* Число событий с признаком опустошения (потери значимости) данных АЦП */
    test_ch_state.adc_overflow   = 12;   /* Число событий с признаком переполнения (потери значимости) данных АЦП */
    test_ch_state.qdc_underflow  = 13;   /* Число событий с признаком опустошения (потери значимости) координатных сигналов */
    test_ch_state.qdc_overflow   = 14;   /* Число событий с признаком переполнения (потери значимости) координатных сигналов */
    test_ch_state.gate_overlay   = 15;   /* Число событий с признаком наложения событий */
    test_ch_state.ts_invalid     = 16;   /* Число событий с признаком не корректности метки времени */

    test_frame.addr = 0;
    test_frame.cmd = PET_CMD_GET_CH_STATE;
    test_frame.len = sizeof(test_ch_state);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&test_frame,     sizeof(test_frame));
    ba_test.append((char *)&test_ch_state,  sizeof(test_ch_state));
    //---

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_STATE;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(brg_sim->f_PET_CMD_GET_CH_STATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_state)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_CH_CONTROL(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_brg_sim_ch_ctrl_t ch_control;

    uint16_t channel;
    channel = 0;

    //---
    for(unsigned int n=0; n<sizeof(ch_control); n++)
    {
        ((char *)&ch_control)[n] = n;
    }

    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    QByteArray ba_test;
    ba_test.clear();
    ba_test.append((char *)&frame,      sizeof(frame));
    ba_test.append((char *)&ch_control, sizeof(ch_control));
    //---

    // set
    frame.addr = 0;
    frame.cmd = PET_CMD_SET_CH_CONTROL;
    frame.len = sizeof(ch_control);

    input.clear();
    input.append((char *)&frame,        sizeof(frame));
    input.append((char *)&ch_control,   sizeof(ch_control));

    QCOMPARE(brg_sim->f_PET_CMD_SET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    // get
    frame.addr = 0;
    frame.cmd = PET_CMD_GET_CH_CONTROL;
    frame.len = sizeof(channel);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&channel,  sizeof(channel));

    QCOMPARE(brg_sim->f_PET_CMD_GET_CH_CONTROL(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)(sizeof(pet_frame) + sizeof(ch_control)));

    if(output != ba_test)
    {
        qDebug() << "output " << output.toHex();
        qDebug() << "ba_test" << ba_test.toHex();
    }

    QCOMPARE(output == ba_test, true);
}
//--------------------------------------------------------------------------------
void Test::check_BRG_SIM_CMD_UPDATE(Device_BRG_SIM *brg_sim)
{
    QByteArray input;
    QByteArray output;

    pet_frame frame;

    pet_update_data data;

    frame.addr = 0;
    frame.cmd = PET_CMD_UPDATE;
    frame.len = sizeof(data);

    input.clear();
    input.append((char *)&frame,    sizeof(frame));
    input.append((char *)&data,     sizeof(data));

    QCOMPARE(brg_sim->f_PET_CMD_UPDATE(input, &output), PET_SUCCESS);
    QCOMPARE(output.length(), (int)sizeof(pet_frame));
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Test::check_bos(void)
{
    pet_device_info_t d_info;

    pet_bos_ctrl_t d_bos_ctrl;
    pet_bos_state_t d_bos_state;
    pet_bos_ch_ctrl_t d_bos_ch_ctrl;
    pet_bos_ch_state_t d_bos_ch_state;

    uint16_t channel = 0;
    int data_len = 0;

    PET_device_bos *bos = new PET_device_bos(0);
    if(bos == nullptr)
    {
        qCritical("bos not created");
        return;
    }
    PET_RESULT err = bos->set_comm_param(get_eth_address(),
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        qDebug() << PET_device_base::get_PET_RESULT_string(err);
    }
    bos->set_pet_addr(0);

    char d_test[100] = { 0 };
    data_len = 100;
    QCOMPARE(bos->test(&d_test, &data_len, &d_test, &data_len), PET_SUCCESS);

    QCOMPARE(bos->reset(),          PET_SUCCESS);
    QCOMPARE(bos->shutdown(),       PET_SUCCESS);
    QCOMPARE(bos->config_save(),    PET_SUCCESS);
    QCOMPARE(bos->config_default(), PET_SUCCESS);
    //TODO QCOMPARE(bos->update(),         PET_SUCCESS);
    QCOMPARE(bos->run(),            PET_SUCCESS);
    QCOMPARE(bos->stop(),           PET_SUCCESS);

    data_len = 0; QCOMPARE(bos->get_device_info(&d_info, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(bos->get_ctrl(&d_bos_ctrl, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(bos->get_state(&d_bos_state, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(bos->get_ch_ctrl(&channel, &data_len, &d_bos_ch_ctrl, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(bos->get_ch_state(&channel, &data_len, &d_bos_ch_state, &data_len), PET_SUCCESS);

    bos->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::check_brg(void)
{
    pet_device_info_t d_info;

    pet_brg_ctrl_t d_brg_ctrl;
    pet_brg_state_t d_brg_state;
    pet_brg_ch_ctrl_t d_brg_ch_ctrl;
    pet_brg_ch_state_t d_brg_ch_state;

    uint16_t channel = 0;
    int data_len = 0;

    PET_device_brg *brg = new PET_device_brg(0);
    if(brg == nullptr)
    {
        qCritical("brg not created");
        return;
    }
    PET_RESULT err = brg->set_comm_param(get_eth_address(),
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        qDebug() << PET_device_base::get_PET_RESULT_string(err);
    }
    brg->set_pet_addr(1);

    char d_test[100] = { 0 };
    data_len = 100;
    QCOMPARE(brg->get_test(&d_test, &data_len, &d_test, &data_len), PET_SUCCESS);

    QCOMPARE(brg->reset(),          PET_SUCCESS);
    QCOMPARE(brg->shutdown(),       PET_SUCCESS);
    QCOMPARE(brg->config_save(),    PET_SUCCESS);
    QCOMPARE(brg->config_default(), PET_SUCCESS);
    //TODO QCOMPARE(brg->update(),         PET_SUCCESS);
    QCOMPARE(brg->run(),            PET_SUCCESS);
    QCOMPARE(brg->stop(),           PET_SUCCESS);

    data_len = 0; QCOMPARE(brg->get_device_info(&d_info, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(brg->get_ctrl(&d_brg_ctrl, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(brg->get_state(&d_brg_state, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(brg->get_ch_ctrl(&channel, &data_len, &d_brg_ch_ctrl, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(brg->get_ch_state(&channel, &data_len, &d_brg_ch_state, &data_len), PET_SUCCESS);

    brg->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::check_sim(void)
{
    pet_device_info_t d_info;

    pet_bos_ctrl_t d_bos_ctrl;
    pet_bos_state_t d_bos_state;
    pet_bos_ch_ctrl_t d_bos_ch_ctrl;
    pet_bos_ch_state_t d_bos_ch_state;

    uint16_t channel = 0;
    int data_len = 0;

    PET_device_sim *sim = new PET_device_sim(0);
    if(sim == nullptr)
    {
        qCritical("sim not created");
        return;
    }
    PET_RESULT err = sim->set_comm_param(get_eth_address(),
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        qDebug() << PET_device_base::get_PET_RESULT_string(err);
    }
    sim->set_pet_addr(0);

    char d_test[100] = { 0 };
    data_len = 100;
    QCOMPARE(sim->get_test(&d_test, &data_len, &d_test, &data_len), PET_SUCCESS);

    QCOMPARE(sim->reset(),          PET_SUCCESS);
    QCOMPARE(sim->shutdown(),       PET_SUCCESS);
    QCOMPARE(sim->config_save(),    PET_SUCCESS);
    QCOMPARE(sim->config_default(), PET_SUCCESS);
    //TODO QCOMPARE(sim->update(),         PET_SUCCESS);
    QCOMPARE(sim->run(),            PET_SUCCESS);
    QCOMPARE(sim->stop(),           PET_SUCCESS);

    data_len = 0; QCOMPARE(sim->get_device_info(&d_info, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(sim->get_ctrl(&d_bos_ctrl, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(sim->get_state(&d_bos_state, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(sim->get_ch_ctrl(&channel, &data_len, &d_bos_ch_ctrl, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(sim->get_ch_state(&channel, &data_len, &d_bos_ch_state, &data_len), PET_SUCCESS);

    sim->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::check_td(void)
{
    pet_device_info_t d_info;

    pet_bos_ctrl_t d_bos_ctrl;
    pet_bos_state_t d_bos_state;
    pet_bos_ch_ctrl_t d_bos_ch_ctrl;
    pet_bos_ch_state_t d_bos_ch_state;

    uint16_t channel = 0;
    int data_len = 0;

    PET_device_td *td = new PET_device_td(0);
    if(td == nullptr)
    {
        qCritical("td not created");
        return;
    }
    PET_RESULT err = td->set_comm_param(get_eth_address(),
                                        PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        qDebug() << PET_device_base::get_PET_RESULT_string(err);
    }
    td->set_pet_addr(0);

    char d_test[100] = { 0 };
    data_len = 100; QCOMPARE(td->get_test(&d_test, &data_len, &d_test, &data_len), PET_SUCCESS);

    QCOMPARE(td->reset(),           PET_SUCCESS);
    QCOMPARE(td->shutdown(),        PET_SUCCESS);
    QCOMPARE(td->config_save(),     PET_SUCCESS);
    QCOMPARE(td->config_default(),  PET_SUCCESS);
    //TODO QCOMPARE(td->update(),          PET_SUCCESS);
    QCOMPARE(td->run(),             PET_SUCCESS);
    QCOMPARE(td->stop(),            PET_SUCCESS);

    data_len = 0; QCOMPARE(td->get_device_info(&d_info, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(td->get_ctrl(&d_bos_ctrl, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(td->get_state(&d_bos_state, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(td->get_ch_ctrl(&channel, &data_len, &d_bos_ch_ctrl, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(td->get_ch_state(&channel, &data_len, &d_bos_ch_state, &data_len), PET_SUCCESS);

    td->deleteLater();
}
//--------------------------------------------------------------------------------
void Test::check_brg_sim(void)
{
    pet_device_info_t d_info;

    pet_brg_ctrl_t d_brg_ctrl;
    pet_brg_state_t d_brg_state;
    pet_brg_ch_ctrl_t d_brg_ch_ctrl;
    pet_brg_ch_state_t d_brg_ch_state;

    uint16_t channel = 0;
    int data_len = 0;

    PET_device_brg_sim *brg_sim = new PET_device_brg_sim(0);
    if(brg_sim == nullptr)
    {
        qCritical("brg_sim not created");
        return;
    }
    PET_RESULT err = brg_sim->set_comm_param(get_eth_address(),
                                             PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        qDebug() << PET_device_base::get_PET_RESULT_string(err);
    }
    brg_sim->set_pet_addr(1);

    char d_test[100] = { 0 };
    data_len = 100; QCOMPARE(brg_sim->test(&d_test, &data_len, &d_test, &data_len), PET_SUCCESS);

    QCOMPARE(brg_sim->reset(),          PET_SUCCESS);
    QCOMPARE(brg_sim->shutdown(),       PET_SUCCESS);
    QCOMPARE(brg_sim->config_save(),    PET_SUCCESS);
    QCOMPARE(brg_sim->config_default(), PET_SUCCESS);
    //TODO QCOMPARE(brg_sim->update(),         PET_SUCCESS);
    QCOMPARE(brg_sim->run(),            PET_SUCCESS);
    QCOMPARE(brg_sim->stop(),           PET_SUCCESS);

    data_len = 0; QCOMPARE(brg_sim->get_device_info(&d_info, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(brg_sim->get_ctrl(&d_brg_ctrl, &data_len), PET_SUCCESS);
    data_len = 0; QCOMPARE(brg_sim->get_state(&d_brg_state, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(brg_sim->get_ch_ctrl(&channel, &data_len, &d_brg_ch_ctrl, &data_len), PET_SUCCESS);
    data_len = 2; QCOMPARE(brg_sim->get_ch_state(&channel, &data_len, &d_brg_ch_state, &data_len), PET_SUCCESS);

    brg_sim->deleteLater();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
