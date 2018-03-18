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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QMessageBox>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "ui_pet_demo.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "pet_demo.hpp"
//--------------------------------------------------------------------------------
#include "udp_packet_sender.hpp"
//--------------------------------------------------------------------------------
#include "demo_device_bos.hpp"
#include "demo_device_brg.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "gen_sinus_box.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
PET_demo::PET_demo(QString caption, QWidget *parent) :
    PET_device_base(parent),
    ui(new Ui::PET_demo),
    server_name(caption)
{
    init();

    load_QSpinBox("PET_demo");
}
//--------------------------------------------------------------------------------
PET_demo::~PET_demo()
{
    save_QSpinBox("PET_demo");

    if(packet_sender)
    {
        emit s_stop();
        packet_sender->s_stop();
        packet_sender->deleteLater();
    }

    if(udp_socket) udp_socket->deleteLater();
    delete ui;
}
//--------------------------------------------------------------------------------
int PET_demo::get_PID(void)
{
    return PET_PID_SIM;
}
//--------------------------------------------------------------------------------
void PET_demo::init(void)
{
    ui->setupUi(this);

    devices.clear();

    ui->btn_disconnect->setDisabled(true);

    ui->le_name_server->setReadOnly(true);
    ui->le_name_server->setText(server_name);

    last_error = 0;
    last_error_text.clear();

    init_server();
    init_widgets();

    block_wheel();

    //---
    QStringList sl = PETCat_options::get_eth_addresses();
    foreach (QString addr, sl)
    {
        ui->cb_ethernet->addItem(addr);
    }
    //---
}
//--------------------------------------------------------------------------------
QToolButton *PET_demo::add_button(QToolBar *tool_bar,
                                  QToolButton *tool_button,
                                  QIcon icon,
                                  const QString &text,
                                  const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void PET_demo::init_server(void)
{
    udp_socket = new QUdpSocket(this);
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    udp_socket_broadcast = new QUdpSocket(this);
    connect(udp_socket_broadcast, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams_broadcast()));
}
//--------------------------------------------------------------------------------
void PET_demo::init_sender(void)
{
    if(thread)
    {
        emit error("Генерация активна");
        return;
    }

    emit debug("sender started");

    thread = new QThread();

    packet_sender = new UDP_packet_sender(udp_socket,
                                          remote_host,
                                          remote_port);
    connect(packet_sender,  SIGNAL(info(QString)),      this, SIGNAL(info(QString)));
    connect(packet_sender,  SIGNAL(debug(QString)),     this, SIGNAL(debug(QString)));
    connect(packet_sender,  SIGNAL(error(QString)),     this, SIGNAL(error(QString)));
    connect(packet_sender,  SIGNAL(tarce(QString)),     this, SIGNAL(trace(QString)));

    packet_sender->moveToThread(thread);

    connect(thread,         SIGNAL(started()),  packet_sender,  SLOT(process()));
    connect(packet_sender,  SIGNAL(finished()), thread,         SLOT(quit()));
    connect(packet_sender,  SIGNAL(finished()), packet_sender,  SLOT(deleteLater()));
    connect(thread,         SIGNAL(finished()), thread,         SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), this, SLOT(thread_is_finished()));

    connect(this,   SIGNAL(s_start()),  packet_sender,  SLOT(s_start()));
    connect(this,   SIGNAL(s_stop()),   packet_sender,  SLOT(s_stop()));

    thread->start();
}
//--------------------------------------------------------------------------------
void PET_demo::thread_is_finished(void)
{
    emit debug("thread_is_finished");
}
//--------------------------------------------------------------------------------
void PET_demo::init_widgets(void)
{
    ui->le_name_server->setMinimumWidth(200);

    ui->sb_cmd_port->setRange(MIN_UDP_PORT, MAX_UDP_PORT);
    ui->sb_cmd_port->setValue(PETCat_options::get_port_cmd());

    ui->sb_event_port->setRange(MIN_UDP_PORT, MAX_UDP_PORT);
    ui->sb_event_port->setValue(PETCat_options::get_port_event());

    //---
    ui->cb_bos_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    ui->cb_bos_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);

    ui->cb_brg_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    ui->cb_brg_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);

    ui->cb_sim_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    ui->cb_sim_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);

    ui->cb_td_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    ui->cb_td_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);

    ui->cb_brg_sim_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    ui->cb_brg_sim_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);
    //---

    //TODO
#if 0
    Gen_sinus_box *gs = new Gen_sinus_box(this);
    ui->gen_sinus_layout->addWidget(gs);
#endif

    //---
    ui->le_bos_pid->setReadOnly(true);
    ui->le_brg_pid->setReadOnly(true);
    ui->le_sim_pid->setReadOnly(true);
    ui->le_td_pid->setReadOnly(true);
    ui->le_brg_sim_pid->setReadOnly(true);

    ui->le_bos_pid->setMinimumWidth(ui->le_bos_pid->sizeHint().width());
    ui->le_brg_pid->setMinimumWidth(ui->le_brg_pid->sizeHint().width());
    ui->le_sim_pid->setMinimumWidth(ui->le_sim_pid->sizeHint().width());
    ui->le_td_pid->setMinimumWidth(ui->le_td_pid->sizeHint().width());
    ui->le_brg_sim_pid->setMinimumWidth(ui->le_brg_sim_pid->sizeHint().width());
    //---

    //---
    ui->le_bos_pid->setText(get_PET_PID_string(PET_PID_BOS));
    ui->le_brg_pid->setText(get_PET_PID_string(PET_PID_BRG));
    ui->le_sim_pid->setText(get_PET_PID_string(PET_PID_SIM));
    ui->le_td_pid->setText(get_PET_PID_string(PET_PID_TD));
    ui->le_brg_sim_pid->setText(get_PET_PID_string(PET_PID_BRG_SIM));
    //---

    //---
    ui->sb_addr_bos->setRange(0, 0xFF);
    ui->sb_addr_brg->setRange(1, 0xFF);
    ui->sb_addr_sim->setRange(0, 0xFF);
    ui->sb_addr_td->setRange(0, 0xFF);
    ui->sb_addr_brg_sim->setRange(1, 0xFF);
    //---

    //---
    ui->sb_brg_cnt->setRange(1, PET_ADDR_BRG_MAX);
    ui->sb_brg_sim_cnt->setRange(1, PET_ADDR_BRG_MAX);
    //---

    //---
    init_tw_devices();
    //---

    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(f_connect()));
    connect(ui->btn_disconnect, SIGNAL(clicked(bool)),  this,   SLOT(f_disconnect()));

    connect(ui->btn_add_device_bos,     SIGNAL(clicked(bool)),  this,   SLOT(add_device_bos()));
    connect(ui->btn_add_device_brg,     SIGNAL(clicked(bool)),  this,   SLOT(add_device_brg()));
    connect(ui->btn_add_device_sim,     SIGNAL(clicked(bool)),  this,   SLOT(add_device_sim()));
    connect(ui->btn_add_device_td,      SIGNAL(clicked(bool)),  this,   SLOT(add_device_td()));
    connect(ui->btn_add_device_brg_sim, SIGNAL(clicked(bool)),  this,   SLOT(add_device_brg_sim()));

    ui->tw_devices->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tw_devices, SIGNAL(customContextMenuRequested(QPoint)),        this, SLOT(popup(QPoint)));

    //connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    connect(ui->btn_clean,  SIGNAL(clicked(bool)),  this,    SLOT(clean()));
}
//--------------------------------------------------------------------------------
void PET_demo::popup(QPoint point)
{
    QTreeWidgetItem *item = ui->tw_devices->itemAt(point);
    if(item == nullptr)
    {
        return;
    }

    str_adr = item->text(0);
    str_vid = item->text(1);
    str_pid = item->text(2);
    str_uid = item->text(3);
    emit debug(str_adr);
    emit debug(str_vid);
    emit debug(str_pid);
    emit debug(str_uid);

    QMenu *popup_menu = new QMenu(this);

    QAction *del_file_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("Удаление устройства"),
                                           this);

    popup_menu->addAction(del_file_action);

    connect(del_file_action,  SIGNAL(triggered()), this, SLOT(f_del_device()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void PET_demo::f_del_device(void)
{
    int btn = MyWidget::messagebox_question("Удаление устройства", "Удалить?");
    if(btn == QMessageBox::Yes)
    {
        for(int n=0; n<devices.count(); n++)
        {
            if(QString("%1").arg(devices.at(n)->pet_address) == str_adr &&
                    QString("%1").arg(PET_device_base::get_PET_VID_string(devices.at(n)->get_vid())) == str_vid &&
                    QString("%1").arg(PET_device_base::get_PET_PID_string(devices.at(n)->get_pid())) == str_pid &&
                    QString("%1").arg(devices.at(n)->get_str_uid()) == str_uid)
            {
                emit debug("del device");
                devices.removeAt(n);

                ui->tw_devices->clear();
                foreach (Device_base *device, devices)
                {
                    Device_BOS      *bos = dynamic_cast<Device_BOS *>(device);
                    Device_BRG      *brg = dynamic_cast<Device_BRG *>(device);
                    Device_SIM      *sim = dynamic_cast<Device_SIM *>(device);
                    Device_TD       *td  = dynamic_cast<Device_TD *>(device);
                    Device_BRG_SIM  *brg_sim = dynamic_cast<Device_BRG_SIM *>(device);

                    if(bos)     add_tw_device(bos);
                    if(brg)     add_tw_device(brg);
                    if(sim)     add_tw_device(sim);
                    if(td)      add_tw_device(td);
                    if(brg_sim) add_tw_device(brg_sim);
                }
                return;
            }
        }
    }
}
//--------------------------------------------------------------------------------
int PET_demo::test(void)
{
#ifndef QT_DEBUG
    emit error("debug only");
    return -1;
#endif

    emit info("test");

    return 0;
}
//--------------------------------------------------------------------------------
void PET_demo::init_tw_devices(void)
{
    ui->tw_devices->clear();

    QStringList sl;
    sl << "PET адрес";
    sl << "Производитель";
    sl << "Тип";
    sl << "UID";

    ui->tw_devices->setHeaderLabels(sl);
}
//--------------------------------------------------------------------------------
QComboBox *PET_demo::create_cb_vid(int vid)
{
    QComboBox *cb_vid = new QComboBox;
    cb_vid->addItem(get_PET_VID_string(PET_VID_NIITFA),    PET_VID_NIITFA);
    cb_vid->addItem(get_PET_VID_string(PET_VID_RDCVECTOR), PET_VID_RDCVECTOR);
    for(int n=0; n<cb_vid->count(); n++)
    {
        int id = cb_vid->itemData(n, Qt::UserRole).toInt();
        if(id == vid)
        {
            cb_vid->setCurrentIndex(n);
        }
    }
    cb_vid->installEventFilter(this);
    return cb_vid;
}
//--------------------------------------------------------------------------------
QComboBox *PET_demo::create_cb_pid(int pid)
{
    QComboBox *cb_pid = new QComboBox;
    cb_pid->addItem(get_PET_PID_string(PET_PID_BOS),        PET_PID_BOS);
    cb_pid->addItem(get_PET_PID_string(PET_PID_BRG),        PET_PID_BRG);
    cb_pid->addItem(get_PET_PID_string(PET_PID_SIM),        PET_PID_SIM);
    cb_pid->addItem(get_PET_PID_string(PET_PID_TD),         PET_PID_TD);
    cb_pid->addItem(get_PET_PID_string(PET_PID_BRG_SIM),    PET_PID_BRG_SIM);
    for(int n=0; n<cb_pid->count(); n++)
    {
        int id = cb_pid->itemData(n, Qt::UserRole).toInt();
        if(id == pid)
        {
            cb_pid->setCurrentIndex(n);
        }
    }
    cb_pid->installEventFilter(this);
    return cb_pid;
}
//--------------------------------------------------------------------------------
void PET_demo::add_tw_device(Device_BOS *device)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, QString("%1").arg(device->get_pet_address()));
    item->setText(1, QString("%1").arg(get_PET_VID_string(device->get_vid())));
    item->setText(2, QString("%1").arg(get_PET_PID_string(device->get_pid())));
    item->setText(3, QString("%1").arg(device->get_str_uid()));

    ui->tw_devices->addTopLevelItem(item);
    ui->tw_devices->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void PET_demo::add_tw_device(Device_BRG *device)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, QString("%1").arg(device->get_pet_address()));
    item->setText(1, QString("%1").arg(get_PET_VID_string(device->get_vid())));
    item->setText(2, QString("%1").arg(get_PET_PID_string(device->get_pid())));
    item->setText(3, QString("%1").arg(device->get_str_uid()));

    ui->tw_devices->addTopLevelItem(item);
    ui->tw_devices->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void PET_demo::add_tw_device(Device_SIM *device)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, QString("%1").arg(device->get_pet_address()));
    item->setText(1, QString("%1").arg(get_PET_VID_string(device->get_vid())));
    item->setText(2, QString("%1").arg(get_PET_PID_string(device->get_pid())));
    item->setText(3, QString("%1").arg(device->get_str_uid()));

    ui->tw_devices->addTopLevelItem(item);
    ui->tw_devices->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void PET_demo::add_tw_device(Device_TD *device)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, QString("%1").arg(device->get_pet_address()));
    item->setText(1, QString("%1").arg(get_PET_VID_string(device->get_vid())));
    item->setText(2, QString("%1").arg(get_PET_PID_string(device->get_pid())));
    item->setText(3, QString("%1").arg(device->get_str_uid()));

    ui->tw_devices->addTopLevelItem(item);
    ui->tw_devices->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void PET_demo::add_tw_device(Device_BRG_SIM *device)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, QString("%1").arg(device->get_pet_address()));
    item->setText(1, QString("%1").arg(get_PET_VID_string(device->get_vid())));
    item->setText(2, QString("%1").arg(get_PET_PID_string(device->get_pid())));
    item->setText(3, QString("%1").arg(device->get_str_uid()));

    ui->tw_devices->addTopLevelItem(item);
    ui->tw_devices->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void PET_demo::add_device_bos(void)
{
    int pid = PET_PID_BOS;
    int vid = ui->cb_bos_vid->itemData(ui->cb_bos_vid->currentIndex(), Qt::UserRole).toInt();
    int addr = ui->sb_addr_bos->value();

    Device_BOS *bos = new Device_BOS(this);
    bos->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    bos->set_pet_address(addr);
    bos->set_net_mask(0xFFFFFF00);
    bos->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    bos->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    bos->set_cmd_port(ui->sb_cmd_port->value());
    bos->set_event_port(ui->sb_event_port->value());
    bos->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    bos->set_vid((PET_VID)vid);
    bos->set_pid((PET_PID)pid);
    bos->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(bos);

    devices.append(bos);
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::add_device_bos(int vid, int pid, int addr)
{
    Device_BOS *bos = new Device_BOS(this);
    bos->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    bos->set_pet_address(addr);
    bos->set_net_mask(0xFFFFFF00);
    bos->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    bos->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    bos->set_cmd_port(ui->sb_cmd_port->value());
    bos->set_event_port(ui->sb_event_port->value());
    bos->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    bos->set_vid((PET_VID)vid);
    bos->set_pid((PET_PID)pid);
    bos->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(bos);

    devices.append(bos);
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::add_device_brg(void)
{
    int pid = PET_PID_BRG;
    int vid = ui->cb_brg_vid->itemData(ui->cb_brg_vid->currentIndex(), Qt::UserRole).toInt();
    int addr = ui->sb_addr_brg->value();

    for(int n=0; n<ui->sb_brg_cnt->value(); n++)
    {
        Device_BRG *brg = new Device_BRG(this);
        brg->set_pet_address(addr);
        brg->set_vid((PET_VID)vid);
        brg->set_pid((PET_PID)pid);
        brg->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
        add_tw_device(brg);

        devices.append(brg);

        addr++;
    }
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::add_device_brg(int vid, int pid, int addr)
{
    Device_BRG *brg = new Device_BRG(this);
    brg->set_pet_address(addr);
    brg->set_vid((PET_VID)vid);
    brg->set_pid((PET_PID)pid);
    brg->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(brg);

    devices.append(brg);
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::add_device_sim(void)
{
    int pid = PET_PID_SIM;
    int vid = ui->cb_sim_vid->itemData(ui->cb_sim_vid->currentIndex(), Qt::UserRole).toInt();
    int addr = ui->sb_addr_sim->value();

    Device_SIM *sim = new Device_SIM(this);
    sim->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    sim->set_pet_address(addr);
    sim->set_net_mask(0xFFFFFF00);
    sim->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    sim->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    sim->set_cmd_port(ui->sb_cmd_port->value());
    sim->set_event_port(ui->sb_event_port->value());
    sim->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    sim->set_vid((PET_VID)vid);
    sim->set_pid((PET_PID)pid);
    sim->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(sim);

    devices.append(sim);
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::add_device_sim(int vid, int pid, int addr)
{
    Device_SIM *sim = new Device_SIM(this);
    sim->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    sim->set_pet_address(addr);
    sim->set_net_mask(0xFFFFFF00);
    sim->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    sim->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    sim->set_cmd_port(ui->sb_cmd_port->value());
    sim->set_event_port(ui->sb_event_port->value());
    sim->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    sim->set_vid((PET_VID)vid);
    sim->set_pid((PET_PID)pid);
    sim->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(sim);

    devices.append(sim);
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::add_device_td(void)
{
    int pid = PET_PID_TD;
    int vid = ui->cb_td_vid->itemData(ui->cb_td_vid->currentIndex(), Qt::UserRole).toInt();
    int addr = ui->sb_addr_td->value();

    Device_TD *td = new Device_TD(this);
    td->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    td->set_pet_address(addr);
    td->set_net_mask(0xFFFFFF00);
    td->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    td->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    td->set_cmd_port(ui->sb_cmd_port->value());
    td->set_event_port(ui->sb_event_port->value());
    td->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    td->set_vid((PET_VID)vid);
    td->set_pid((PET_PID)pid);
    td->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(td);

    devices.append(td);
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::add_device_td(int vid, int pid, int addr)
{
    Device_TD *td = new Device_TD(this);
    td->set_net_address(QHostAddress(ui->cb_ethernet->currentText()));
    td->set_pet_address(addr);
    td->set_net_mask(0xFFFFFF00);
    td->set_net_gateway(QHostAddress(ui->cb_ethernet->currentText()).toIPv4Address());
    td->set_event_address(QHostAddress(ui->cb_ethernet->currentText()));
    td->set_cmd_port(ui->sb_cmd_port->value());
    td->set_event_port(ui->sb_event_port->value());
    td->set_mac_address(0x01,0x02,0x03,0x04,0x05,0x06);
    td->set_vid((PET_VID)vid);
    td->set_pid((PET_PID)pid);
    td->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(td);

    devices.append(td);
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::add_device_brg_sim(void)
{
    int pid = PET_PID_BRG_SIM;
    int vid = ui->cb_brg_sim_vid->itemData(ui->cb_brg_sim_vid->currentIndex(), Qt::UserRole).toInt();
    int addr = ui->sb_addr_brg_sim->value();

    for(int n=0; n<ui->sb_brg_sim_cnt->value(); n++)
    {
        Device_BRG_SIM *brg_sim = new Device_BRG_SIM(this);
        brg_sim->set_pet_address(addr);
        brg_sim->set_vid((PET_VID)vid);
        brg_sim->set_pid((PET_PID)pid);
        brg_sim->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
        add_tw_device(brg_sim);

        devices.append(brg_sim);

        addr++;
    }
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::add_device_brg_sim(int vid, int pid, int addr)
{
    Device_BRG_SIM *brg_sim = new Device_BRG_SIM(this);
    brg_sim->set_pet_address(addr);
    brg_sim->set_vid((PET_VID)vid);
    brg_sim->set_pid((PET_PID)pid);
    brg_sim->set_uid(0x01,0x02,0x03,0x04,0x05,0x06);
    add_tw_device(brg_sim);

    devices.append(brg_sim);
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::del_device(void)
{
    QPushButton *btn = (QPushButton *)sender();
    if(btn)
    {
        emit error(QString("item [%1]").arg(btn->property("item").toInt()));
    }
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
void PET_demo::set_flag_test(bool state)
{
    flag_test = state;
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::start_send_events(void)
{
    remote_host = QHostAddress::LocalHost;
    remote_port = PET_NET_EVENT_PORT_DEFAULT;

    emit debug(QString("start_send_events %1:%2")
               .arg(remote_host.toString())
               .arg(remote_port));

    init_sender();

    if(flag_test == false)
    {
        emit s_start();
    }
}
//--------------------------------------------------------------------------------
void PET_demo::stop_send_events(void)
{
    if(flag_test == false)
    {
        emit s_stop();
    }
}
//--------------------------------------------------------------------------------
void PET_demo::f_connect(void)
{
    bool ok = false;

    quint16 port = ui->sb_cmd_port->value();

    // не факт, что будет работать с несколькими интерфейсами в винде
    // linux вполне работает
#ifdef Q_OS_WIN
    emit info(QString("Соединение %1").arg(port));
    ok = udp_socket->bind(port);
    if(!ok)
    {
        emit error(QString("Ошибка: %1").arg(udp_socket->errorString()));
        if(udp_socket->isValid())
        {
            udp_socket->abort();
        }
        if(udp_socket_broadcast->isValid())
        {
            udp_socket_broadcast->abort();
        }
        return;
    }
    emit info("Соединение: OK");
#endif

#ifdef Q_OS_LINUX
    //---
    // ethernet
    QHostAddress addr = QHostAddress(ui->cb_ethernet->currentText());
    emit info(QString("Соединение %1:%2")
              .arg(addr.toString())
              .arg(port));
    ok = udp_socket->bind(addr, port);
    if(!ok)
    {
        emit error(QString("Ошибка: %1").arg(udp_socket->errorString()));
        if(udp_socket->isValid())
        {
            udp_socket->abort();
        }
        if(udp_socket_broadcast->isValid())
        {
            udp_socket_broadcast->abort();
        }
        return;
    }
    emit info("Соединение: OK");
    //---

    //---
    // broadcast
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    emit info(QString("Соединение %1:%2")
              .arg(addr.toString())
              .arg(port));
    ok = udp_socket_broadcast->bind(addr, port);
    if(!ok)
    {
        emit error(QString("Ошибка: %1").arg(udp_socket_broadcast->errorString()));
        if(udp_socket->isValid())
        {
            udp_socket->abort();
        }
        if(udp_socket_broadcast->isValid())
        {
            udp_socket_broadcast->abort();
        }
        return;
    }
    //---
#endif

    ui->cb_ethernet->setDisabled(true);
    ui->sb_cmd_port->setDisabled(true);
    ui->sb_event_port->setDisabled(true);
    ui->btn_connect->setDisabled(true);
    ui->btn_disconnect->setEnabled(true);

    emit info("Соединение: OK");
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
bool PET_demo::f_connect(int port)
{
    bool ok = false;
    ok = udp_socket->bind(port);
    if(!ok)
    {
        emit error(QString("Ошибка: %1").arg(udp_socket->errorString()));
        return false;
    }
    emit info(QString("Соединение %1:%2")
              .arg(udp_socket->localAddress().toString())
              .arg(udp_socket->localPort()));
    emit info("Соединение: OK");
    return true;
}
#endif
//--------------------------------------------------------------------------------
void PET_demo::f_disconnect(void)
{
    if(udp_socket->isValid())
    {
        udp_socket->abort();
    }
    if(udp_socket_broadcast->isValid())
    {
        udp_socket_broadcast->abort();
    }

    ui->cb_ethernet->setEnabled(true);
    ui->sb_cmd_port->setEnabled(true);
    ui->sb_event_port->setEnabled(true);
    ui->btn_connect->setEnabled(true);
    ui->btn_disconnect->setDisabled(true);

    emit info("Разъединение: OK");
}
//--------------------------------------------------------------------------------
void PET_demo::clean(void)
{
    devices.clear();
    ui->tw_devices->clear();
}
//--------------------------------------------------------------------------------
void PET_demo::processPendingDatagrams(void)
{
    emit debug("processPendingDatagrams");
    while (udp_socket->hasPendingDatagrams())
    {
        input_buffer.resize(udp_socket->pendingDatagramSize());
        QHostAddress host;
        quint16 port = 0;
        qint64 bytes = udp_socket->readDatagram(input_buffer.data(), input_buffer.size(), &host, &port);
        if(bytes == input_buffer.size())
        {
            emit receive_net_log(QString("%1:%2 [%3]")
                                 .arg(host.toString())
                                 .arg(port)
                                 .arg(input_buffer.toHex().data()));
            QByteArray output_packet;
            PET_RESULT err = PET_ERR_NONE;
            foreach (Device_base *device, devices)
            {
                err = device->check(input_buffer, &output_packet);
                if(err == PET_SUCCESS)
                {
                    pet_frame *frame = (pet_frame *)input_buffer.data();
                    if(frame->cmd == PET_CMD_RUN)
                    {
                        start_send_events();
                    }
                    if(frame->cmd == PET_CMD_STOP)
                    {
                        stop_send_events();
                    }
                    send_packet(host, port, output_packet);
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
void PET_demo::processPendingDatagrams_broadcast(void)
{
    emit debug("processPendingDatagrams_broadcast");
    while (udp_socket_broadcast->hasPendingDatagrams())
    {
        input_buffer.resize(udp_socket_broadcast->pendingDatagramSize());
        QHostAddress host;
        quint16 port = 0;
        qint64 bytes = udp_socket_broadcast->readDatagram(input_buffer.data(), input_buffer.size(), &host, &port);
        if(bytes == input_buffer.size())
        {
            emit receive_net_log(QString("%1:%2 [%3]")
                                 .arg(host.toString())
                                 .arg(port)
                                 .arg(input_buffer.toHex().data()));
            QByteArray output_packet;
            PET_RESULT err = PET_ERR_NONE;
            foreach (Device_base *device, devices)
            {
                err = device->check(input_buffer, &output_packet);
                if(err == PET_SUCCESS)
                {
                    pet_frame *frame = (pet_frame *)input_buffer.data();
                    if(frame->cmd == PET_CMD_RUN)
                    {
                        start_send_events();
                    }
                    if(frame->cmd == PET_CMD_STOP)
                    {
                        stop_send_events();
                    }
                    send_packet_broadcast(host, port, output_packet);
                }
            }
        }
    }
}//--------------------------------------------------------------------------------
bool PET_demo::send_packet(QHostAddress host,
                           quint16 port,
                           QByteArray data)
{
    emit send_net_log(QString("%1:%2 [%3]")
                      .arg(host.toString())
                      .arg(port)
                      .arg(data.toHex().data()));

    qint64 bytes = udp_socket->writeDatagram(data, host, port);
    if(bytes != data.size())
    {
        last_error_text = QString("error write datagram: %1").arg(udp_socket->errorString());
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool PET_demo::send_packet_broadcast(QHostAddress host,
                                     quint16 port,
                                     QByteArray data)
{
    emit send_net_log(QString("%1:%2 [%3]")
                      .arg(host.toString())
                      .arg(port)
                      .arg(data.toHex().data()));

    qint64 bytes = udp_socket_broadcast->writeDatagram(data, host, port);
    if(bytes != data.size())
    {
        last_error_text = QString("error write datagram: %1").arg(udp_socket_broadcast->errorString());
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void PET_demo::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
void PET_demo::get_answer(QString *answer)
{
    if(udp_socket == nullptr)
    {
        return;
    }
    if(udp_socket->isOpen())
    {
        *answer = "Вы уверены, что хотите разорвать соединение и закрыть окно?";
    }
}
//--------------------------------------------------------------------------------
bool PET_demo::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void PET_demo::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
