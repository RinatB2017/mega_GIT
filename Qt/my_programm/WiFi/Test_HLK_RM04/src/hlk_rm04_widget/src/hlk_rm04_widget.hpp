/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef HLK_RM04_WIDGET_HPP
#define HLK_RM04_WIDGET_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class HLK_RM04_widget;
}
//--------------------------------------------------------------------------------
class HLK_RM04_widget : public MyWidget
{
    Q_OBJECT

signals:
    void send(QByteArray);

public:
    explicit HLK_RM04_widget(QWidget *parent = nullptr);
    ~HLK_RM04_widget();

private slots:
    void readChannelFinished(void);
    int split_data(QByteArray dirty_data);
    void read_data(QByteArray ba);
    void lock_iface(bool state);

private slots:
    void choice_command(void);
    void choice_serial_to(void);
    void send_text(void);

    void init_widgets(void);

private:
    enum {
        ID_test = 0,
        ID_info,
        ID_scan,
        ID_get_MAC,
        ID_get_netmode,
        ID_set_netmode,
        ID_get_wifi_conf,
        ID_set_wifi_conf,
        ID_channel,
        ID_get_net_IP,
        ID_set_net_IP,
        ID_get_net_DNS,
        ID_set_net_DNS,
        ID_dhcpd,
        ID_get_dhcpd_ip,
        ID_set_dhcpd_ip,
        ID_get_dhcpd_dns,
        ID_set_dhcpd_dns,
        ID_get_dhcpd_time,
        ID_set_dhcpd_time,
        ID_net_commit,
        ID_out_trans,
        ID_get_remote_IP,
        ID_set_remote_IP,
        ID_get_remote_port,
        ID_set_remote_port,
        ID_remote_pro,
        ID_timeout,
        ID_mode,
        ID_uart,
        ID_uartpacklen,
        ID_uartpacktimeout,
        ID_escape,
        ID_tcp_auto,
        ID_default,
        ID_reboot,
        ID_ver
    };
    enum {
        ID_serial_to_ethernet_dynamic_ip = 0,
        ID_serial_to_ethernet_static_ip,
        ID_serial_to_wifi_client,
        ID_serial_to_wifi_client_static,
        ID_serial_to_wifi_ap
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        void (HLK_RM04_widget::*func)(void);
    } CMD_t;
    QList<CMD> l_commands;
    QList<CMD> l_serial_to;

    Ui::HLK_RM04_widget *ui;
    QByteArray serial_data;
    QStringList sl_read_data;

    void init(void);
    void init_serial(void);

    void send_command(QString cmd);
    void send_cmd(QString cmd, QString name, int default_cnt = 2);

    QString get_ssid(void);
    QString get_password(void);
    QUrl get_ip(void);
    QUrl get_remote_ip(void);
    QUrl get_mask(void);
    QUrl get_gate(void);
    int get_remote_port(void);
    QString get_encrypt_type(void);

    void set_ssid(QString ssid);
    void set_password(QString password);
    void set_ip(QUrl ip);
    void set_remote_ip(QUrl remote_id);
    void set_mask(QUrl mask);
    void set_gate(QUrl gate);
    void set_remote_port(int port);
    void set_encrypt_type(QString encrypt_type);

    QUrl get_net_ip(void);
    QUrl get_net_mask(void);
    QUrl get_net_dns(void);
    QUrl get_net_gate(void);

    QUrl get_dhcpd_ip(void);
    QUrl get_dhcpd_mask(void);
    QUrl get_dhcpd_gate(void);
    QUrl get_dhcpd_dns(void);

    void wait_msec(int timeout_msec);

    void s_test(void);
    void s_info(void);
    void s_scan(void);
    void s_get_MAC(void);
        void s_get_netmode(void);
        void s_set_netmode(void);
        void s_get_wifi_conf(void);
        void s_set_wifi_conf(void);
    void s_channel(void);
        void s_get_net_IP(void);
        void s_set_net_IP(void);
        void s_get_net_DNS(void);
        void s_set_net_DNS(void);
    void s_dhcpd(void);
        void s_get_dhcpd_ip(void);
        void s_set_dhcpd_ip(void);
        void s_get_dhcpd_dns(void);
        void s_set_dhcpd_dns(void);
        void s_get_dhcpd_time(void);
        void s_set_dhcpd_time(void);
    void s_net_commit(void);
    void s_out_trans(void);
        void s_get_remote_IP(void);
        void s_set_remote_IP(void);
        void s_get_remote_port(void);
        void s_set_remote_port(void);
    void s_remote_pro(void);
    void s_timeout(void);
    void s_mode(void);
    void s_uart(void);
    void s_uartpacklen(void);
    void s_uartpacktimeout(void);
    void s_escape(void);
    void s_tcp_auto(void);
    void s_default(void);
    void s_reboot(void);
    void s_ver(void);

    void s_serial_to_ethernet_dynamic_ip(void);
    void s_serial_to_ethernet_static_ip(void);
    void s_serial_to_wifi_client(void);
    void s_serial_to_wifi_client_static(void);
    void s_serial_to_wifi_ap(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // HLK_RM04_WIDGET_HPP
