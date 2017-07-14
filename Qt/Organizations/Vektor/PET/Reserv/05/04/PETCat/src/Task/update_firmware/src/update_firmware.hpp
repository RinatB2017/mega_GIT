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
#ifndef UPDATE_FIRMWARE_HPP
#define UPDATE_FIRMWARE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_comm.hpp"
//--------------------------------------------------------------------------------
#include "PET_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class UpdateFirmware;
}
//--------------------------------------------------------------------------------
class QProgressBar;
class QCheckBox;
class QLabel;
//--------------------------------------------------------------------------------
class UpdateFirmware : public MyWidget
{
    Q_OBJECT

public:
    explicit UpdateFirmware(QList<pet_discovery_data> pet_config,
                            QWidget *parent);
    ~UpdateFirmware();

private slots:
    void refresh(void);
    void update(void);

    void choice_firmware_brg(void);

    void item_clicked(QTreeWidgetItem *item, int column);

private:
    QList<QTreeWidgetItem *> uf_list;

    Ui::UpdateFirmware *ui = 0;

    PET_comm *comm = 0;
    QList<pet_discovery_data_t> p_pet_config;
    PET_device_brg *brg = 0;

    QByteArray ba_firmware_brg;
    int cycles_brg = 0;

    QTreeWidgetItem *root = 0;
    void set_item_param(QTreeWidgetItem *item,
                        Qt::ItemFlags flags,
                        const QString &caption,
                        Qt::CheckState state);

    void init(void);
    void init_tw_main(void);

    void work(void);

    void update_tree(void);
    bool update_device(QTreeWidgetItem *item);
    static bool comp(pet_discovery_data_t &e1, pet_discovery_data_t &e2);
};
//--------------------------------------------------------------------------------
#endif
