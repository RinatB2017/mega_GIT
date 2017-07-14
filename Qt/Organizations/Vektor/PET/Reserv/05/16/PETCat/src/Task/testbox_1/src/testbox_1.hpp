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
#ifndef TESTBOX_1_HPP
#define TESTBOX_1_HPP
//--------------------------------------------------------------------------------
#include <QUdpSocket>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "demo_device_bos.hpp"
#include "demo_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class TestBox_1;
}
//--------------------------------------------------------------------------------
class TestBox_1 : public MyWidget
{
    Q_OBJECT

public:
    explicit TestBox_1(QList<pet_discovery_data> pet_config,
                       QWidget *parent);
    ~TestBox_1();

    int get_PID(void);

private slots:
    void test(void);

private:
    Ui::TestBox_1 *ui = 0;

    QList<pet_discovery_data> p_pet_config;

    void init(void);

    Device_BOS *device = 0;
    PET_RESULT check_packet(uint8_t addr,
                            uint8_t cmd,
                            uint8_t len,
                            QByteArray input_data,
                            QByteArray *output_packet);
};
//--------------------------------------------------------------------------------
#endif
