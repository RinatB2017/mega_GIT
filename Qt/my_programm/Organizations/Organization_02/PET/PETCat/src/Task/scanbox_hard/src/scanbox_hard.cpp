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
#include <QUdpSocket>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "scanbox_hard.hpp"
#include "ui_scanbox_hard.h"
#include "pet_defines.hpp"
#include "defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
ScanBox_hard::ScanBox_hard(pet_discovery_data pet_config,
                           QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ScanBox_hard),
    p_pet_config(pet_config)
{
    init();

    load_QDoubleSpinBox("ScanBox_hard");
    load_QSpinBox("ScanBox_hard");
    load_QComboBox("ScanBox_hard");
    //load_widgets("ScanBox_hard");    //TODO
}
//--------------------------------------------------------------------------------
ScanBox_hard::~ScanBox_hard()
{
    save_QDoubleSpinBox("ScanBox_hard");
    save_QSpinBox("ScanBox_hard");
    save_QComboBox("ScanBox_hard");
    //save_widgets("ScanBox_hard");    //TODO

    scan_socket->close();
    scan_socket->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void ScanBox_hard::init(void)
{
    ui->setupUi(this);

    //---
    cnt_second = 0;
    //---
    cnt_all_packet = 0;
    cnt_good_packet = 0;
    cnt_good_packet_last_sec = 0;
    cnt_packet_bad_cmd = 0;
    cnt_packet_bad_len = 0;

    block_received_packet = true;
    //---
    ui->sb_port->setRange(MIN_UDP_PORT, MAX_UDP_PORT);
    ui->sb_port->setValue(PETCat_options::get_port_event());
    //---
    ui->cb_host->addItem("Все");
    //---
    ui->btn_close->setDisabled(true);
    //---

    ui->pb_events->setRange(0, 100);

    ui->pb_events->setRange(0, 100);
    ui->pb_events->setValue(0);
    ui->pb_events->setTextVisible(false);

    ui->le_filename->setReadOnly(true);

    ui->le_time->setReadOnly(true);
    ui->le_status->setReadOnly(true);

    ui->btn_choose_file->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));

    connect(ui->btn_choose_file, SIGNAL(clicked(bool)), this, SLOT(choose_file()));
    connect(ui->btn_start_stop,  SIGNAL(clicked(bool)), this, SLOT(f_start_stop()));

    init_timer();
    init_udp_socket();

    ui->btn_start_stop->setDisabled(true);

    block_wheel();

    f_update();
}
//--------------------------------------------------------------------------------
void ScanBox_hard::init_timer(void)
{
    timer = new QTimer();
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(f_update()));
}
//--------------------------------------------------------------------------------
void ScanBox_hard::init_udp_socket(void)
{
    scan_socket = new QUdpSocket();
    connect(scan_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}
//--------------------------------------------------------------------------------
bool ScanBox_hard::bind(void)
{
    bool ok = false;
    scan_socket->abort();
    ok = scan_socket->bind(QHostAddress::AnyIPv4, ui->sb_port->value());
    if(!ok)
    {
        emit error(QString("Ошибка: %1").arg(scan_socket->errorString()));
        return false;
    }
    emit info(QString("Init UDP-port %1: OK").arg(ui->sb_port->value()));
    return true;
}
//--------------------------------------------------------------------------------
void ScanBox_hard::processPendingDatagrams(void)
{
    while(scan_socket->hasPendingDatagrams())
    {
        scan_datagram.resize(scan_socket->pendingDatagramSize());

        QHostAddress remote_host;
        quint16 remote_port = 0;
        qint64 bytes = scan_socket->readDatagram(scan_datagram.data(),
                                                 scan_datagram.size(),
                                                 &remote_host,
                                                 &remote_port);
        if(block_received_packet == false)
        {
            if(bytes == scan_datagram.size())
            {
                QHostAddress remote_host_ipv4 = QHostAddress(remote_host.toIPv4Address());
                get_packet(remote_host_ipv4, remote_port, scan_datagram);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void ScanBox_hard::get_packet(QHostAddress host, quint16 port, QByteArray packet)
{
#if 0
    if(p_pet_config.host != host)
    {
        return;
    }
    if(p_pet_config.port != port)
    {
        return;
    }
#else
    Q_UNUSED(host)
    Q_UNUSED(port)
#endif

    if(packet.length() <= (int)sizeof(pet_frame_t))
    {
        return;
    }

    pet_frame_t *frame = (pet_frame_t *)packet.data();
    Q_CHECK_PTR(frame);
    if(frame->cmd != PET_CMD_EVENT_STREAM)
    {
        cnt_packet_bad_cmd += (frame->len / sizeof(pet_event_t));
        return;
    }
    cnt_all_packet += (frame->len / sizeof(pet_event_t));
    if(frame->len % sizeof(pet_event_t))
    {
        cnt_packet_bad_len += (frame->len / sizeof(pet_event_t));
        return;
    }

    cnt_good_packet += (frame->len / sizeof(pet_event_t));
    if(file)
    {
        quint64 bytes = file->write((char *)frame->data, frame->len);
        if(bytes != frame->len)
        {
            emit error("Ошибка записи в файл");
        }
    }
}
//--------------------------------------------------------------------------------
void ScanBox_hard::choose_file(void)
{
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "sb_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setConfirmOverwrite(true);
    QStringList filters;
    filters << "Файлы данных (*.dat3)"
            << "Все файлы (*.*)";
    dlg->setNameFilters(filters);
    dlg->selectFile("без имени");
    dlg->setDefaultSuffix(tr("dat3"));
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    else
    {
        dlg->deleteLater();
        return;
    }
    dlg->deleteLater();

    if(filename.isEmpty())
    {
        return;
    }

    ui->le_filename->setText(filename);
    ui->btn_start_stop->setEnabled(true);

    //---
    cnt_all_packet = 0;
    cnt_good_packet = 0;
    cnt_good_packet_last_sec = 0;
    cnt_packet_bad_cmd = 0;
    cnt_packet_bad_len = 0;
    f_update();
    cnt_second = 0;
}
//--------------------------------------------------------------------------------
void ScanBox_hard::f_start_stop(void)
{
    if(ui->btn_start_stop->text() == "Пуск")
    {
        //---
        cnt_second = 0;
        //---
        cnt_all_packet = 0;
        cnt_good_packet = 0;
        cnt_good_packet_last_sec = 0;
        cnt_packet_bad_cmd = 0;
        cnt_packet_bad_len = 0;
        //---
        file = new QFile(ui->le_filename->text());
        if(!file)
        {
            messagebox_critical("Ошибка", QString("Ошибка создания файла %1").arg(ui->le_filename->text()));
            ui->btn_start_stop->setText("Пуск");
            return;
        }
        bool ok = false;
        if(file->exists())
        {
            QString str;
            str.append(QString("Файл %1 уже существует\n").arg(ui->le_filename->text()));
            str.append("Перезаписать?\n");

            int btn = messagebox_question("Ошибка", str);

            if(btn != MyWidget::Yes)
            {
                return;
            }
        }
        ok = file->open(QFile::ReadWrite | QFile::Truncate);
        if(!ok)
        {
            ui->btn_start_stop->setText("Пуск");
            messagebox_critical("Ошибка", QString("Ошибка создания файла %1").arg(ui->le_filename->text()));
            return;
        }
        //---
        ui->btn_start_stop->setText("Стоп");
        f_timer_start();
    }
    else
    {
        ui->btn_start_stop->setText("Старт");
        f_timer_stop();
    }
}
//--------------------------------------------------------------------------------
void ScanBox_hard::f_timer_start(void)
{
    bool ok = bind();
    if(!ok)
    {
        ui->btn_start_stop->setText("Старт");
        return;
    }

    ui->btn_start_stop->setText("Стоп");
    ui->pb_events->setRange(0, 0);

    ui->btn_choose_file->setDisabled(true);
    ui->sb_port->setDisabled(true);
    block_received_packet = false;
    timer->start(1000);
}
//--------------------------------------------------------------------------------
void ScanBox_hard::f_timer_stop(void)
{
    ui->btn_start_stop->setText("Пуск");
    ui->pb_events->setRange(0, 100);

    ui->btn_choose_file->setDisabled(false);
    ui->sb_port->setDisabled(false);
    block_received_packet = true;
    timer->stop();

    file->close();
}
//--------------------------------------------------------------------------------
QString ScanBox_hard::convert_int_to_string(quint64 value)
{
    if(value >= 1e9)
    {
        return QString("%1 млрд.").arg((double)value / (double)1e9, 0, 'f', 3);
    }
    if(value >= 1e6)
    {
        return QString("%1 млн.").arg((double)value / (double)1e6, 0, 'f', 3);
    }
    if(value >= 1e3)
    {
        return QString("%1 тыс.").arg((double)value / (double)1e3, 0, 'f', 3);
    }
    return QString("%1").arg(value);
}
//--------------------------------------------------------------------------------
void ScanBox_hard::f_update(void)
{
    //    quint64 cnt_all_packet;
    //    quint64 cnt_good_packet;
    //    quint64 cnt_good_packet_last_sec;
    //    quint64 cnt_packet_bad_cmd;
    //    quint64 cnt_packet_bad_len;

    //Х млн событий (Н тыс соб/сек, ошибок; A/B)

    QTime t(0, 0, 0);
    QTime n = t.addSecs(cnt_second);
    ui->le_time->setText(n.toString("hh:mm:ss"));
    cnt_second++;

    quint64 delta = cnt_good_packet - cnt_good_packet_last_sec;
    cnt_good_packet_last_sec = cnt_good_packet;
    QString res = QString("%1 событий (%2 соб./сек, %3/%4 ошибок)")
            .arg(convert_int_to_string(cnt_all_packet))
            .arg(convert_int_to_string(delta))
            .arg(cnt_packet_bad_cmd)
            .arg(cnt_packet_bad_len);

    ui->le_status->setText(res);
}
//--------------------------------------------------------------------------------
void ScanBox_hard::get_answer(QString *answer)
{
    if(timer->isActive())
    {
        *answer = "Вы уверены, что хотите завершить сбор данных и закрыть окно?";
    }
}
//--------------------------------------------------------------------------------
