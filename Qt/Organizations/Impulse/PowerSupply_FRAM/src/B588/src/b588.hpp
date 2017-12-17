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
#ifndef B588_HPP
#define B588_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
#include <QList>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
class Powersupply_B588;
class QToolButton;
class QToolBar;
class QHexEdit;

class QStringList;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    B588(QWidget *parent = 0);
    ~B588();

private slots:
    void read_fram(void);
    void save_data_to_fram(void);
    void analize(void);
    bool save_data(uint16_t address,
                   uint8_t data);
    void test(void);

    void save_fram_to_file(void);

    void dataChanged(void);

    void find_devices(void);

    void popup_user_U(QPoint);
    void popup_user_I(QPoint);
    void popup_factory_U(QPoint);
    void popup_factory_I(QPoint);

    void calc_crc16_user_U(void);
    void calc_crc16_user_I(void);
    void calc_crc16_factory_U(void);
    void calc_crc16_factory_I(void);

private:
    Ui::B588 *ui = 0;

    Powersupply_B588  *powersupply = 0;

    QByteArray fram_data;

    QHexEdit *he = 0;
    QHexEdit *he_user_U = 0;
    QHexEdit *he_user_I = 0;
    QHexEdit *he_factory_U = 0;
    QHexEdit *he_factory_I = 0;

    void createTestBar(void);
    void createPowerSupplyBar(void);

    void create_hex_views(void);

    void init();

    void add_user_points_U(void);
    void add_user_points_I(void);
    void add_factory_points_U(void);
    void add_factory_points_I(void);
    void add_profile_points(void);
    void add_Value_K_Zero_U(void);
    void add_Value_K_Zero_I(void);
    void add_Bright(void);
    void add_Sound(void);
    void add_Adr_GPIB(void);
    void add_Count_Last_U_I(void);
    void add_Save_Counts_U_I(void);
    void add_Speed_UART(void);
    void add_Adr_ModBus(void);
    void add_moto_time(void);
    void add_Year_Factory(void);
    void add_Month_Factory(void);
    void add_Serial_Factory(void);

    void add_factor_k(void);
    void add_disable_factor_k(void);
    void add_cal_temp(void);

    void block_this_button(bool state);
    void block_interface(bool state);
};
//--------------------------------------------------------------------------------
#endif
