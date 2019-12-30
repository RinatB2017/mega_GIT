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
#include "hlk_rm04_widget.hpp"
#include "ui_hlk_rm04_widget.h"
//--------------------------------------------------------------------------------
HLK_RM04_widget::HLK_RM04_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::HLK_RM04_widget)
{
    init();
}
//--------------------------------------------------------------------------------
HLK_RM04_widget::~HLK_RM04_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::init(void)
{
    ui->setupUi(this);

    connect(this,   SIGNAL(info(QString)),  ui->log_widget, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), ui->log_widget, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), ui->log_widget, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), ui->log_widget, SLOT(traceLog(QString)));

    connect(ui->btn_test,       &QPushButton::clicked,    this,   &HLK_RM04_widget::s_test);
    connect(ui->btn_info,       &QPushButton::clicked,    this,   &HLK_RM04_widget::s_info);
    connect(ui->btn_scan,       &QPushButton::clicked,    this,   &HLK_RM04_widget::s_scan);
    connect(ui->btn_get_MAC,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_get_MAC);
    connect(ui->btn_netmode,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_netmode);
    connect(ui->btn_wifi_conf,  &QPushButton::clicked,    this,   &HLK_RM04_widget::s_wifi_conf);
    connect(ui->btn_channel,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_channel);
    connect(ui->btn_net_IP,     &QPushButton::clicked,    this,   &HLK_RM04_widget::s_net_IP);
    connect(ui->btn_net_DNS,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_net_DNS);
    connect(ui->btn_dhcpd,      &QPushButton::clicked,    this,   &HLK_RM04_widget::s_dhcpd);
    connect(ui->btn_dhcpd_ip,   &QPushButton::clicked,    this,   &HLK_RM04_widget::s_dhcpd_ip);
    connect(ui->btn_dhcpd_dns,  &QPushButton::clicked,    this,   &HLK_RM04_widget::s_dhcpd_dns);
    connect(ui->btn_dhcpd_time, &QPushButton::clicked,    this,   &HLK_RM04_widget::s_dhcpd_time);
    connect(ui->btn_net_commit, &QPushButton::clicked,    this,   &HLK_RM04_widget::s_net_commit);
    connect(ui->btn_out_trans,  &QPushButton::clicked,    this,   &HLK_RM04_widget::s_out_trans);
    connect(ui->btn_remote_IP,  &QPushButton::clicked,    this,   &HLK_RM04_widget::s_remote_IP);
    connect(ui->btn_remote_port,&QPushButton::clicked,    this,   &HLK_RM04_widget::s_remote_port);
    connect(ui->btn_remote_pro, &QPushButton::clicked,    this,   &HLK_RM04_widget::s_remote_pro);
    connect(ui->btn_timeout,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_timeout);
    connect(ui->btn_mode,       &QPushButton::clicked,    this,   &HLK_RM04_widget::s_mode);
    connect(ui->btn_uart,       &QPushButton::clicked,    this,   &HLK_RM04_widget::s_uart);
    connect(ui->btn_uartpacklen,&QPushButton::clicked,    this,   &HLK_RM04_widget::s_uartpacklen);
    connect(ui->btn_uartpacktimeout,    &QPushButton::clicked,    this,   &HLK_RM04_widget::s_uartpacktimeout);
    connect(ui->btn_escape,     &QPushButton::clicked,    this,   &HLK_RM04_widget::s_escape);
    connect(ui->btn_tcp_auto,   &QPushButton::clicked,    this,   &HLK_RM04_widget::s_tcp_auto);
    connect(ui->btn_ver,        &QPushButton::clicked,    this,   &HLK_RM04_widget::s_ver);

    ui->cb_encrypt_type->addItem("none");
    ui->cb_encrypt_type->addItem("wep_open");
    ui->cb_encrypt_type->addItem("wep");
    ui->cb_encrypt_type->addItem("wpa_tkip");
    ui->cb_encrypt_type->addItem("wpa_aes");
    ui->cb_encrypt_type->addItem("wpa2_tkip");
    ui->cb_encrypt_type->addItem("wpa2_aes");
    ui->cb_encrypt_type->addItem("wpa/wpa2 tkip");
    ui->cb_encrypt_type->addItem("wpa/wpa2 aes");

    ui->sb_remote_post->setRange(0, 0xFFFF);

    init_serial();
    lock_iface(true);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::init_serial(void)
{
    ui->serial_widget->set_fix_baudrate(115200);

    connect(this,               SIGNAL(send(QByteArray)),       ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),     this,               SLOT(read_data(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(port_is_active(bool)),   this,               SLOT(lock_iface(bool)));
    connect(ui->serial_widget,  SIGNAL(readChannelFinished()),  this,               SLOT(readChannelFinished()));
}
//--------------------------------------------------------------------------------
int HLK_RM04_widget::split_data(QByteArray dirty_data)
{
    QString temp = dirty_data.data();

    sl_read_data = temp.split(QRegExp("[\r\n]+"), QString::SkipEmptyParts);
    int cnt = sl_read_data.count();
    if(cnt > 0)
    {
        emit trace("---");
        emit trace(QString("cnt %1").arg(cnt));
        foreach(QString text, sl_read_data)
        {
            emit trace(QString("text [%1]").arg(text));
        }
        emit trace("---");
    }
    return cnt;
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::read_data(QByteArray ba)
{
    emit trace(Q_FUNC_INFO);
    if(ba.isEmpty())
    {
        return;
    }

    //emit debug(ba);
    serial_data.append(ba);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::lock_iface(bool state)
{
    state ? lock_interface() : unlock_interface();
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::readChannelFinished(void)
{
    emit error("readChannelFinished");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::send_command(QString cmd)
{
    QByteArray ba;
    ba.clear();
    ba.append(cmd);
    ba.append('\r');

    emit send(ba);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::wait_msec(int timeout_msec)
{
    QElapsedTimer time;

    if(timeout_msec < 1)
    {
        return;
    }

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    emit info(QString("SSID: %1").arg(get_ssid()));
    emit info(QString("Password: %1").arg(get_password()));
    emit info(QString("IP: %1").arg(get_ip().host()));
    emit info(QString("Remote IP: %1").arg(get_remote_id().host()));
    emit info(QString("Mask: %1").arg(get_mask().host()));
    emit info(QString("Gate: %1").arg(get_gate().host()));
    emit info(QString("Port: %1").arg(get_remote_port()));
    emit info(QString("Encrypt type: %1").arg(get_encrypt_type()));

#else
    if(ui->serial_widget->isOpen() == false)
    {
        emit error("Port not open!");
        return;
    }

    serial_data.clear();
    send_command("at");

    wait_msec(1000);
    int cnt = split_data(serial_data);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return;
    }
    if(cnt != 1)
    {
        emit error("Bad data cnt");
        return;
    }

    emit info(QString("%1").arg(sl_read_data.at(0)));
#endif
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_info(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->serial_widget->isOpen() == false)
    {
        emit error("Port not open!");
        return;
    }

    QString temp;
    temp.append("at+netmode=?\r\n");
    temp.append("at+wifi_conf=?\r");
    temp.append("at+dhcpd=?\r");
    temp.append("at+dhcpd_ip=?\r");
    temp.append("at+dhcpd_dns=?\r");
    temp.append("at+dhcpd_time=?\r");
    temp.append("at+dhcpc=?\r");
    temp.append("at+net_ip=?\r");
    temp.append("at+net_dns=?\r");
    temp.append("at+net_wanip=?\r");
    temp.append("at+remoteip=?\r");
    temp.append("at+remoteport=?\r");
    temp.append("at+remotepro=?\r");
    temp.append("at+timeout=?\r");
    temp.append("at+mode=?\r");
    temp.append("at+uart=?\r");
    temp.append("at+uartpacklen=?\r");
    temp.append("at+uartpacktimeout=?\r");
    temp.append("at+ver=?\r");

    serial_data.clear();
    send_command(temp);

    wait_msec(1000);
    int cnt = split_data(serial_data);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return;
    }
    if(cnt != 38)
    {
        emit error(QString("Bad data cnt (%1)").arg(cnt));
        return;
    }

    //emit info(QString("Cnt: %1").arg(cnt));
    for(int n=1; n<cnt; n+=2)
    {
        emit info(QString("%1").arg(sl_read_data.at(n)));
    }
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_scan(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->serial_widget->isOpen() == false)
    {
        emit error("Port not open!");
        return;
    }

    serial_data.clear();
    send_command("at+wifi_Scan=?");

    wait_msec(10000);
    int cnt = split_data(serial_data);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return;
    }
    if(cnt < 2)
    {
        emit error("Bad data cnt");
        return;
    }

    emit info(QString("Cnt: %1").arg(cnt));
    for(int n=2; n<cnt; n++)
    {
        QStringList sl_scan_data = sl_read_data.at(n).split(QRegExp("[ ]+"), QString::SkipEmptyParts);
        //emit info(QString("SSID: %1").arg(sl_scan_data.at(1)));
        emit info(QString("BSSID: %1").arg(sl_scan_data.at(2)));
    }
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_MAC(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->serial_widget->isOpen() == false)
    {
        emit error("Port not open!");
        return;
    }

    serial_data.clear();
    send_command("at+Get_MAC=?");

    wait_msec(1000);
    int cnt = split_data(serial_data);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return;
    }
    if(cnt != 2)
    {
        emit error("Bad data cnt");
        return;
    }

    //emit info(QString("Cnt: %1").arg(cnt));
    emit info(QString("MAC: [%1]").arg(sl_read_data.at(1)));
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::send_cmd(QString cmd, QString name, int default_cnt)
{
    if(ui->serial_widget->isOpen() == false)
    {
        emit error("Port not open!");
        return;
    }
    if(default_cnt < 1)
    {
        emit error("bad default_cnt");
        return;
    }

    serial_data.clear();
    send_command(cmd);

    wait_msec(1000);
    int cnt = split_data(serial_data);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return;
    }
    if(cnt != default_cnt)
    {
        emit error("Bad data cnt");
        return;
    }

    emit info(QString("%1: [%2]")
              .arg(name)
              .arg(sl_read_data.at(default_cnt - 1)));
}
//--------------------------------------------------------------------------------
QString HLK_RM04_widget::get_ssid(void)
{
    return ui->le_ssid->text();
}
//--------------------------------------------------------------------------------
QString HLK_RM04_widget::get_password(void)
{
    return ui->le_password->text();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_ip(void)
{
    return ui->ip_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_remote_id(void)
{
    return ui->remove_ip_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_mask(void)
{
    return ui->mask_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_gate(void)
{
    return ui->gate_widget->get_url();
}
//--------------------------------------------------------------------------------
int HLK_RM04_widget::get_remote_port(void)
{
    return ui->sb_remote_post->value();
}
//--------------------------------------------------------------------------------
QString HLK_RM04_widget::get_encrypt_type(void)
{
    return ui->cb_encrypt_type->currentText();
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_ssid(QString ssid)
{
    ui->le_ssid->setText(ssid);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_password(QString password)
{
    ui->le_password->setText(password);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_ip(QUrl ip)
{
    ui->ip_widget->set_url(ip);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_remote_id(QUrl remote_id)
{
    ui->remove_ip_widget->set_url(remote_id);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_mask(QUrl mask)
{
    ui->mask_widget->set_url(mask);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_gate(QUrl gate)
{
    ui->mask_widget->set_url(gate);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_remote_port(int port)
{
    ui->sb_remote_post->setValue(port);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_encrypt_type(QString encrypt_type)
{
    ui->cb_encrypt_type->setCurrentText(encrypt_type);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::serial_to_ethernet_dynamic_ip(void)
{
    QString temp;
    temp.append("at+netmode=1\r");
    temp.append("at+dhcpc=1\r");
    temp.append("at+remoteip=192.168.11.245\r");
    temp.append("at+remoteport=8080\r");
    temp.append("at+remotepro=tcp\r");
    temp.append("at+timeout=0\r");
    temp.append("at+mode=server\r");
    temp.append("at+uart=115200,8,n,1\r");
    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::serial_to_ethernet_static_ip(void)
{
    QString temp;
    temp.append("at+netmode=1\r");
    temp.append("at+dhcpc=0\r");
    temp.append("at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1\r");
    temp.append("at+net_dns=192.168.11.1,8.8.8.8\r");
    temp.append("at+remoteip=192.168.11.245\r");
    temp.append("at+remoteport=8080\r");
    temp.append("at+remotepro=tcp\r");
    temp.append("at+timeout=0\r");
    temp.append("at+mode=server\r");
    temp.append("at+uart=115200,8,n,1\r");
    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::serial_to_wifi_client(void)
{
    QString temp;
    temp.append("at+netmode=2\r");
    temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append("at+dhcpc=1\r");
    temp.append("at+remoteip=192.168.11.245\r");
    temp.append("at+remoteport=8080\r");
    temp.append("at+remotepro=tcp\r");
    temp.append("at+timeout=0\r");
    temp.append("at+mode=server\r");
    temp.append("at+uart=115200,8,n,1\r");
    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::serial_to_wifi_client_static(void)
{
    QString temp;
    temp.append("at+netmode=2\r");
    temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append("at+dhcpc=0\r");
    temp.append("at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1\r");
    temp.append("at+net_dns=192.168.11.1,8.8.8.8\r");
    temp.append("at+remoteip=192.168.11.245\r");
    temp.append("at+remoteport=8080\r");
    temp.append("at+remotepro=tcp\r");
    temp.append("at+timeout=0\r");
    temp.append("at+mode=server\r");
    temp.append("at+uart=115200,8,n,1\r");
    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::serial_to_wifi_ap(void)
{
    QString temp;
    temp.append("at+netmode=3\r");
    temp.append("at+wifi_conf=Hi-Link_,wpa2_aes,0000000000\r");
    temp.append("at+dhcpd=1\r");
    temp.append("at+dhcpd_ip=192.168.16.100,192.168.16.200,255.255.255.0,192.168.16.254\r");
    temp.append("at+dhcpd_dns=192.168.16.254,8.8.8.8\r");
    temp.append("at+dhcpd_time=86400\r");
    temp.append("at+net_ip=192.168.16.254,255.255.255.0,192.168.16.254\r");
    temp.append("at+net_dns=192.168.16.254,8.8.8.8\r");
    temp.append("at+remoteip=192.168.11.245\r");
    temp.append("at+remoteport=8080\r");
    temp.append("at+remotepro=tcp\r");
    temp.append("at+timeout=0\r");
    temp.append("at+mode=server\r");
    temp.append("at+uart=115200,8,n,1\r");
    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_netmode(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+netmode=?", "netmode");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_wifi_conf(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+wifi_conf=?", "wifi conf");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_channel(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+Channel=?", "Channel");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_net_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+net_ip=?", "IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_net_DNS(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+net_dns=?", "DNS");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_dhcpd(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd=?", "DHCPD");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_dhcpd_ip(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_ip=?", "DHCPD IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_dhcpd_dns(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_dns=?", "DHCPD DNS");

}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_dhcpd_time(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_time=?", "DHCPD TIME");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_net_commit(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+Net_commit=?", "Net commit");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_out_trans(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+out_trans=?", "Out trans", 1);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_remote_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+remoteip=?", "Remote IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_remote_port(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+remoteport=?", "Remote port");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_remote_pro(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+remotepro=?", "Remote pro");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_timeout(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+timeout=?", "Timeout");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_mode(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+mode=?", "Mode");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_uart(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+uart=?", "Uart");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_uartpacklen(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+uartpacklen=?", "Uartpacklen");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_uartpacktimeout(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+uartpacktimeout=?", "Uartpacktimeout");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_escape(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+escape=?", "Escape");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_tcp_auto(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+tcp_auto=?", "Tcp auto");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_ver(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+ver=?", "Ver");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool HLK_RM04_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
