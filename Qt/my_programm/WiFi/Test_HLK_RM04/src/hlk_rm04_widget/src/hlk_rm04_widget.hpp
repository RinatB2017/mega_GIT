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

    void s_test(void);
    void s_info(void);
    void s_scan(void);
    void s_get_MAC(void);
    void s_netmode(void);
    void s_wifi_conf(void);
    void s_channel(void);
    void s_net_IP(void);
    void s_net_DNS(void);
    void s_dhcpd(void);
    void s_dhcpd_ip(void);
    void s_dhcpd_dns(void);
    void s_dhcpd_time(void);
    void s_net_commit(void);
    void s_out_trans(void);
    void s_remote_IP(void);
    void s_remote_port(void);
    void s_remote_pro(void);
    void s_timeout(void);
    void s_mode(void);
    void s_uart(void);
    void s_uartpacklen(void);
    void s_uartpacktimeout(void);
    void s_escape(void);
    void s_tcp_auto(void);
    void s_ver(void);

private:
    Ui::HLK_RM04_widget *ui;
    QByteArray serial_data;
    QStringList sl_read_data;

    void init(void);
    void init_serial(void);

    void send_command(QString cmd);
    void send_cmd(QString cmd, QString name, int default_cnt = 2);

    void serial_to_ethernet_dynamic_ip(void);
    void serial_to_ethernet_static_ip(void);
    void serial_to_wifi_client(void);
    void serial_to_wifi_client_static(void);
    void serial_to_wifi_ap(void);

    QString get_ssid(void);
    QString get_password(void);
    QUrl get_ip(void);
    QUrl get_remote_id(void);
    QUrl get_mask(void);
    QUrl get_gate(void);
    int get_remote_port(void);
    QString get_encrypt_type(void);

    void set_ssid(QString ssid);
    void set_password(QString password);
    void set_ip(QUrl ip);
    void set_remote_id(QUrl remote_id);
    void set_mask(QUrl mask);
    void set_gate(QUrl gate);
    void set_remote_port(int port);
    void set_encrypt_type(QString encrypt_type);

    void wait_msec(int timeout_msec);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // HLK_RM04_WIDGET_HPP
