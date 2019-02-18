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
#ifndef B590_HPP
#define B590_HPP
//--------------------------------------------------------------------------------
#include <QModelIndex>
#include <QFrame>
#include <QList>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B590;
}
//--------------------------------------------------------------------------------
class Powersupply_B590;
class QToolButton;
class QToolBar;
class QHexEdit;

class QStringList;
//--------------------------------------------------------------------------------
enum {
    DATA_ADDRESS = 0,
    DATA_SIZE
};
//--------------------------------------------------------------------------------
class B590 : public MyWidget
{
    Q_OBJECT

public:
    B590(QWidget *parent = nullptr);
    ~B590();

private slots:
    void load_fram(void);
    void save_fram(void);
    void load_fram_from_file(void);
    void save_fram_to_file(void);

    void analize(void);
    bool save_data(uint16_t address,
                   uint8_t data);
    void test(void);

    void dataChanged(void);

    void find_devices(void);

    void calc_crc16_user_U(void);
    void calc_crc16_user_I(void);
    void calc_crc16_factory_U(void);
    void calc_crc16_factory_I(void);

    void tw_fram_clicked(QModelIndex index);

private:
    Ui::B590 *ui;

    Powersupply_B590  *powersupply;

    QByteArray fram_data;

    QHexEdit *he_fram;

    void createPowerSupplyBar(void);

    void create_hex_views(void);

    void init();

    void add_menu(void);

    //---
    void add_signature(void);
    void add_user_points_U(void);
    void add_user_points_I(void);
    void add_factory_points_U(void);
    void add_factory_points_I(void);
    void add_profile_points(void);
    void add_user_Value_K_Zero_U(void);
    void add_user_Value_K_Zero_I(void);
    void add_factory_Value_K_Zero_U(void);
    void add_factory_Value_K_Zero_I(void);
    void add_Bright(void);
    void add_Sound(void);
    void add_Count_Last_U_I(void);
    void add_Speed_UART(void);
    void add_Adr_ModBus(void);
    void add_moto_time(void);
    void add_Year_Factory(void);
    void add_Month_Factory(void);
    void add_Serial_Factory(void);

    void add_factor_k_DAC(void);
    void add_factor_k_ADC(void);
    void add_factor_S(void);

    void add_cal_temp(void);

    void add_anti_turn(void);
    void add_last_U(void);
    void add_last_I(void);
    //---

    void block_this_button(bool state);
    void block_interface(bool state);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
