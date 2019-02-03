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
#ifndef PET_DEMO_HPP
#define PET_DEMO_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QtNetwork>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "demo_device_bos.hpp"
#include "demo_device_brg.hpp"
#include "demo_device_sim.hpp"
#include "demo_device_td.hpp"
#include "demo_device_brg_sim.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_td.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_demo;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QPushButton;
class QToolButton;
class QToolBar;
class QComboBox;
class QSpinBox;
class QTreeWidgetItem;
//--------------------------------------------------------------------------------
class BOS_page_options;
class BRG_page_options;
class BRG_sim_page_options;
class TD_page_options;
//--------------------------------------------------------------------------------
class Database;
//--------------------------------------------------------------------------------
class UDP_packet_sender;
//--------------------------------------------------------------------------------
class QtVariantPropertyManager;
class QtVariantProperty;
//---
typedef struct device
{
    uint32_t    vid;
    uint32_t    pid;
    uint8_t     addr;
    QString     uid;
} device_t;
//--------------------------------------------------------------------------------
class PET_demo : public PET_device_base
{
    Q_OBJECT

public:
    explicit PET_demo(QString caption, QWidget *parent);
    ~PET_demo();

    void add_tw_device(Device_BOS *device);
    void add_tw_device(Device_BRG *device);
    void add_tw_device(Device_SIM *device);
    void add_tw_device(Device_TD *device);
    void add_tw_device(Device_BRG_SIM *device);

    int get_PID(void);

    void get_answer(QString *answer);

#ifdef QT_DEBUG
    void set_flag_test(bool state);
    bool f_connect(int port);
    void add_device_bos(int vid, int pid, int addr);
    void add_device_brg(int vid, int pid, int addr);
    void add_device_sim(int vid, int pid, int addr);
    void add_device_td(int vid, int pid, int addr);
    void add_device_brg_sim(int vid, int pid, int addr);
#endif

signals:
    void s_start(void);
    void s_stop(void);

    void block_widget(bool);

public slots:
    void add_device_bos(void);
    void add_device_brg(void);
    void add_device_sim(void);
    void add_device_td(void);
    void add_device_brg_sim(void);

    void f_connect(void);
    void f_disconnect(void);

    void clean(void);

private slots:
    int test(void);

    void del_device(void);

    void start_send_events(void);
    void stop_send_events(void);

    void processPendingDatagrams(void);
    void processPendingDatagrams_broadcast(void);

    void thread_is_finished(void);

    void popup(QPoint point);
    void f_del_device(void);

private:
    Ui::PET_demo *ui;

    // для удаления
    QString str_adr;
    QString str_vid;
    QString str_pid;
    QString str_uid;
    //---

    QComboBox *cb_test;

    QUdpSocket *udp_socket;
    QUdpSocket *udp_socket_broadcast;

    int last_error;
    QString last_error_text;

    QString server_name;

    QThread *thread;

    bool flag_test = false;

    UDP_packet_sender *packet_sender;

    QByteArray input_buffer;
    QByteArray output_buffer;

    QHostAddress    remote_host;
    quint16         remote_port;
    uint8_t         addr;

    QList<Device_base *> devices;

    void init(void);
    void init_widgets(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void init_server(void);
    void init_sender(void);

    void init_tw_devices(void);

    bool send_packet(QHostAddress host, quint16 port, QByteArray data);
    bool send_packet_broadcast(QHostAddress host, quint16 port, QByteArray data);

    QComboBox *create_cb_vid(int vid);
    QComboBox *create_cb_pid(int pid);

    pet_discovery_data pet_config;

    void block_wheel(void);

protected:
    bool eventFilter(QObject*, QEvent* event);
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif
