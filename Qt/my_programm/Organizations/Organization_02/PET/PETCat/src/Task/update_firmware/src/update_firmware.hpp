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
enum {
    STATE_BEGIN = 0,
    STATE_END,
    STATE_ERROR
};
typedef struct BRG_DEVICE
{
    QString     device;
    uint8_t     pet_address;
    uint32_t    offset;
    uint16_t    sw_version;
    int32_t     sw_timestamp;
    int         state;
    QString     state_str;
} BRG_DEVICE_t;
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

    bool choice_firmware_brg(void);

    void btn_refresh_click(void);

    void btn_check_all_click(void);
    void btn_uncheck_all_click(void);

    void item_clicked(QTreeWidgetItem *item, int column);

private:
    QList<QTreeWidgetItem *> uf_list;
    QList<BRG_DEVICE *> brg_list;

    Ui::UpdateFirmware *ui;

    QList<pet_discovery_data_t> p_pet_config;
    PET_device_brg *brg;
    PET_comm *comm;

    QByteArray ba_firmware_brg;
    int cycles_brg = 0;

    bool is_working = false;

    void set_item_param(QTreeWidgetItem *item,
                        Qt::ItemFlags flags,
                        const QString &caption,
                        Qt::CheckState state);

    void init(void);
    void init_tw_main(void);

    bool consistent_work(void);
    bool parallel_work(void);
    void update_items(void);

    PET_RESULT update_device(uint8_t pet_address,
                             uint32_t cnt,
                             uint32_t offset,
                             QByteArray data);

    void update_tree(void);
    static bool comp(pet_discovery_data_t &e1, pet_discovery_data_t &e2);
};
//--------------------------------------------------------------------------------
#endif
