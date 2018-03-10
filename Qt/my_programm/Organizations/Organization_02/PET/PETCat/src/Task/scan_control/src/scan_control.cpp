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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
#include "scan_control.hpp"
#include "ui_scan_control.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_td.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Scan_control::Scan_control(QWidget *parent) : PET_device_brg(parent),
    ui(new Ui::Scan_control)
{
    init();

    load_QDoubleSpinBox("Scan_control");
    load_QSpinBox("Scan_control");
    load_QComboBox("Scan_control");
}
//--------------------------------------------------------------------------------
Scan_control::~Scan_control()
{
    save_QDoubleSpinBox("Scan_control");
    save_QSpinBox("Scan_control");
    save_QComboBox("Scan_control");

    delete ui;
}
//--------------------------------------------------------------------------------
void Scan_control::refresh(void)
{
    QList<int> permitted_dev;
    permitted_dev.clear();
    permitted_dev.append(PET_PID_BOS);
    //permitted_dev.append(PET_PID_BRG);
    //permitted_dev.append(PET_PID_SIM);
    //permitted_dev.append(PET_PID_TD);
    //permitted_dev.append(PET_PID_BRG_SIM);

    FindPetBox *f = new FindPetBox(permitted_dev, this);
    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }
    p_pet_config = f->get_current_pet_config();
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
    update_tw_state_pet();
    update_state();
}
//--------------------------------------------------------------------------------
bool Scan_control::start(void)
{
    //---
    if(ui->le_device->text().isEmpty())
    {
        QString title = "Ошибка";
        QString text = "Томограф не выбран";

        messagebox_critical(title, text);
        return false;
    }
    //---
    PET_RESULT err = set_comm_param(p_pet_config.at(0).host,
                                    p_pet_config.at(0).port);
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }
    //---
    emit info("Запускается сбор данных");
    QVariant max_pet_timeout = 3000;
    PETCat_options::get_max_pet_timeout(&max_pet_timeout);
    set_max_waiting_msec(max_pet_timeout.toInt());

    err = PET_device_base::run();
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }
    else
    {
        emit info("Выполнено");
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Scan_control::stop(void)
{
    //---
    if(ui->le_device->text().isEmpty())
    {
        QString title = "Ошибка";
        QString text  = "Томограф не выбран";

        messagebox_critical(title, text);
        return false;
    }
    //---
    //---
    emit info("Останавливается сбор данных");
    QVariant max_pet_timeout = 3000;
    PETCat_options::get_max_pet_timeout(&max_pet_timeout);
    set_max_waiting_msec(max_pet_timeout.toInt());

    PET_device_base::stop();

    emit info("Выполнено");
    return true;
}
//--------------------------------------------------------------------------------
void Scan_control::init(void)
{
    ui->setupUi(this);

    ui->le_device->setDisabled(true);

    create_timer();

    //---
    ui->btn_collapsed_all->setDisabled(true);
    ui->btn_expanding_all->setDisabled(true);
    //---
    ui->sb_interval_auto_state->setRange(1000, 1e6);
    connect(ui->btn_refresh_state,      SIGNAL(clicked(bool)),  this,                       SLOT(update_state()));
    connect(ui->cb_auto_refresh_state,  SIGNAL(toggled(bool)),  ui->sb_interval_auto_state, SLOT(setDisabled(bool)));
    connect(ui->cb_auto_refresh_state,  SIGNAL(toggled(bool)),  ui->btn_refresh_state,      SLOT(setDisabled(bool)));
    connect(ui->cb_auto_refresh_state,  SIGNAL(toggled(bool)),  this,                       SLOT(auto_update(bool)));
    //---
    pet_discovery_data data;
    p_pet_config.clear();
    p_pet_config.append(data);
    p_pet_config[0].host = QHostAddress::LocalHost;
    p_pet_config[0].port = PETCat_options::get_port_cmd();
    p_pet_config[0].addr = 0;
    //---
    QStringList sl;
    sl << "Устройство";
    sl << "Адрес";
    sl << "Загрузка";
    sl << "Цикл синхронизации";
    sl << "Состояние";

    ui->tw_state_pet->setHeaderLabels(sl);
    ui->tw_state_pet->clear();
    //---
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    connect(ui->btn_start,      SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,       SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    //---
    connect(ui->btn_collapsed_all,  SIGNAL(clicked(bool)),  this,   SLOT(collapsed_all()));
    connect(ui->btn_expanding_all,  SIGNAL(clicked(bool)),  this,   SLOT(expanding_all()));
    //---
    block_wheel();
}
//--------------------------------------------------------------------------------
void Scan_control::create_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_state()));
}
//--------------------------------------------------------------------------------
void Scan_control::update_tw_state_pet(void)
{
    l_info.clear();
    //---
    QVariant max_pet_timeout = 1000;
    PETCat_options::get_max_pet_timeout(&max_pet_timeout);
    PET_RESULT res = PET_device_base::discovery(this,
                                                comm,
                                                p_pet_config.at(0).host,
                                                p_pet_config.at(0).port,
                                                PET_ADDR_BROADCAST,
                                                max_pet_timeout.toInt(),
                                                &l_info);
    if(res != PET_SUCCESS)
    {
        QString title = "Ошибка";
        QString text = QString("%1\n").arg(PET_device_base::get_PET_RESULT_string(res));
        messagebox_critical(title, text);

        emit error(PET_device_base::get_PET_RESULT_string(res));
        return;
    }
    //---

    //---
    ui->tw_state_pet->clear();

    QTreeWidgetItem *root = 0;
    root = new QTreeWidgetItem(QStringList() << PET_device_base::get_device_type_short_str(p_pet_config.at(0).pid));
    root->setData(0, Qt::UserRole, 0);
    root->setIcon(0, QIcon(NAME_RADIATION_ICON));
    root->setText(0, PET_device_base::get_device_type_short_str(p_pet_config.at(0).pid));
    root->setText(1, "0");

    ui->tw_state_pet->clear();
    ui->tw_state_pet->addTopLevelItem(root);
    root->setExpanded(true);

    foreach (pet_discovery_data_t packet, l_info)
    {
        //emit debug(QString("PID %1").arg(PET_device_base::get_PET_PID_string(packet.pid)));
        if(packet.pid == PET_PID_BRG)
        {
            QTreeWidgetItem *item_brg = new QTreeWidgetItem();
            //---
            item_brg->setIcon(0, QIcon(NAME_RADIATION_ICON));
            //---
            item_brg->setText(0, PET_device_base::get_device_type_short_str(packet.pid));
            //---
            item_brg->setText(1, QString("%1").arg(packet.addr));
            //---
            QString t0 = QString("brg_%1_utilisation").arg(packet.addr);
            item_brg->setData(2, Qt::UserRole, t0);
            item_brg->setText(2, "---");
            //---
            QString t1 = QString("brg_%1_sync_cycle").arg(packet.addr);
            item_brg->setData(3, Qt::UserRole, t1);
            item_brg->setText(3, "---");
            //---
            QString t2 = QString("brg_%1_state").arg(packet.addr);
            item_brg->setData(4, Qt::UserRole, t2);
            item_brg->setText(4, "---");
            //---
            item_brg->setExpanded(true);
            //---
            if(root)
                root->addChild(item_brg);
            else
                ui->tw_state_pet->addTopLevelItem(item_brg);
            //---

            for(int n=0; n<PET_BRG_CH_NUM; n++)
            {
                QTreeWidgetItem *item_channel = new QTreeWidgetItem();
                //---
                item_channel->setText(0, QString("Канал %1").arg(n));
                //---
                QString c0 = QString("channel_%1_%2_utilisation").arg(packet.addr).arg(n);
                item_channel->setData(2, Qt::UserRole, c0);
                item_channel->setText(2, "---");
                //---
                QString c1 = QString("channel_%1_%2_sync_cycle").arg(packet.addr).arg(n);
                item_channel->setData(3, Qt::UserRole, c1);
                item_channel->setText(3, "---");
                //---
                QString c2 = QString("channel_%1_%2_state").arg(packet.addr).arg(n);
                item_channel->setData(4, Qt::UserRole, c2);
                item_channel->setText(4, "---");
                //---
                item_brg->addChild(item_channel);
            }
        }
    }

    expanded_item(root);
    ui->tw_state_pet->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void Scan_control::update_value(QString name,
                                QString value)
{
    QTreeWidgetItem *root = ui->tw_state_pet->topLevelItem(0);
    if(!root)
    {
        return;
    }

    find_item(root, name, value);
}
//--------------------------------------------------------------------------------
void Scan_control::find_item(QTreeWidgetItem *root,
                             QVariant data,
                             QString text)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        if(root->data(n, Qt::UserRole) == data)
        {
            root->setText(n, text);
            return;
        }
    }

    for(int n=0; n<root->childCount(); n++)
    {
        find_item(root->child(n), data, text);
    }
}
//--------------------------------------------------------------------------------
void Scan_control::show_itemtext(QTreeWidgetItem *root)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        emit info(root->text(n));
    }

    for(int n=0; n<root->childCount(); n++)
    {
        show_itemtext(root->child(n));
    }
}
//--------------------------------------------------------------------------------
void Scan_control::replace_itemtext(QTreeWidgetItem *root, QString text)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        root->setText(n, text);
    }

    for(int n=0; n<root->childCount(); n++)
    {
        replace_itemtext(root->child(n), text);
    }
}
//--------------------------------------------------------------------------------
void Scan_control::expanded_item(QTreeWidgetItem *root)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        root->setExpanded(true);
    }

    for(int n=0; n<root->childCount(); n++)
    {
        expanded_item(root->child(n));
    }
}
//--------------------------------------------------------------------------------
void Scan_control::collapsed_item(QTreeWidgetItem *root)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        root->setExpanded(false);
    }

    for(int n=0; n<root->childCount(); n++)
    {
        collapsed_item(root->child(n));
    }
}
//--------------------------------------------------------------------------------
void Scan_control::update_state(void)
{
    if(flag_busy == true)
    {
        //emit error("Занято");
        return;
    }
    flag_busy = true;

    foreach (pet_discovery_data_t packet, p_pet_config)
    {
        emit debug(QString("PID %1").arg(PET_device_base::get_PET_PID_string(packet.pid)));
        if(packet.pid == PET_PID_BRG)
        {
            pet_brg_state_t brg_state;
            memset((char *)&brg_state, 0, sizeof(brg_state));
            int len = 0;

            set_pet_addr(packet.addr);
            PET_RESULT err = get_state(&brg_state, &len);
            if(err == PET_SUCCESS)
            {
                QString t_state = "Отключено";
                switch(brg_state.state)
                {
                case PET_BRG_STATE_DISABLE:     t_state = "Отключено";      break;
                case PET_BRG_STATE_IDLE:        t_state = "Ожидание";       break;
                case PET_BRG_STATE_SCAN:        t_state = "Сбор данных";    break;
                case PET_BRG_STATE_CALIBRATE:   t_state = NAME_CALIBRATION;     break;
                default:                        t_state = "н/д";            break;
                }

                bool f_sync_cycle = brg_state.flags & PET_BRG_STATE_FLAG_SYNC_CYCLE_MSK;
                bool f_utilisation = brg_state.flags & PET_BRG_STATE_FLAG_UTILISATION_MSK;

                QString s_utilisation = "н/д";
                QString s_sync_cycle  = "н/д";
                QString s_state;

                if(f_utilisation)
                    s_utilisation = QString("%1").arg(brg_state.utilisation);

                if(f_sync_cycle)
                    s_sync_cycle = QString("%1").arg(brg_state.sync_cycle);

                s_state = QString("%1").arg(t_state);

                update_value(QString("brg_%1_utilisation").arg(packet.addr),    s_utilisation);
                update_value(QString("brg_%1_sync_cycle").arg(packet.addr),     s_sync_cycle);
                update_value(QString("brg_%1_state").arg(packet.addr),          s_state);
            }
            else
            {
                update_value(QString("brg_%1_utilisation").arg(packet.addr),    "н/д");
                update_value(QString("brg_%1_sync_cycle").arg(packet.addr),     "н/д");
                update_value(QString("brg_%1_state").arg(packet.addr),          "н/д");
                emit error(PET_device_base::get_PET_RESULT_string(err));
            }

            for(int n=0; n<PET_BRG_CH_NUM; n++)
            {
                pet_brg_ch_state state;
                memset((char *)&state, 0, sizeof(state));
                int len_state = sizeof(state);
                uint16_t ch = n;
                int len = sizeof(ch);

                set_pet_addr(packet.addr);
                emit debug(QString("pet_address %1 channel %2")
                           .arg(get_pet_addr())
                           .arg(ch));
                PET_RESULT err = get_ch_state(&ch, &len,
                                              &state, &len_state);
                if(err == PET_SUCCESS)
                {
                    QString t_state = "Отключено";
                    switch(state.state)
                    {
                    case PET_BRG_STATE_DISABLE:     t_state = "Отключено";  break;
                    case PET_BRG_STATE_IDLE:        t_state = "Ожидание";   break;
                    case PET_BRG_STATE_SCAN:        t_state = "Сбор данных";    break;
                    case PET_BRG_STATE_CALIBRATE:   t_state = NAME_CALIBRATION; break;
                    default:
                        t_state = "н/д";
                        break;
                    }

                    bool f_sync_cycle   = state.flags & PET_BRG_STATE_FLAG_SYNC_CYCLE_MSK;
                    bool f_utilisation  = state.flags & PET_BRG_STATE_FLAG_UTILISATION_MSK;

                    QString s_utilisation = "н/д";
                    QString s_sync_cycle  = "н/д";
                    QString s_state;

                    if(f_utilisation)
                        s_utilisation = QString("%1").arg(state.utilisation);

                    if(f_sync_cycle)
                        s_sync_cycle = QString("%1").arg(state.sync_cycle);

                    s_state = QString("%1").arg(t_state);

                    update_value(QString("channel_%1_%2_utilisation").arg(packet.addr).arg(n),    s_utilisation);
                    update_value(QString("channel_%1_%2_sync_cycle").arg(packet.addr).arg(n),     s_sync_cycle);
                    update_value(QString("channel_%1_%2_state").arg(packet.addr).arg(n),          s_state);
                }
                else
                {
                    update_value(QString("channel_%1_%2_utilisation").arg(packet.addr).arg(n),    "н/д");
                    update_value(QString("channel_%1_%2_sync_cycle").arg(packet.addr).arg(n),     "н/д");
                    update_value(QString("channel_%1_%2_state").arg(packet.addr).arg(n),          "н/д");
                    emit error(PET_device_base::get_PET_RESULT_string(err));
                }
            }
        }
    }
    flag_busy = false;
}
//--------------------------------------------------------------------------------
void Scan_control::auto_update(bool state)
{
    if(timer == 0)
    {
        return;
    }

    if(state)
    {
        timer->start(ui->sb_interval_auto_state->value());
    }
    else
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Scan_control::collapsed_all(void)
{
    QTreeWidgetItem *root = ui->tw_state_pet->topLevelItem(0);
    if(root)
    {
        collapsed_item(root);
    }
}
//--------------------------------------------------------------------------------
void Scan_control::expanding_all(void)
{
    QTreeWidgetItem *root = ui->tw_state_pet->topLevelItem(0);
    if(root)
    {
        expanded_item(root);
    }
}
//--------------------------------------------------------------------------------
