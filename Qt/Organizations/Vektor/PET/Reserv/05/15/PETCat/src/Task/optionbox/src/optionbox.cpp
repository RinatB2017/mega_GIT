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
#include <QTreeWidgetItem>
#include <QProgressDialog>
#include <QStyleFactory>
#include <QProgressBar>
#include <QPushButton>
#include <QToolButton>
#include <QGridLayout>
#include <QSettings>
#include <QSplitter>
#include <QSpinBox>
#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "myprogressdialog.hpp"
#include "findpetbox.hpp"
#include "optionbox.hpp"
//--------------------------------------------------------------------------------
#include "ui_optionbox.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "pet_bos_main_page.hpp"
#include "pet_brg_main_page.hpp"
#include "pet_sim_main_page.hpp"
#include "pet_td_main_page.hpp"
#include "pet_brg_sim_main_page.hpp"
//--------------------------------------------------------------------------------
OptionBox::OptionBox(QList<pet_discovery_data> pet_config, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::OptionBox),
    p_pet_config(pet_config)
{
    init();
    load_widgets("OptionBox");
}
//--------------------------------------------------------------------------------
OptionBox::~OptionBox()
{
    save_widgets("OptionBox");

    //---
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    QByteArray ba = main_splitter->saveState();
    if(ba.isEmpty() ==  false)
    {
        settings->beginGroup(PETCAT_OPTIONS_MAIN_GROUP);
        settings->setValue("splitterState", ba);
        settings->endGroup();
    }
    settings->deleteLater();
    //---

    delete ui;
}
//--------------------------------------------------------------------------------
void OptionBox::init(void)
{
    ui->setupUi(this);

    //---
    bool ok = false;
    //---
    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port));
    comm = new PET_comm();
    connect(comm,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(comm,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(comm,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(comm,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    if(parentWidget())
    {
        connect(comm,   SIGNAL(send_net_log(QString)),      parentWidget(), SIGNAL(send_net_log(QString)));
        connect(comm,   SIGNAL(receive_net_log(QString)),   this,           SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("parentWidget not found");
    }
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    comm->set_comm_param(addr, PETCat_options::get_port_cmd());
    ok = comm->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(comm->get_last_error());
    }
    //---

    //---
    update_tree();
    //---
    main_splitter = new QSplitter(Qt::Horizontal);
    main_splitter->setChildrenCollapsible(false);

    ui->left_frame->setParent(main_splitter);
    ui->sw_main->setParent(main_splitter);
    main_splitter->addWidget(ui->left_frame);
    main_splitter->addWidget(ui->sw_main);
    ui->layout_main->addWidget(main_splitter);

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->beginGroup(PETCAT_OPTIONS_MAIN_GROUP);
    QByteArray ba = settings->value("splitterState").toByteArray();
    if(ba.isEmpty() == false)
    {
        main_splitter->restoreState(ba);
    }
    settings->endGroup();
    settings->deleteLater();
    //---
    connect(ui->tv_main, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(choice_device(QTreeWidgetItem*,int)));
    QModelIndex index = ui->tv_main->model()->index(0, 0);
    emit debug(QString("index = %1").arg(index.row()));

    if(index.row() >= 0)
    {
        ui->tv_main->setCurrentIndex(index);
        ui->tv_main->clicked(index);
    }

    //---
    layout()->addWidget(main_splitter);
    //---

    //connect(ui->btn_update_tree,    SIGNAL(clicked(bool)),  this,   SLOT(update_tree()));
}
//--------------------------------------------------------------------------------
void OptionBox::update_tree(void)
{
    block_this_button(true);

    //---
    MyProgressDialog *dlg = new MyProgressDialog("Настройки", "Пожалуйста, ждите!", this);
    dlg->show();

    //---
    emit debug("update_tree");
    //---
    QList<pet_discovery_data_t> l_info;
    //---
    QVariant max_net_timeout = 3000; //TODO
    PETCat_options::get_max_net_timeout(&max_net_timeout);
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    PET_RESULT res = PET_device_base::discovery(this,
                                                comm,
                                                addr,
                                                PETCat_options::get_port_cmd(),
                                                PET_ADDR_BROADCAST,
                                                max_net_timeout.toInt(),
                                                &l_info);
    if(res != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(res));
        dlg->close();
        block_this_button(false);
        return;
    }
    //---
    ui->tv_main->clear();
    QStringList sl;
    sl << "Устройство";
    sl << "Адрес";
    ui->tv_main->setHeaderLabels(sl);
    ui->tv_main->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tv_main->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    while(ui->sw_main->count())
    {
        ui->sw_main->removeWidget(ui->sw_main->currentWidget());
    }

    QTreeWidgetItem *root = 0;

    PET_bos_main_page       *bos_page = 0;
    PET_brg_main_page       *brg_page = 0;
    PET_sim_main_page       *sim_page = 0;
    PET_td_main_page        *td_page = 0;
    PET_brg_sim_main_page   *brg_sim_page = 0;

    //---
    foreach (pet_discovery_data_t d_packet, l_info)
    {
        if(d_packet.addr == PET_ADDR_ROOT)
        {
            int index = -1;
            switch(d_packet.pid)
            {
            case PET_PID_BOS:
                bos_page = new PET_bos_main_page(this);
                bos_page->set_pet_addr(d_packet.addr);
                bos_page->set_comm_param(p_pet_config.at(0).host,
                                         p_pet_config.at(0).port);
                //bos_page->all_refresh();
                index = ui->sw_main->addWidget(bos_page);
                bos_page->setProperty("index", index);
                ui->sw_main->setCurrentIndex(index);
                break;

            case PET_PID_SIM:
                sim_page = new PET_sim_main_page(this);
                sim_page->set_pet_addr(d_packet.addr);
                sim_page->set_comm_param(p_pet_config.at(0).host,
                                         p_pet_config.at(0).port);
                //sim_page->all_refresh();
                index = ui->sw_main->addWidget(sim_page);
                sim_page->setProperty("index", index);
                ui->sw_main->setCurrentIndex(index);
                break;

            case PET_PID_TD:
                td_page = new PET_td_main_page(this);
                td_page->set_pet_addr(d_packet.addr);
                td_page->set_comm_param(p_pet_config.at(0).host,
                                        p_pet_config.at(0).port);
                //td_page->all_refresh();
                index = ui->sw_main->addWidget(td_page);
                td_page->setProperty("index", index);
                ui->sw_main->setCurrentIndex(index);
                break;

            default:
                break;
            }

            if(index != -1)
            {
                root = new QTreeWidgetItem;
                root->setData(0, Qt::UserRole, index);
                root->setIcon(0, QIcon(NAME_RADIATION_ICON));
                root->setText(0, PET_device_base::get_device_type_short_str(d_packet.pid));
                root->setText(1, QString("%1").arg(d_packet.addr));

                ui->tv_main->addTopLevelItem(root);
                root->setExpanded(true);
            }
        }
    }
    //---
    foreach (pet_discovery_data_t d_packet, l_info)
    {
        if(d_packet.addr != PET_ADDR_ROOT)
        {
            int index = -1;
            switch(d_packet.pid)
            {
            case PET_PID_BRG:
                brg_page = new PET_brg_main_page(this);
                brg_page->set_pet_addr(d_packet.addr);
                brg_page->create_pages(d_packet.addr);
                brg_page->set_comm_param(p_pet_config.at(0).host,
                                         p_pet_config.at(0).port);
                //brg_page->all_refresh();
                index = ui->sw_main->addWidget(brg_page);
                brg_page->setProperty("index", index);
                ui->sw_main->setCurrentIndex(index);
                break;

            case PET_PID_BRG_SIM:
                brg_sim_page = new PET_brg_sim_main_page(this);
                brg_sim_page->set_pet_addr(d_packet.addr);
                brg_sim_page->set_comm_param(p_pet_config.at(0).host,
                                             p_pet_config.at(0).port);
                //brg_sim_page->all_refresh();
                index = ui->sw_main->addWidget(brg_sim_page);
                brg_sim_page->setProperty("index", index);
                ui->sw_main->setCurrentIndex(index);
                break;

            default:
                break;
            }

            if(index != -1)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setData(0, Qt::UserRole, index);
                item->setIcon(0, QIcon(NAME_RADIATION_ICON));
                item->setText(0, PET_device_base::get_device_type_short_str(d_packet.pid));
                item->setText(1, QString("%1").arg(d_packet.addr));

                root->addChild(item);
            }
        }
    }
    //---
    ui->tv_main->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tv_main->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->left_frame->setMaximumWidth(ui->left_frame->sizeHint().width());
    //---
    dlg->close();
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void OptionBox::choice_device(QTreeWidgetItem *item, int)
{
    Q_CHECK_PTR(item);
    if(!item)
    {
        emit error("choise_brg: item is NULL");
        return;
    }

    QVariant value = item->data(0, Qt::UserRole);
    ui->sw_main->setCurrentIndex(value.toInt());

    //TODO
    //---
    PET_bos_main_page       *bos = dynamic_cast<PET_bos_main_page *>(ui->sw_main->currentWidget());
    PET_brg_main_page       *brg = dynamic_cast<PET_brg_main_page *>(ui->sw_main->currentWidget());
    PET_sim_main_page       *sim = dynamic_cast<PET_sim_main_page *>(ui->sw_main->currentWidget());
    PET_td_main_page        *td  = dynamic_cast<PET_td_main_page *>(ui->sw_main->currentWidget());
    PET_brg_sim_main_page   *brg_sim = dynamic_cast<PET_brg_sim_main_page *>(ui->sw_main->currentWidget());

    if(bos)     bos->main_refresh();
    if(brg)     brg->main_refresh();
    if(sim)     sim->main_refresh();
    if(td)      td->main_refresh();
    if(brg_sim) brg_sim->main_refresh();
    //---
}
//--------------------------------------------------------------------------------
