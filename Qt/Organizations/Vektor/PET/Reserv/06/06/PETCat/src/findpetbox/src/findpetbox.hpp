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
#ifndef FINDPETBOX_HPP
#define FINDPETBOX_HPP
//--------------------------------------------------------------------------------
#include <QModelIndex>
#include <QHostAddress>
#include <QDialog>
#include <QQueue>
//--------------------------------------------------------------------------------
#include "PET_comm.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class FindPetBox;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
//--------------------------------------------------------------------------------
class FindPetBox : public QDialog
{
    Q_OBJECT

public:
    explicit FindPetBox(QList<int> permitted_dev,
                        QWidget *parent);
    ~FindPetBox();

    QList<pet_discovery_data> get_current_pet_config(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void send_net_log(const QString &);
    void receive_net_log(const QString &);

public slots:
    void accept(void);
    void reject(void);
    bool add_pet_device(pet_discovery_data p_device);

private slots:
    void log(const QString data);
    void check_selected(void);
    void refresh_devices(void);

    void pet_select(QModelIndex);
    void doubleClicked(QModelIndex);

private:
    Ui::FindPetBox *ui = 0;

    bool pet_is_selected = false;

    QList<pet_discovery_data> pet_device_lists;

    QList<int> permitted_dev;

    PET_comm *pet = 0;
    bool block_close = false;

    void init(void);
    void connect_log(void);
    void init_tw_pets(void);

    bool add_item(QTreeWidgetItem *item);

    bool cmd_DISCOVERY(void);
    bool cmd_DISCOVERY_all_interfaces(void);

    void block_this_button(bool state);
    void block_widget(const QString name, bool state);
    void block_interface(bool state);

    static bool comp(pet_discovery_data_t &e1, pet_discovery_data_t &e2);

protected:
    void closeEvent(QCloseEvent *event);

};
Q_DECLARE_METATYPE(pet_discovery_data)
//--------------------------------------------------------------------------------
#endif
