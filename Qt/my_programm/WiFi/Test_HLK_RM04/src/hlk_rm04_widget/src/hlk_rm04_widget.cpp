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
#include "defines.hpp"
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

    l_commands.clear();
    l_commands.append({ ID_test,            "test",             &HLK_RM04_widget::s_test });
    l_commands.append({ ID_info,            "info",             &HLK_RM04_widget::s_info });
    l_commands.append({ ID_scan,            "scan",             &HLK_RM04_widget::s_scan });
    l_commands.append({ ID_get_MAC,         "GET MAC",          &HLK_RM04_widget::s_get_MAC });
    l_commands.append({ ID_get_netmode,     "GET netmode",      &HLK_RM04_widget::s_get_netmode });
    l_commands.append({ ID_set_netmode,     "SET netmode",      &HLK_RM04_widget::s_set_netmode });
    l_commands.append({ ID_set_wifi_conf,   "SET wifi_conf",    &HLK_RM04_widget::s_set_wifi_conf });
    l_commands.append({ ID_get_wifi_conf,   "GET wifi_conf",    &HLK_RM04_widget::s_get_wifi_conf });
    l_commands.append({ ID_channel,         "channel",          &HLK_RM04_widget::s_channel });
    l_commands.append({ ID_get_net_IP,      "GET net IP",       &HLK_RM04_widget::s_get_net_IP });
    l_commands.append({ ID_set_net_IP,      "SET net IP",       &HLK_RM04_widget::s_set_net_IP });
    l_commands.append({ ID_get_net_DNS,     "GET net_DNS",      &HLK_RM04_widget::s_get_net_DNS });
    l_commands.append({ ID_set_net_DNS,     "SET net_DNS",      &HLK_RM04_widget::s_set_net_DNS });
    l_commands.append({ ID_dhcpd,           "dhcpd",            &HLK_RM04_widget::s_dhcpd });
    l_commands.append({ ID_get_dhcpd_ip,    "GET dhcp IP",      &HLK_RM04_widget::s_get_dhcpd_ip });
    l_commands.append({ ID_set_dhcpd_ip,    "SET dhcp IP",      &HLK_RM04_widget::s_set_dhcpd_ip });
    l_commands.append({ ID_get_dhcpd_dns,   "GET dhcpd DNS",    &HLK_RM04_widget::s_get_dhcpd_dns });
    l_commands.append({ ID_set_dhcpd_dns,   "SET dhcpd DNS",    &HLK_RM04_widget::s_set_dhcpd_dns });
    l_commands.append({ ID_get_dhcpd_time,  "GET dhcpd TIME",   &HLK_RM04_widget::s_get_dhcpd_time });
    l_commands.append({ ID_set_dhcpd_time,  "SET dhcpd TIME",   &HLK_RM04_widget::s_set_dhcpd_time });
    l_commands.append({ ID_net_commit,      "net commit",       &HLK_RM04_widget::s_net_commit });
    l_commands.append({ ID_out_trans,       "out trans",        &HLK_RM04_widget::s_out_trans });
    l_commands.append({ ID_get_remote_IP,   "GET remote IP",    &HLK_RM04_widget::s_get_remote_IP });
    l_commands.append({ ID_set_remote_IP,   "SET remote IP",    &HLK_RM04_widget::s_set_remote_IP });
    l_commands.append({ ID_get_remote_port, "GET remote PORT",  &HLK_RM04_widget::s_get_remote_port });
    l_commands.append({ ID_set_remote_port, "SET remote PORT",  &HLK_RM04_widget::s_set_remote_port });
    l_commands.append({ ID_remote_pro,      "remote PRO",       &HLK_RM04_widget::s_remote_pro });
    l_commands.append({ ID_timeout,         "timeout",          &HLK_RM04_widget::s_timeout });
    l_commands.append({ ID_mode,            "mode",             &HLK_RM04_widget::s_mode });
    l_commands.append({ ID_uart,            "uart",             &HLK_RM04_widget::s_uart });
    l_commands.append({ ID_uartpacklen,     "uartpacketlen",    &HLK_RM04_widget::s_uartpacklen });
    l_commands.append({ ID_uartpacktimeout, "uartpacktimeout",  &HLK_RM04_widget::s_uartpacktimeout });
    l_commands.append({ ID_escape,          "escape",           &HLK_RM04_widget::s_escape });
    l_commands.append({ ID_tcp_auto,        "tcp_auto",         &HLK_RM04_widget::s_tcp_auto });
    l_commands.append({ ID_default,         "default",          &HLK_RM04_widget::s_default });
    l_commands.append({ ID_reboot,          "reboot",           &HLK_RM04_widget::s_reboot });
    l_commands.append({ ID_ver,             "version",          &HLK_RM04_widget::s_ver });

    l_serial_to.clear();
    l_serial_to.append({ ID_serial_to_ethernet_dynamic_ip,    "serial_to_ethernet_dynamic_ip",    &HLK_RM04_widget::s_serial_to_ethernet_dynamic_ip });
    l_serial_to.append({ ID_serial_to_ethernet_static_ip,     "serial_to_ethernet_static_ip",     &HLK_RM04_widget::s_serial_to_ethernet_static_ip  });
    l_serial_to.append({ ID_serial_to_wifi_client,            "serial_to_wifi_client",            &HLK_RM04_widget::s_serial_to_wifi_client });
    l_serial_to.append({ ID_serial_to_wifi_client_static,     "serial_to_wifi_client_static",     &HLK_RM04_widget::s_serial_to_wifi_client_static });
    l_serial_to.append({ ID_serial_to_wifi_ap,                "serial_to_wifi_ap",                &HLK_RM04_widget::s_serial_to_wifi_ap });

    foreach (CMD command, l_commands)
    {
        ui->cb_function->addItem(command.cmd_text, QVariant(command.cmd));
    }
    foreach (CMD command, l_serial_to)
    {
        ui->cb_serial_to->addItem(command.cmd_text, QVariant(command.cmd));
    }

    connect(ui->btn_function,   &QToolButton::clicked,  this,   &HLK_RM04_widget::choice_command);
    connect(ui->btn_serial_to,  &QToolButton::clicked,  this,   &HLK_RM04_widget::choice_serial_to);
    connect(ui->btn_send,       &QToolButton::clicked,  this,   &HLK_RM04_widget::send_text);

    connect(ui->le_send,        &QLineEdit::returnPressed,  this,   &HLK_RM04_widget::send_text);

    ui->cb_encrypt_type->addItem("none");
    ui->cb_encrypt_type->addItem("wep_open");
    ui->cb_encrypt_type->addItem("wep");
    ui->cb_encrypt_type->addItem("wpa_tkip");
    ui->cb_encrypt_type->addItem("wpa_aes");
    ui->cb_encrypt_type->addItem("wpa2_tkip");
    ui->cb_encrypt_type->addItem("wpa2_aes");
    ui->cb_encrypt_type->addItem("wpawpa2_tkip");
    ui->cb_encrypt_type->addItem("wpawpa2_aes");

    ui->cb_remotepro->addItem("tcp");
    ui->cb_remotepro->addItem("udp");

    ui->cb_mode->addItem("server");
    ui->cb_mode->addItem("client");

    ui->cb_netmode->addItem("Default setup");
    ui->cb_netmode->addItem("Ethernet");
    ui->cb_netmode->addItem("Wifi client");
    ui->cb_netmode->addItem("Wifi ap");

    ui->sb_dhcpd_time_widget->setRange(0, INT_MAX);

    ui->sb_remote_port->setRange(0, 0xFFFF);

    ui->sb_timeout->setRange(0, 0xFFFF);

    ui->btn_function->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));
    ui->btn_serial_to->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

    init_serial();

    //TODO надо доделать
    ui->sb_timeout->installEventFilter(this);
    ui->sb_remote_port->installEventFilter(this);
    ui->sb_dhcpd_time_widget->installEventFilter(this);

    ui->cb_mode->installEventFilter(this);
    ui->cb_netmode->installEventFilter(this);
    ui->cb_function->installEventFilter(this);
    ui->cb_remotepro->installEventFilter(this);
    ui->cb_serial_to->installEventFilter(this);
    ui->cb_encrypt_type->installEventFilter(this);
    //---

    //---
    bool first_start = load_value("First_start").toBool();
    if(!first_start)
    {
        emit info("First_start");
        save_value("First_start", true);
        QTimer::singleShot(100, this, SLOT(init_widgets()));
    }
    //---

#if 0
    //init_w_lists();
    add_widget_to_w_lists(ui->le_ssid);
    add_widget_to_w_lists(ui->le_password);
    add_widget_to_w_lists(ui->sb_remote_port);
    add_widget_to_w_lists(ui->cb_function);
    add_widget_to_w_lists(ui->cb_serial_to);
    add_widget_to_w_lists(ui->cb_encrypt_type);
    add_widget_to_w_lists(ui->cb_netmode);
    add_widget_to_w_lists(ui->btn_function);
    add_widget_to_w_lists(ui->btn_serial_to);
    //lock_iface(false);
#endif
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::init_widgets(void)
{
    emit trace(Q_FUNC_INFO);
    ui->le_ssid->setText("Ssid");
    ui->le_password->setText("Password2");

    QUrl url;
    url.setHost("192.168.1.1");
    ui->ip_widget->set_url(url);
    ui->gate_widget->set_url(url);
    ui->net_ip_widget->set_url(url);
    ui->dhcpd_ip_widget->set_url(url);
    ui->dhcpd_dns_widget->set_url(url);
    ui->dhcpd_gate_widget->set_url(url);
    ui->net_dns_widget->set_url(url);

    url.setHost("192.168.1.10");
    ui->remote_ip_widget->set_url(url);

    ui->sb_remote_port->setValue(5000);

    ui->sb_timeout->setValue(3600);
    ui->sb_dhcpd_time_widget->setValue(3600);

    url.setHost("255.255.255.0");
    ui->mask_widget->set_url(url);
    ui->net_gate_widget->set_url(url);
    ui->net_mask_widget->set_url(url);
    ui->dhcpd_mask_widget->set_url(url);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::init_serial(void)
{
    ui->serial_widget->set_fix_baudrate(115200);

    connect(this,               SIGNAL(send(QByteArray)),       ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),     this,               SLOT(read_data(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(port_is_active(bool)),   this,               SLOT(unlock_iface(bool)));
    connect(ui->serial_widget,  SIGNAL(readChannelFinished()),  this,               SLOT(readChannelFinished()));
}
//--------------------------------------------------------------------------------
int HLK_RM04_widget::split_data(QByteArray dirty_data)
{
    QString temp = dirty_data.data();

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    sl_read_data = temp.split(QRegExp("[\r\n]+"), Qt::SkipEmptyParts);
#else
    sl_read_data = temp.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
#endif
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

    emit debug(ba);
    serial_data.append(ba);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::lock_iface(bool state)
{
    emit info(QString("state is %1").arg(state ? "true" : "false"));
    ui->ip_widget->block_interface(state);
    ui->gate_widget->block_interface(state);
    ui->mask_widget->block_interface(state);
    ui->remote_ip_widget->block_interface(state);
    ui->net_ip_widget->block_interface(state);
    ui->net_gate_widget->block_interface(state);
    ui->net_mask_widget->block_interface(state);
    ui->dhcpd_ip_widget->block_interface(state);
    ui->dhcpd_dns_widget->block_interface(state);
    ui->dhcpd_mask_widget->block_interface(state);
    ui->dhcpd_gate_widget->block_interface(state);
    ui->net_dns_widget->block_interface(state);

    ui->sb_timeout->setDisabled(state);
    ui->sb_remote_port->setDisabled(state);
    ui->sb_dhcpd_time_widget->setDisabled(state);

    ui->cb_mode->setDisabled(state);
    ui->cb_netmode->setDisabled(state);
    ui->cb_function->setDisabled(state);
    ui->cb_remotepro->setDisabled(state);
    ui->cb_serial_to->setDisabled(state);
    ui->cb_encrypt_type->setDisabled(state);

    ui->btn_send->setDisabled(state);
    ui->btn_function->setDisabled(state);
    ui->btn_serial_to->setDisabled(state);

    ui->le_send->setDisabled(state);
    ui->le_ssid->setDisabled(state);
    ui->le_password->setDisabled(state);

    state ? lock_interface() : unlock_interface();
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::unlock_iface(bool state)
{
    emit info(QString("state is %1").arg(!state ? "true" : "false"));
    ui->ip_widget->block_interface(!state);
    ui->gate_widget->block_interface(!state);
    ui->mask_widget->block_interface(!state);
    ui->remote_ip_widget->block_interface(!state);
    ui->net_ip_widget->block_interface(!state);
    ui->net_gate_widget->block_interface(!state);
    ui->net_mask_widget->block_interface(!state);
    ui->dhcpd_ip_widget->block_interface(!state);
    ui->dhcpd_dns_widget->block_interface(!state);
    ui->dhcpd_mask_widget->block_interface(!state);
    ui->dhcpd_gate_widget->block_interface(!state);
    ui->net_dns_widget->block_interface(!state);

    ui->sb_timeout->setEnabled(state);
    ui->sb_remote_port->setEnabled(state);
    ui->sb_dhcpd_time_widget->setEnabled(state);

    ui->cb_mode->setEnabled(state);
    ui->cb_netmode->setEnabled(state);
    ui->cb_function->setEnabled(state);
    ui->cb_remotepro->setEnabled(state);
    ui->cb_serial_to->setEnabled(state);
    ui->cb_encrypt_type->setEnabled(state);

    ui->btn_send->setEnabled(state);
    ui->btn_function->setEnabled(state);
    ui->btn_serial_to->setEnabled(state);

    ui->le_send->setEnabled(state);
    ui->le_ssid->setEnabled(state);
    ui->le_password->setEnabled(state);

    !state ? lock_interface() : unlock_interface();
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::readChannelFinished(void)
{
    emit error("readChannelFinished");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::send_command(QString cmd)
{
//    QByteArray ba;
//    ba.clear();
//    ba.append(cmd);
//    ba.append('\r');
//    emit send(ba);

    cmd.append('\r');
    emit send(cmd.toLatin1());
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
void HLK_RM04_widget::choice_command(void)
{
    bool ok = false;
    int cmd = ui->cb_function->itemData(ui->cb_function->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        l_commands.begin(),
        l_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != l_commands.end())
    {
        typedef void (HLK_RM04_widget::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::choice_serial_to(void)
{
    bool ok = false;
    int cmd = ui->cb_serial_to->itemData(ui->cb_serial_to->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        l_commands.begin(),
        l_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != l_commands.end())
    {
        typedef void (HLK_RM04_widget::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_test(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    emit info(QString("SSID: %1").arg(get_ssid()));
    emit info(QString("Password: %1").arg(get_password()));
    emit info(QString("IP: %1").arg(get_ip().host()));
    emit info(QString("Remote IP: %1").arg(get_remote_ip().host()));
    emit info(QString("Mask: %1").arg(get_mask().host()));
    emit info(QString("Gate: %1").arg(get_gate().host()));
    emit info(QString("Port: %1").arg(get_remote_port()));
    emit info(QString("Encrypt type: %1").arg(get_encrypt_type()));

    emit info(QString("Net IP: %1").arg(get_net_ip().host()));
    emit info(QString("Net mask: %1").arg(get_net_mask().host()));
    emit info(QString("Dhcpd IP: %1").arg(get_dhcpd_ip().host()));
    emit info(QString("Dhcpd DNS: %1").arg(get_dhcpd_dns().host()));
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QStringList sl_scan_data = sl_read_data.at(n).split(QRegExp("[ ]+"), Qt::SkipEmptyParts);
#else
        QStringList sl_scan_data = sl_read_data.at(n).split(QRegularExpression("[ ]+"), Qt::SkipEmptyParts);
#endif
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
void HLK_RM04_widget::send_cmd(const QString &cmd,
                               const QString &name,
                               int default_cnt)
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
QUrl HLK_RM04_widget::get_remote_ip(void)
{
    return ui->remote_ip_widget->get_url();
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
    return ui->sb_remote_port->value();
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
void HLK_RM04_widget::set_remote_ip(QUrl remote_id)
{
    ui->remote_ip_widget->set_url(remote_id);
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
    ui->sb_remote_port->setValue(port);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::set_encrypt_type(QString encrypt_type)
{
    ui->cb_encrypt_type->setCurrentText(encrypt_type);
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_net_ip(void)
{
    return ui->net_ip_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_net_mask(void)
{
    return ui->net_mask_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_dhcpd_ip(void)
{
    return ui->dhcpd_ip_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_dhcpd_mask(void)
{
    return ui->dhcpd_mask_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_dhcpd_gate(void)
{
    return ui->dhcpd_gate_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_dhcpd_dns(void)
{
    return ui->dhcpd_dns_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_net_dns(void)
{
    return ui->net_dns_widget->get_url();
}
//--------------------------------------------------------------------------------
QUrl HLK_RM04_widget::get_net_gate(void)
{
    return ui->net_gate_widget->get_url();
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_serial_to_ethernet_dynamic_ip(void)
{
    QString temp;
    temp.append("at+netmode=1\r");

    //temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append(QString("at+wifi_conf=%1,%2,%3\r")
                .arg(get_ssid())
                .arg(get_encrypt_type())
                .arg(get_password()));

    temp.append("at+dhcpc=1\r");

    //temp.append("at+remoteip=192.168.11.245\r");
    temp.append(QString("at+remoteip=%1\r").arg(get_remote_ip().host()));
    //temp.append("at+remoteport=8080\r");
    temp.append(QString("at+remoteport=%1\r").arg(get_remote_port()));

    //temp.append("at+remotepro=tcp\r");
    temp.append(QString("at+remotepro=%1\r").arg(ui->cb_remotepro->currentText()));
    //temp.append("at+timeout=0\r");
    temp.append(QString("at+timeout=%1\r").arg(ui->sb_timeout->value()));
    //temp.append("at+mode=server\r");
    temp.append(QString("at+mode=%1\r").arg(ui->cb_mode->currentText()));

    //temp.append("at+uart=115200,8,n,1\r");
    temp.append(QString("at+uart=%1,%2,%3,%4\r")
                .arg(ui->serial_widget->get_baudRate())
                .arg(ui->serial_widget->get_dataBits())
                .arg(ui->serial_widget->get_flowControl())
                .arg(ui->serial_widget->get_stopBits()));

    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");

    //TODO test
    temp.append("test\r");

    emit debug(temp);
    send_command(temp);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_serial_to_ethernet_static_ip(void)
{
    QString temp;
    temp.append("at+netmode=1\r");

    //temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append(QString("at+wifi_conf=%1,%2,%3\r")
                .arg(get_ssid())
                .arg(get_encrypt_type())
                .arg(get_password()));

    temp.append("at+dhcpc=0\r");

    //temp.append("at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1\r");
    temp.append(QString("at+net_ip=%1,%2,%3\r")
                .arg(get_net_ip().host())
                .arg(get_net_mask().host())
                .arg(get_net_gate().host()));

    //temp.append("at+net_dns=192.168.11.1,8.8.8.8\r");
    temp.append(QString("at+net_dns=%1\r").arg(get_net_dns().host()));

    //temp.append("at+remoteip=192.168.11.245\r");
    temp.append(QString("at+remoteip=%1\r").arg(get_remote_ip().host()));

    //temp.append("at+remoteport=8080\r");
    temp.append(QString("at+remoteport=%1\r").arg(get_remote_port()));

    //temp.append("at+remotepro=tcp\r");
    temp.append(QString("at+remotepro=%1\r").arg(ui->cb_remotepro->currentText()));
    //temp.append("at+timeout=0\r");
    temp.append(QString("at+timeout=%1\r").arg(ui->sb_timeout->value()));
    //temp.append("at+mode=server\r");
    temp.append(QString("at+mode=%1\r").arg(ui->cb_mode->currentText()));

    //temp.append("at+uart=115200,8,n,1\r");
    temp.append(QString("at+uart=%1,%2,%3,%4\r")
                .arg(ui->serial_widget->get_baudRate())
                .arg(ui->serial_widget->get_dataBits())
                .arg(ui->serial_widget->get_flowControl())
                .arg(ui->serial_widget->get_stopBits()));

    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");

    //TODO test
    temp.append("test\r");

    emit debug(temp);
    send_command(temp);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_serial_to_wifi_client(void)
{
    QString temp;
    temp.append("at+netmode=2\r");

    //temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append(QString("at+wifi_conf=%1,%2,%3\r")
                .arg(get_ssid())
                .arg(get_encrypt_type())
                .arg(get_password()));

    temp.append("at+dhcpc=1\r");

    //temp.append("at+remoteip=192.168.11.245\r");
    temp.append(QString("at+remoteip=%1\r").arg(get_remote_ip().host()));

    //temp.append("at+remoteport=8080\r");
    temp.append(QString("at+remoteport=%1\r").arg(get_remote_port()));

    //temp.append("at+remotepro=tcp\r");
    temp.append(QString("at+remotepro=%1\r").arg(ui->cb_remotepro->currentText()));
    //temp.append("at+timeout=0\r");
    temp.append(QString("at+timeout=%1\r").arg(ui->sb_timeout->value()));
    //temp.append("at+mode=server\r");
    temp.append(QString("at+mode=%1\r").arg(ui->cb_mode->currentText()));

    //temp.append("at+uart=115200,8,n,1\r");
    temp.append(QString("at+uart=%1,%2,%3,%4\r")
                .arg(ui->serial_widget->get_baudRate())
                .arg(ui->serial_widget->get_dataBits())
                .arg(ui->serial_widget->get_flowControl())
                .arg(ui->serial_widget->get_stopBits()));

    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");

    //TODO test
    temp.append("test\r");

    emit debug(temp);
    send_command(temp);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_serial_to_wifi_client_static(void)
{
    QString temp;
    temp.append("at+netmode=2\r");

    //temp.append("at+wifi_conf=HI-LINK,wpa2_aes,12345678\r");
    temp.append(QString("at+wifi_conf=%1,%2,%3\r")
                .arg(get_ssid())
                .arg(get_encrypt_type())
                .arg(get_password()));

    temp.append("at+dhcpc=0\r");

    //temp.append("at+net_ip=192.168.11.254,255.255.255.0,192.168.11.1\r");
    temp.append(QString("at+net_ip=%1,%2,%3\r")
                .arg(get_net_ip().host())
                .arg(get_net_mask().host())
                .arg(get_net_gate().host()));

    //temp.append("at+net_dns=192.168.11.1,8.8.8.8\r");
    temp.append(QString("at+net_dns=%1\r").arg(get_net_dns().host()));

    //temp.append("at+remoteip=192.168.11.245\r");
    temp.append(QString("at+remoteip=%1\r").arg(get_remote_ip().host()));

    //temp.append("at+remoteport=8080\r");
    temp.append(QString("at+remoteport=%1\r").arg(get_remote_port()));

    //temp.append("at+remotepro=tcp\r");
    temp.append(QString("at+remotepro=%1\r").arg(ui->cb_remotepro->currentText()));
    //temp.append("at+timeout=0\r");
    temp.append(QString("at+timeout=%1\r").arg(ui->sb_timeout->value()));
    //temp.append("at+mode=server\r");
    temp.append(QString("at+mode=%1\r").arg(ui->cb_mode->currentText()));

    //temp.append("at+uart=115200,8,n,1\r");
    temp.append(QString("at+uart=%1,%2,%3,%4\r")
                .arg(ui->serial_widget->get_baudRate())
                .arg(ui->serial_widget->get_dataBits())
                .arg(ui->serial_widget->get_flowControl())
                .arg(ui->serial_widget->get_stopBits()));

    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");

    //TODO test
    temp.append("test\r");

    emit debug(temp);
    send_command(temp);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_serial_to_wifi_ap(void)
{
    QString temp;
    temp.append("at+netmode=3\r");
    temp.append("at+wifi_conf=Hi-Link_,wpa2_aes,0000000000\r");
    temp.append("at+dhcpd=1\r");

    //temp.append("at+dhcpd_ip=192.168.16.100,192.168.16.200,255.255.255.0,192.168.16.254\r");
    temp.append(QString("at+dhcpd_ip=%1,%2,%3\r")
                .arg(get_dhcpd_ip().host())
                .arg(get_dhcpd_mask().host())
                .arg(get_dhcpd_gate().host()));

    //temp.append("at+dhcpd_dns=192.168.16.254,8.8.8.8\r");
    temp.append(QString("at+dhcpd_dns=%1\r")
                .arg(get_dhcpd_dns().host()));

    temp.append("at+dhcpd_time=86400\r");

    //temp.append("at+net_ip=192.168.16.254,255.255.255.0,192.168.16.254\r");
    temp.append(QString("at+net_ip=%1,%2,%3\r")
                .arg(get_net_ip().host())
                .arg(get_net_mask().host())
                .arg(get_net_gate().host()));

    //temp.append("at+net_dns=192.168.16.254,8.8.8.8\r");
    temp.append(QString("at+net_dns=%1\r").arg(get_net_dns().host()));

    //temp.append("at+remoteip=192.168.11.245\r");
    temp.append(QString("at+remoteip=%1\r").arg(get_remote_ip().host()));

    //temp.append("at+remoteport=8080\r");
    temp.append(QString("at+remoteport=%1\r").arg(get_remote_port()));

    //temp.append("at+remotepro=tcp\r");
    temp.append(QString("at+remotepro=%1\r").arg(ui->cb_remotepro->currentText()));
    //temp.append("at+timeout=0\r");
    temp.append(QString("at+timeout=%1\r").arg(ui->sb_timeout->value()));
    //temp.append("at+mode=server\r");
    temp.append(QString("at+mode=%1\r").arg(ui->cb_mode->currentText()));

    //temp.append("at+uart=115200,8,n,1\r");
    temp.append(QString("at+uart=%1,%2,%3,%4\r")
                .arg(ui->serial_widget->get_baudRate())
                .arg(ui->serial_widget->get_dataBits())
                .arg(ui->serial_widget->get_flowControl())
                .arg(ui->serial_widget->get_stopBits()));

    temp.append("at+uartpacklen=64\r");
    temp.append("at+uartpacktimeout=10\r");
    temp.append("at+net_commit=1\r");
    temp.append("at+reconn=1\r");

    emit debug(temp);
    send_command(temp);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_netmode(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+netmode=?", "get netmode");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_netmode(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+netmode=%1")
             .arg(ui->cb_netmode->currentIndex()),
             "set netmode");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_wifi_conf(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+wifi_conf=%1,%2,%3")
             .arg(ui->le_ssid->text())
             .arg(ui->cb_encrypt_type->currentText())
             .arg(ui->le_password->text()), "wifi conf");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_wifi_conf(void)
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
void HLK_RM04_widget::s_get_net_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+net_ip=?", "GET net ip");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_net_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+net_ip=%1,%2,%3\r")
             .arg(get_net_ip().host())
             .arg(get_net_mask().host())
             .arg(get_net_gate().host()),
             "SET net ip");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_net_DNS(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+net_dns=?", "get DNS");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_net_DNS(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+net_dns=%1")
             .arg(ui->net_dns_widget->get_url().host()),
             "set DNS");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_dhcpd(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd=?", "DHCPD");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_dhcpd_ip(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_ip=?", "DHCPD IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_dhcpd_ip(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+dhcpd_ip=%1")
             .arg(ui->dhcpd_ip_widget->get_url().host()),
             "DHCPD IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_dhcpd_dns(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_dns=?", "get DHCPD DNS");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_dhcpd_dns(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+dhcpd_dns=%1")
             .arg(ui->dhcpd_dns_widget->get_url().host()),
             "set DHCPD DNS");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_dhcpd_time(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+dhcpd_time=?", "get DHCPD TIME");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_dhcpd_time(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+dhcpd_time=%1")
             .arg(ui->sb_dhcpd_time_widget->value()),
             "set DHCPD TIME");
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
void HLK_RM04_widget::s_get_remote_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+remoteip=?", "Get remote IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_remote_IP(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+remoteip=%1")
             .arg(ui->remote_ip_widget->get_url().host()),
             "Set remote IP");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_get_remote_port(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+remoteport=?", "Get remote port");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_set_remote_port(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd(QString("at+remoteport=%1")
             .arg(ui->sb_remote_port->value()),
             "Set remote port");
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
void HLK_RM04_widget::s_default(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+default=1", "Default", 1);
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_reboot(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+reboot=1", "Reboot");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::s_ver(void)
{
    emit trace(Q_FUNC_INFO);
    send_cmd("at+ver=?", "Ver");
}
//--------------------------------------------------------------------------------
void HLK_RM04_widget::send_text(void)
{
    emit trace(Q_FUNC_INFO);
    send_command(ui->le_send->text());
}
//--------------------------------------------------------------------------------
bool HLK_RM04_widget::eventFilter(QObject*, QEvent* event)
{
    if(event->type() == QEvent::Wheel)
    {
#ifdef QT_DEBUG
        qDebug() << "Wheel event blocked";
#endif
        return true;
    }
    return false;
}//--------------------------------------------------------------------------------
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
