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
#include <QProgressDialog>
#include <QProgressBar>
#include <QStyleFactory>

#include <QDoubleSpinBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QToolButton>
#include <QStringList>
#include <QtGlobal>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
//--------------------------------------------------------------------------------
#include "ui_findpetbox.h"
//--------------------------------------------------------------------------------
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "myprogressdialog.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
FindPetBox::FindPetBox(QList<int> permitted_dev,
                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindPetBox),
    permitted_dev(permitted_dev)
{
    init();

#ifdef QT_DEBUG
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
#endif
}
//--------------------------------------------------------------------------------
FindPetBox::~FindPetBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void FindPetBox::init(void)
{
    ui->setupUi(this);
    connect_log();

    //---
    //setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    //---
    ui->btn_ok->setDisabled(true);
    //---
    bool ok = false;
    //---
    pet = new PET_comm();
    connect(pet,    SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(pet,    SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(pet,    SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(pet,    SIGNAL(trace(QString)),     this,   SIGNAL(trace(QString)));

    connect(pet,    SIGNAL(send_net_log(QString)),      this,   SIGNAL(send_net_log(QString)));
    connect(pet,    SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));

    //---
    QStringList sl = PETCat_options::get_eth_addresses();
    ui->cb_ethernet->addItem("Все");    //TODO
    foreach (QString addr, sl)
    {
        ui->cb_ethernet->addItem(addr);
    }
    //---
    QHostAddress addr = QHostAddress(ui->cb_ethernet->currentText());
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    PET_RESULT err = pet->set_comm_param(addr, PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }
    ok = pet->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(pet->get_last_error());
    }
    //---
    pet_is_selected = false;

    init_tw_pets();

    connect(ui->btn_ok,         SIGNAL(clicked(bool)),  this, SLOT(check_selected()));
    connect(ui->btn_cancel,     SIGNAL(clicked(bool)),  this, SLOT(reject()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this, SLOT(refresh_devices()));

    setWindowTitle(tr("Выбор ПЭТ"));
    setMinimumSize(800, 400);

    QTimer::singleShot(100, this, SLOT(refresh_devices()));
}
//--------------------------------------------------------------------------------
void FindPetBox::log(const QString data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void FindPetBox::connect_log(void)
{
    if(parentWidget())
    {
        connect(this,   SIGNAL(info(QString)),      parentWidget(), SIGNAL(info(QString)));
        connect(this,   SIGNAL(debug(QString)),     parentWidget(), SIGNAL(debug(QString)));
        connect(this,   SIGNAL(error(QString)),     parentWidget(), SIGNAL(error(QString)));
        connect(this,   SIGNAL(trace(QString)),     parentWidget(), SIGNAL(trace(QString)));
    }
    else
    {
        connect(this,   SIGNAL(info(QString)),      this,   SLOT(log(QString)));
        connect(this,   SIGNAL(debug(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(error(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(trace(QString)),     this,   SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void FindPetBox::init_tw_pets(void)
{
    ui->tw_pets->clear();

    QStringList sl;
    sl << "Устройство";
    sl << "Сетевой адрес";
    sl << "Адрес ПЭТ";
    sl << "UID";

    ui->tw_pets->setHeaderLabels(sl);

    ui->tw_pets->sortByColumn(2, Qt::AscendingOrder);
    ui->tw_pets->setSortingEnabled(true);

    connect(ui->tw_pets,    SIGNAL(clicked(QModelIndex)),       this,   SLOT(pet_select(QModelIndex)));
    connect(ui->tw_pets,    SIGNAL(doubleClicked(QModelIndex)), this,   SLOT(doubleClicked(QModelIndex)));
}
//--------------------------------------------------------------------------------
void FindPetBox::pet_select(QModelIndex)
{
    block_widget("btn_ok", true);

    QTreeWidgetItem *item = ui->tw_pets->currentItem();
    foreach (int type, permitted_dev)
    {
        QVariant var = item->data(0, Qt::UserRole);
        if(var.isValid())
        {
            pet_discovery_data pet_device = var.value<pet_discovery_data>();
            if(type == (int)pet_device.pid)
            {
                block_widget("btn_ok", false);
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
void FindPetBox::doubleClicked(QModelIndex)
{
    QTreeWidgetItem *item = ui->tw_pets->currentItem();
    foreach (int type, permitted_dev)
    {
        QVariant var = item->data(0, Qt::UserRole);
        if(var.isValid())
        {
            pet_discovery_data pet_device = var.value<pet_discovery_data>();
            if(type == (int)pet_device.pid)
            {
                check_selected();
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool FindPetBox::add_pet_device(pet_discovery_data p_device)
{
    emit info(QString("найден %1 %2:%3 адрес %4")
              .arg(PET_device_base::get_PET_PID_string(p_device.pid))
              .arg(p_device.host.toString())
              .arg(p_device.port)
              .arg(p_device.addr));


    QString uid_data;
    uid_data.clear();
    uid_data.append(QString("%1").arg(p_device.uid[0], 2, 16, QChar('0')).toUpper());
    uid_data.append(":");
    uid_data.append(QString("%1").arg(p_device.uid[1], 2, 16, QChar('0')).toUpper());
    uid_data.append(":");
    uid_data.append(QString("%1").arg(p_device.uid[2], 2, 16, QChar('0')).toUpper());
    uid_data.append(":");
    uid_data.append(QString("%1").arg(p_device.uid[3], 2, 16, QChar('0')).toUpper());
    uid_data.append(":");
    uid_data.append(QString("%1").arg(p_device.uid[4], 2, 16, QChar('0')).toUpper());
    uid_data.append(":");
    uid_data.append(QString("%1").arg(p_device.uid[5], 2, 16, QChar('0')).toUpper());

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, QVariant::fromValue(p_device));
    //---
    item->setText(0, PET_device_base::get_PET_PID_string(p_device.pid));
    if(p_device.addr == 0)
    {
        item->setText(1, QString("%1:%2")
                      .arg(p_device.host.toString())
                      .arg(p_device.port));
    }
    item->setText(2, QString("%1").arg(p_device.addr, 2, 10, QChar('0')));
    item->setText(3, uid_data);

    ui->tw_pets->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    return add_item(item);
}
//--------------------------------------------------------------------------------
bool FindPetBox::add_item(QTreeWidgetItem *item)
{
    if(item == nullptr)
    {
        return false;
    }

    if(ui->tw_pets->topLevelItemCount() == 0)
    {
        QTreeWidgetItem *temp_root = new QTreeWidgetItem;
        temp_root->setText(0, "---");
        temp_root->setText(1, "---");
        temp_root->setText(2, "---");
        temp_root->setText(3, "---");
        temp_root->setExpanded(true);

        ui->tw_pets->addTopLevelItem(temp_root);
    }
    if(item->text(2)== "00")
    {
        QTreeWidgetItem *root = ui->tw_pets->topLevelItem(0);
        if(root)
        {
            root->setText(0, item->text(0));
            root->setText(1, item->text(1));
            root->setText(2, item->text(2));
            root->setText(3, item->text(3));
            root->setData(0, Qt::UserRole, item->data(0, Qt::UserRole));
        }
        else
        {
            qFatal("root is NULL");
        }
    }
    else
    {
        QTreeWidgetItem *root = ui->tw_pets->topLevelItem(0);
        if(root)
        {
            root->addChild(item);
            //root->setExpanded(true);
        }
    }

    return true;
}
//--------------------------------------------------------------------------------
void FindPetBox::refresh_devices(void)
{
    block_this_button(true);
    block_widget("btn_ok", true);
    block_widget("cb_ethernet", true);

    block_close = true;
    emit debug("refresh_devices");
    if(ui->cb_ethernet->currentText() == "Все")
    {
        cmd_DISCOVERY_all_interfaces();
    }
    else
    {
        cmd_DISCOVERY();
    }

    block_this_button(false);
    block_widget("cb_ethernet", false);

    block_close = false;
}
//--------------------------------------------------------------------------------
void FindPetBox::check_selected(void)
{
    if(ui->tw_pets->topLevelItemCount() == 0)
    {
        MyWidget::messagebox_critical("Ошибка", "Нет доступных томографов");
        return;
    }

    QList<QTreeWidgetItem*> lists = ui->tw_pets->selectedItems();
    if(lists.count() == 0)
    {
        MyWidget::messagebox_critical("Ошибка", "Томограф не выбран");
        return;
    }
    emit debug(QString("check_selected: count %1").arg(lists.count()));

    pet_device_lists.clear();
    QTreeWidgetItem *item = ui->tw_pets->currentItem();
    if(item)
    {
        //---
        if(item->text(0) == "---")  return;
        if(item->text(1) == "---")  return;
        if(item->text(2) == "---")  return;
        if(item->text(3) == "---")  return;
        //---
        emit debug(QString(">>> %1").arg(item->text(0)));
        QVariant var = item->data(0, Qt::UserRole);
        if(var.isValid())
        {
            pet_discovery_data pet_device = var.value<pet_discovery_data>();
            emit debug(QString(">>> host %1").arg(pet_device.host.toString()));
            emit debug(QString(">>> address %1").arg(pet_device.addr));
            pet_device_lists.append(pet_device);
        }
        for(int n=0; n<item->childCount(); n++)
        {
            QTreeWidgetItem *child = item->child(n);
            emit debug(QString(">>> %1").arg(child->text(0)));
            QVariant var = child->data(0, Qt::UserRole);
            if(var.isValid())
            {
                pet_discovery_data pet_device = var.value<pet_discovery_data>();
                emit debug(QString(">>> host %1").arg(pet_device.host.toString()));
                emit debug(QString(">>> address %1").arg(pet_device.addr));
                pet_device_lists.append(pet_device);
            }
        }
    }

    emit debug(QString("added %1 items").arg(pet_device_lists.count()));
    qSort(pet_device_lists.begin(), pet_device_lists.end(), comp);
    accept();
}
//--------------------------------------------------------------------------------
bool FindPetBox::cmd_DISCOVERY(void)
{
    QList<pet_discovery_data_t> l_info;
    //---
    ui->tw_pets->clear();
    //---
    emit info("Поиск устройств:");
    //---
    QHostAddress addr = QHostAddress(ui->cb_ethernet->currentText());
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    qDebug() << "addr" << addr.toString();
    //---
    PET_RESULT err = pet->set_comm_param(addr,
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }
    QVariant max_net_timeout = 3000; //TODO
    PETCat_options::get_max_net_timeout(&max_net_timeout);
    block_widget("btn_cancel",  true);
    block_widget("btn_refresh", true);

    //---
    //MyProgressDialog *dlg = new MyProgressDialog("Поиск устройств", "Пожалуйста, ждите!", this);
    //dlg->show();

    PET_RESULT res = PET_device_base::discovery(this,
                                                pet,
                                                addr,
                                                PETCat_options::get_port_cmd(),
                                                PET_ADDR_BROADCAST,
                                                max_net_timeout.toInt(),
                                                &l_info);
    //dlg->close();
    //---

    block_widget("btn_cancel",  false);
    block_widget("btn_refresh", false);
    if(res != PET_SUCCESS)
    {
        MyWidget::messagebox_critical("Ошибка", "Нет доступных томографов!");

        emit error(PET_device_base::get_PET_RESULT_string(res));
        return false;
    }

    //TODO
    //---
    // сортировка
    //qSort(l_info.begin(), l_info.end(), comp);
    //---
    //foreach (pet_discovery_data_t data, l_info)
    //{
    //    add_pet_device(data);
    //}

    return true;
}
//--------------------------------------------------------------------------------
bool FindPetBox::cmd_DISCOVERY_all_interfaces(void)
{
    QList<pet_discovery_data_t> l_info;
    //---
    ui->tw_pets->clear();
    //---
    emit info("Поиск устройств:");
    //---
    //MyProgressDialog *dlg = new MyProgressDialog("Поиск устройств", "Пожалуйста, ждите!", this);
    //dlg->show();
    //---
    QStringList sl = PETCat_options::get_eth_addresses();
    PET_RESULT res = 0;
    foreach (QString i_addr, sl)
    {
        QHostAddress addr = QHostAddress(i_addr);
        quint32 t_addr = QHostAddress(addr).toIPv4Address();
        t_addr |= 0x000000FF;
        addr = QHostAddress(t_addr);
        //---
        qDebug() << "addr" << addr.toString();
        //---
        PET_RESULT err = pet->set_comm_param(addr,
                                             PETCat_options::get_port_cmd());
        if(err != PET_SUCCESS)
        {
            emit error(PET_device_base::get_PET_RESULT_string(err));
        }
        QVariant max_net_timeout = 3000; //TODO
        PETCat_options::get_max_net_timeout(&max_net_timeout);
        block_widget("btn_cancel",  true);
        block_widget("btn_refresh", true);


        res = PET_device_base::discovery(this,
                                         pet,
                                         addr,
                                         PETCat_options::get_port_cmd(),
                                         PET_ADDR_BROADCAST,
                                         max_net_timeout.toInt(),
                                         &l_info);
    }
    //dlg->close();
    //---

    block_widget("btn_cancel",  false);
    block_widget("btn_refresh", false);
    if(res != PET_SUCCESS)
    {
        MyWidget::messagebox_critical("Ошибка", "Нет доступных томографов!");

        emit error(PET_device_base::get_PET_RESULT_string(res));
        return false;
    }

    //TODO
    //---
    // сортировка
    //qSort(l_info.begin(), l_info.end(), comp);
    //---
    //foreach (pet_discovery_data_t data, l_info)
    //{
    //    add_pet_device(data);
    //}

    return true;
}
//--------------------------------------------------------------------------------
bool FindPetBox::comp(pet_discovery_data_t &e1, pet_discovery_data_t &e2)
{
    return e1.addr < e2.addr;
}
//--------------------------------------------------------------------------------
QList<pet_discovery_data> FindPetBox::get_current_pet_config(void)
{
    return pet_device_lists;
}
//--------------------------------------------------------------------------------
void FindPetBox::accept(void)
{
    QDialog::accept();
}
//--------------------------------------------------------------------------------
void FindPetBox::reject(void)
{
    QDialog::reject();
}
//--------------------------------------------------------------------------------
void FindPetBox::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(btn))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void FindPetBox::block_widget(const QString name, bool state)
{
    if(name.isEmpty())
    {
        return;
    }
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName() == name)
        {
            if(obj->property(FLAG_NO_BLOCK).toBool() == false)
            {
                obj->setDisabled(state);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void FindPetBox::block_interface(bool state)
{
    QList<QAbstractButton *> all_obj = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *obj, all_obj)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QDoubleSpinBox *> all_dsb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_dsb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
void FindPetBox::closeEvent(QCloseEvent *event)
{
    if(block_close)
        event->ignore();
    else
        event->accept();
}
//--------------------------------------------------------------------------------
