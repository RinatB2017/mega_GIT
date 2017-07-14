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
#include <QProgressBar>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
//--------------------------------------------------------------------------------
#include "update_firmware.hpp"
#include "ui_update_firmware.h"
//--------------------------------------------------------------------------------
#include "myprogressdialog.hpp"
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "pet_brg.h"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
#include "sbglass.h"
//--------------------------------------------------------------------------------
#define NAME_DEVICE         "Устройство"
#define NAME_ADDR           "Адрес"
#define NAME_VER_FIRMWARE   "Версия ПО"
#define NAME_DATE_FIRMWARE  "Дата ПО"
#define NAME_STATUS         "Статус"

#define COL_DEVICE          0
#define COL_ADDR            1
#define COL_VER_FIRMWARE    2
#define COL_DATE_FIRMWARE   3
#define COL_STATUS          4
//--------------------------------------------------------------------------------
#define SET_GLASS
//--------------------------------------------------------------------------------
UpdateFirmware::UpdateFirmware(QList<pet_discovery_data> pet_config,
                               QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::UpdateFirmware),
    p_pet_config(pet_config)
{
    init();
    load_widgets("UpdateFirmware");
}
//--------------------------------------------------------------------------------
UpdateFirmware::~UpdateFirmware()
{
    save_widgets("UpdateFirmware");

    delete ui;
}
//--------------------------------------------------------------------------------
void UpdateFirmware::init(void)
{
    ui->setupUi(this);

    //---
    comm = new PET_comm();
    connect(comm,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(comm,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(comm,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(comm,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));
    //---
    brg = new PET_device_brg(this);
    //---
    if(parentWidget())
    {
        connect(brg,   SIGNAL(send_net_log(QString)),      parentWidget(), SIGNAL(send_net_log(QString)));
        connect(brg,   SIGNAL(receive_net_log(QString)),   this,           SIGNAL(receive_net_log(QString)));
    }
    else
    {
        emit error("parentWidget not found");
    }
    //---
    ui->le_filename_brg->setReadOnly(true);
    //---
    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    PET_RESULT err = brg->set_comm_param(addr, PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }

    bool ok = brg->bind(QHostAddress::AnyIPv4);
    if(!ok)
    {
        emit error(brg->get_last_error());
    }
    //---
    init_tw_main();
    update_tree();
    //---
    connect(ui->btn_refresh,        SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    connect(ui->btn_update,         SIGNAL(clicked(bool)),  this,   SLOT(update()));
    connect(ui->btn_firmware_brg,   SIGNAL(clicked(bool)),  this,   SLOT(choice_firmware_brg()));
}
//--------------------------------------------------------------------------------
void UpdateFirmware::update_tree(void)
{
    /*
     * set_item_param(item_UDC, Qt::ItemIsUserCheckable, UDC, default_value);
     * item_limit_UDC_1000V = new QTreeWidgetItem(ui->treeWidget_limits, QTreeWidgetItem::UserType);
     */

    block_this_button(true);

    pet_device_info_t d_info;
    int d_info_len = sizeof(d_info);

    if(p_pet_config.count() != 1)
    {
        emit error("Ошибка выбора томографа");
        return;
    }

    //---
    MyProgressDialog *dlg = new MyProgressDialog("Поиск устройств", "Пожалуйста, ждите!", this);
    dlg->show();

    //---
    QHostAddress host = p_pet_config.at(0).host;
    //---
    PET_RESULT err = brg->set_comm_param(host,
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }

    QVariant max_net_timeout = 3000;
    PETCat_options::get_max_net_timeout(&max_net_timeout);
    //---
    p_pet_config.clear();
    PET_RESULT res = PET_device_base::discovery(this,
                                                comm,
                                                host,
                                                PETCat_options::get_port_cmd(),
                                                PET_ADDR_BROADCAST,
                                                max_net_timeout.toInt(),
                                                &p_pet_config);
    dlg->close();
    if(res != PET_SUCCESS)
    {
        QString temp_str = brg->get_PET_RESULT_string(res);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    //---
    emit debug(QString("Найдено %1 устройств").arg(p_pet_config.count()));

    //qSort(l_info.begin(), l_info.end(), comp);
    int n=0;
    uf_list.clear();
    foreach (pet_discovery_data_t d_packet, p_pet_config)
    {
        if(d_packet.pid == PET_PID_BRG)
        {
            QString ver_firmvare;
            QString date_firmvare;
            //---
            PET_device_brg *brg = new PET_device_brg(this);
            PET_RESULT res = brg->set_comm_param(p_pet_config.at(0).host,
                                                 p_pet_config.at(0).port);
            if(err != PET_SUCCESS)
            {
                emit error(PET_device_base::get_PET_RESULT_string(err));
            }
            brg->set_pet_addr(d_packet.addr);
            res = brg->get_device_info(&d_info, &d_info_len);
            if(res != PET_SUCCESS)
            {
                emit error(brg->get_PET_RESULT_string(res));
                ver_firmvare = "н/д";
                date_firmvare = "н/д";
            }
            else
            {
                uint16_t sw_version = d_info.sw_version;
                ver_firmvare = QString("%1.%2")
                        .arg(sw_version >> 8)
                        .arg(sw_version & 0xFF);
                date_firmvare = QString("%1")
                        .arg(PET_device_base::convert_timestamp_to_datatime_string(d_info.sw_timestamp));
            }

            //---
            QTreeWidgetItem *item = new QTreeWidgetItem(root, QTreeWidgetItem::Type);
            set_item_param(item, Qt::ItemIsUserCheckable, PET_device_base::get_PET_PID_string(d_packet.pid), Qt::Unchecked);
            //---'=
            uf_list.append(item);
            //---
            item->setTextAlignment(COL_DEVICE, Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextAlignment(COL_ADDR, Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextAlignment(COL_VER_FIRMWARE, Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextAlignment(COL_DATE_FIRMWARE, Qt::AlignHCenter | Qt::AlignVCenter);
            item->setTextAlignment(COL_STATUS, Qt::AlignHCenter | Qt::AlignVCenter);
            //---
            item->setText(COL_ADDR, QString("%1").arg(d_packet.addr));
            item->setText(COL_VER_FIRMWARE, ver_firmvare);
            item->setText(COL_DATE_FIRMWARE, date_firmvare);
            item->setText(COL_STATUS, "");
            //---
            ui->tw_main->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            //---
            n++;
        }
    }
    //---
    root->setExpanded(true);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
bool UpdateFirmware::comp(pet_discovery_data_t &e1, pet_discovery_data_t &e2)
{
    return e1.addr < e2.addr;
}
//--------------------------------------------------------------------------------
void UpdateFirmware::init_tw_main(void)
{
    ui->tw_main->clear();

    root = new QTreeWidgetItem(ui->tw_main, QTreeWidgetItem::Type);
    ui->tw_main->setHeaderLabels(QStringList() << "" << "" << "" << "" << "");

    set_item_param(root, Qt::ItemIsUserCheckable, NAME_DEVICE, Qt::Unchecked);
    root->setText(COL_ADDR, NAME_ADDR);
    root->setText(COL_VER_FIRMWARE, NAME_VER_FIRMWARE);
    root->setText(COL_DATE_FIRMWARE, NAME_DATE_FIRMWARE);
    root->setText(COL_STATUS, NAME_STATUS);

    root->setTextAlignment(COL_DEVICE, Qt::AlignHCenter | Qt::AlignVCenter);
    root->setTextAlignment(COL_ADDR, Qt::AlignHCenter | Qt::AlignVCenter);
    root->setTextAlignment(COL_VER_FIRMWARE, Qt::AlignHCenter | Qt::AlignVCenter);
    root->setTextAlignment(COL_DATE_FIRMWARE, Qt::AlignHCenter | Qt::AlignVCenter);
    root->setTextAlignment(COL_STATUS, Qt::AlignHCenter | Qt::AlignVCenter);

    ui->tw_main->setColumnCount(5);
    ui->tw_main->addTopLevelItem(root);

    connect(ui->tw_main, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(item_clicked(QTreeWidgetItem*,int)));

    uf_list.clear();
}
//--------------------------------------------------------------------------------
void UpdateFirmware::item_clicked(QTreeWidgetItem *item, int column)
{
    if(item->isDisabled())
    {
        return;
    }

    Qt::CheckState state = item->checkState(column) ? Qt::Checked : Qt::Unchecked;
    if(item->parent())
    {
        int cnt_is_checked = 0;
        for(int n=0; n<item->parent()->childCount(); n++)
        {
            if(item->parent()->child(n)->checkState(0) == Qt::Checked)
                cnt_is_checked++;
        }
        if(cnt_is_checked == 0)
        {
            item->parent()->setCheckState(0, Qt::Unchecked);
            return;
        }
        if(cnt_is_checked == item->parent()->childCount())
        {
            item->parent()->setCheckState(0, Qt::Checked);
        }
        else
        {
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        }
    }
    else
    {
        for(int n=0; n<item->childCount(); n++)
        {
            item->child(n)->setCheckState(0, state);
        }
    }
}
//--------------------------------------------------------------------------------
void UpdateFirmware::refresh(void)
{
    FindPetBox *f = new FindPetBox(PET_ADDR_ROOT, this);

    int btn = f->exec();
    if(btn != QDialog::Accepted)
    {
        return;
    }
    p_pet_config.clear();
    p_pet_config = f->get_current_pet_config();

    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));
    //---
    QHostAddress addr = p_pet_config.at(0).host;
    quint32 t_addr = QHostAddress(addr).toIPv4Address();
    t_addr |= 0x000000FF;
    addr = QHostAddress(t_addr);
    //---
    PET_RESULT err = brg->set_comm_param(addr,
                                         PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }

    init_tw_main();
    update_tree();

    f->deleteLater();
}
//--------------------------------------------------------------------------------
void UpdateFirmware::update_items(void)
{
    //FIXME потом переделать
    foreach (QTreeWidgetItem *item, uf_list)
    {
        QString device = item->text(COL_DEVICE);
        QString pet_address = item->text(COL_ADDR);
        foreach (BRG_DEVICE *dev, brg_list)
        {
            //emit debug(dev->device);
            if((device == dev->device) && (pet_address.toInt() == dev->pet_address))
            {
                item->setText(COL_STATUS,   dev->state_str);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool UpdateFirmware::consistent_work(void)
{
    is_working = true;
    emit debug("consistent_work");

    int max_offset = ba_firmware_brg.length();
    int size_data = PET_UPDATE_DATA_LENGTH_MAX - sizeof(pet_update_data);

    foreach (BRG_DEVICE *dev, brg_list)
    {
        if(dev)
        {
            if(dev->state == STATE_BEGIN)
            {
                int cnt = 0;
                PET_RESULT res = PET_SUCCESS;
                while(res == PET_SUCCESS)
                {
                    if(((int)dev->offset + size_data) >= max_offset)
                    {
                        break;
                    }
                    res = brg->set_comm_param(p_pet_config.at(0).host,
                                              p_pet_config.at(0).port);
                    if(res != PET_SUCCESS)
                    {
                        emit error(PET_device_base::get_PET_RESULT_string(res));
                    }
                    brg->set_pet_addr(dev->pet_address);
                    res = update_device(dev->pet_address,
                                        cnt,
                                        dev->offset,
                                        ba_firmware_brg.mid(dev->offset, size_data));
                    cnt++;
                    dev->offset += size_data;
                    dev->state_str = QString("%1%").arg((int)(dev->offset * 100.0 / max_offset + 0.5));
                    update_items();
                }
                if(res != PET_SUCCESS)
                {
                    dev->state = STATE_ERROR;
                    dev->state_str = "Ошибка";
                    update_items();
                    emit error(QString("%1").arg(PET_device_base::get_PET_RESULT_string(res)));
                }
                else
                {
                    dev->state = STATE_END;
                }
            }
        }
    }

    is_working = false;
    return true;
}
//--------------------------------------------------------------------------------
bool UpdateFirmware::parallel_work(void)
{
    //FIXME надо переделать
    is_working = true;
    emit debug("parallel_work");

    int max_offset = ba_firmware_brg.length();
    int size_data = PET_UPDATE_DATA_LENGTH_MAX - sizeof(pet_update_data);
    int cnt_working = brg_list.count();

    int cnt = 0;
    PET_RESULT res = PET_SUCCESS;
    while(cnt_working)
    {
        foreach (BRG_DEVICE *dev, brg_list)
        {
            if(dev->state == STATE_BEGIN)
            {
                res = brg->set_comm_param(p_pet_config.at(0).host,
                                          p_pet_config.at(0).port);
                if(res != PET_SUCCESS)
                {
                    emit error(PET_device_base::get_PET_RESULT_string(res));
                }
                brg->set_pet_addr(dev->pet_address);
                res = update_device(dev->pet_address,
                                    cnt,
                                    dev->offset,
                                    ba_firmware_brg.mid(dev->offset, size_data));
                if(res != PET_SUCCESS)
                {
                    dev->state = STATE_ERROR;
                    dev->state_str = "Ошибка";
                    update_items();
                    emit error(QString("%1").arg(PET_device_base::get_PET_RESULT_string(res)));
                    cnt_working--;
                }
                else
                {
                    dev->offset += size_data;
                    int percent = (int)(dev->offset * 100.0 / max_offset + 0.5);
                    if(percent >= 100)
                    {
                        dev->state = STATE_END;
                        cnt_working--;
                    }
                    dev->state_str = QString("%1%").arg(percent);
                    update_items();
                }
            }
        }
        cnt++;
    }

    is_working = false;
    return true;
}
//--------------------------------------------------------------------------------
void UpdateFirmware::update(void)
{
    //---
    if(ui->le_filename_brg->text().isEmpty())
    {
        QString temp_str = "Файл ПО не выбран";
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }

    foreach (QTreeWidgetItem *item, uf_list)
    {
        if(item->checkState(0) == Qt::Checked)
        {
            emit info(QString("%1 [%2]").arg(item->text(0)).arg(item->text(1)));
        }
    }
    //---
    bool brg_is_exist = false;
    QString temp_str;

    foreach (QTreeWidgetItem *item, uf_list)
    {
        if(item->checkState(0) == Qt::Checked)
        {
            brg_is_exist = true;
        }
    }

    if(brg_is_exist == false)
    {
        temp_str = "Ничего не выбрано!";
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        return;
    }

    int btn = 0;
    if(ui->rb_parallel->isChecked())
    {
        btn = MyWidget::messagebox_question("Внимание!", "Случайное прерывание данной прощедуры может привести к нерабоспособности томографа.\nВы уверены, что хотите продолжить?");
        if(btn != MyWidget::Yes)
        {
            return;
        }
    }

    btn = MyWidget::messagebox_question("Внимание!", "Проверьте, что все необходимые кабели подключены, ИБП исправен.\nВы уверены, что хотите продолжить?");
    if(btn != MyWidget::Yes)
    {
        return;
    }
    //---
    brg_list.clear();
    foreach (QTreeWidgetItem *item, uf_list)
    {
        if(item)
        {
            if(item->checkState(0) == Qt::Checked)
            {
                QString str_device = item->text(COL_DEVICE);
                QString str_pet_address = item->text(COL_ADDR);
                QString str_sw_version = item->text(COL_VER_FIRMWARE);
                QString str_sw_timestamp = item->text(COL_DATE_FIRMWARE);

                BRG_DEVICE *device = new BRG_DEVICE;
                device->device = str_device;
                device->pet_address = str_pet_address.toInt();
                device->offset = 0;
                device->sw_version = str_sw_version.toInt();
                device->sw_timestamp = str_sw_timestamp.toInt();
                device->state = STATE_BEGIN;

                brg_list.append(device);
            }
        }
    }
    //---
#ifdef SET_GLASS
    SBGlass *glass = new SBGlass;
    glass->install(this);
    glass->enableOpacity();
    glass->enableColor();

    QMovie * movie = new QMovie(":/SBGlass/loading.gif");
    glass->enableInfoBlock(movie, "Производится калибровка, ждите...");
#else
    block_interface(true);
#endif

    if(ui->rb_consistent->isChecked())
    {
        consistent_work();
    }
    else
    {
        parallel_work();
    }

#ifdef SET_GLASS
    if(glass)
    {
        glass->remove();
        glass->deleteLater();
    }
#else
    block_interface(false);
#endif
    //---
    //TODO
#if 1
    messagebox_info("Информация", "Не забудьте перезапустить устройства!");
#else
    foreach (QTreeWidgetItem *item, uf_list)
    {
        QString ver_firmvare;
        QString date_firmvare;
        //---
        pet_device_info_t d_info;
        int d_info_len = sizeof(d_info);

        PET_device_brg *brg = new PET_device_brg(this);
        res = brg->set_comm_param(p_pet_config.at(0).host,
                                  p_pet_config.at(0).port);
        if(err != PET_SUCCESS)
        {
            emit error(PET_device_base::get_PET_RESULT_string(err));
        }
        brg->set_pet_addr(item->text(COL_ADDR).toInt());
        PET_RESULT res = brg->get_device_info(&d_info, &d_info_len);
        if(res != PET_SUCCESS)
        {
            emit error(brg->get_PET_RESULT_string(res));
            ver_firmvare = "н/д";
            date_firmvare = "н/д";
        }
        else
        {
            uint16_t sw_version = d_info.sw_version;
            ver_firmvare = QString("%1.%2")
                    .arg(sw_version >> 8)
                    .arg(sw_version & 0xFF);
            date_firmvare = QString("%1")
                    .arg(PET_device_base::convert_timestamp_to_datatime_string(d_info.sw_timestamp));
        }
        item->setText(COL_VER_FIRMWARE, ver_firmvare);
        item->setText(COL_DATE_FIRMWARE, date_firmvare);
    }
#endif
    //---
    root->setDisabled(false);
    for(int n=0; n<root->childCount(); n++)
    {
        QTreeWidgetItem *item = root->child(n);
        QWidget *w = 0;
        if(item)
        {
            w = ui->tw_main->itemWidget(item, COL_DEVICE);      if(w) w->setEnabled(true);
            w = ui->tw_main->itemWidget(item, COL_ADDR);        if(w) w->setEnabled(true);
            w = ui->tw_main->itemWidget(item, COL_VER_FIRMWARE);    if(w) w->setEnabled(true);
            w = ui->tw_main->itemWidget(item, COL_DATE_FIRMWARE);   if(w) w->setEnabled(true);
            w = ui->tw_main->itemWidget(item, COL_STATUS);      if(w) w->setEnabled(true);
        }
    }
    //---
}
//--------------------------------------------------------------------------------
PET_RESULT UpdateFirmware::update_device(uint8_t pet_address,
                                         uint32_t cnt,
                                         uint32_t offset,
                                         QByteArray data)
{
    if((pet_address < 1) || (pet_address > PET_BOS_CH_NUM))  return PET_ERR_BAD_ADDR;
    if(data.length() == 0)  return PET_ERR_BAD_FRAME;

    pet_update_data packet;
    uint32_t crc = 0;

    int s_data = sizeof(packet);
    int res = 0;
    int s_res = sizeof(res);

    packet.cnt = cnt;
    packet.offset = offset;
    packet.length = data.length();
    packet.crc = CRC::crc32c(crc, (const unsigned char *) &data.data()[0], data.length());

    brg->set_pet_addr(pet_address);
    return brg->update(&packet, &s_data,
                       &res,    &s_res);
}
//--------------------------------------------------------------------------------
void UpdateFirmware::choice_firmware_brg(void)
{
    MyFileDialog *dlg = 0;
    QString temp_str;

    dlg = new MyFileDialog("filedialog_states", "uf_dlg");
    dlg->setWindowIcon(QIcon(ICON_PROGRAMM));
    QStringList filters;
    filters << "Файлы ПО (*.pof)"
            << "Файлы ПО (*.sof)"
            << "Все файлы (*.*)";
    dlg->setNameFilters(filters);
    dlg->setDefaultSuffix(tr("pof"));
    dlg->setFileMode(MyFileDialog::ExistingFile);

    QString filename;
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }

    if(filename.isEmpty())
    {
        return;
    }

    //---
    QFile file(filename);
    if(file.exists() == false)
    {
        temp_str = QString("Файл %1 не существует!").arg(filename);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    QFileInfo fi(filename);
    emit info(QString("Размер файла ПО БРГ %1 ").arg(fi.size()));

    bool ok = file.open(QIODevice::ReadOnly);
    if(ok)
    {
        ba_firmware_brg.clear();
        ba_firmware_brg = file.readAll();
    }
    else
    {
        temp_str = QString("Файл %1 не чмтается!").arg(filename);
        messagebox_critical("Ошибка", temp_str);
        return;
    }
    //---

    int s_packet = PET_UPDATE_DATA_LENGTH_MAX - sizeof(pet_update_data);
    cycles_brg = (fi.size() / s_packet);
    if(fi.size() % s_packet)
    {
        cycles_brg++;
    }
    emit debug(QString("s_packet %1").arg(s_packet));
    emit info(QString("Количество циклов ПО БРГ %1 ").arg(cycles_brg));
    //---
    ui->le_filename_brg->setText(filename);
}
//--------------------------------------------------------------------------------
void UpdateFirmware::set_item_param(QTreeWidgetItem *item,
                                    Qt::ItemFlags flags,
                                    const QString &caption,
                                    Qt::CheckState state)
{
    item->setFlags(item->flags() | flags);  // Qt::ItemIsUserCheckable
    item->setText(0, caption);
    item->setCheckState(0, state);          // Qt::Checked
}
//--------------------------------------------------------------------------------
