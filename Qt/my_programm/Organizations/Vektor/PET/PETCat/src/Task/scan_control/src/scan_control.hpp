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
#ifndef SCAN_CONTROL_HPP
#define SCAN_CONTROL_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class Scan_control;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
//--------------------------------------------------------------------------------
class Scan_control : public PET_device_brg
{
    Q_OBJECT

public:
    explicit Scan_control(QWidget *parent);
    ~Scan_control();

private slots:
    void refresh(void);
    bool start(void);
    bool stop(void);

    void update_tw_state_pet(void);
    void update_state(void);

    void auto_update(bool state);

    void collapsed_all(void);
    void expanding_all(void);

private:
    Ui::Scan_control *ui = 0;

    QList<pet_discovery_data_t> l_info;
    QList<pet_discovery_data> p_pet_config;

    QTimer *timer = 0;

    bool flag_busy = false;

    void init(void);
    void create_timer(void);

    void update_value(QString name, QString value);

    void find_item(QTreeWidgetItem *root, QVariant data, QString text);
    void replace_itemtext(QTreeWidgetItem *root, QString text);
    void expanded_item(QTreeWidgetItem *root);
    void collapsed_item(QTreeWidgetItem *root);
    void show_itemtext(QTreeWidgetItem *root);
};
//--------------------------------------------------------------------------------
#endif
