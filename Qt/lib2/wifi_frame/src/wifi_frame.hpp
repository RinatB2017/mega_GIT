/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef WIFI_FRAME_HPP
#define WIFI_FRAME_HPP
//--------------------------------------------------------------------------------
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class WIFI_frame;
}
//--------------------------------------------------------------------------------
class WIFI_frame : public MyWidget
{
    Q_OBJECT

public:
    explicit WIFI_frame(QWidget *parent);

    explicit WIFI_frame(const QString &caption,
                        bool is_server,
                        QWidget *parent);

    bool send_at_command(QString cmd,
                         int wait_ms = 200,
                         bool no_response = false);

public slots:
    void update_ports(void);

private slots:
    void create_server(void);
    void create_client(void);
    void send_data(void);

    void read_settings(void);

    void server_port_read(void);
    void client_port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

    void readChannelFinished(void);

    void lock_interface(bool state);
    void unlock_interface(bool state);  //FIXME костыль

private:
    Ui::WIFI_frame *ui;

    QString caption;
    bool is_server = false;
    bool server_is_created = false;
    QByteArray  serial_data;
    bool is_ready = false;

    void init(void);
    void connect_serial(void);
    bool send_command(QString cmd,
                      int wait_ms = 100);

    bool send_cmd_create_server(bool is_silense = false);
    bool send_cmd_create_client(bool is_silense = false);

    void wait_msec(int timeout_msec);
    void add_server_cmd_layout(void);
    void add_client_cmd_layout(void);
    void add_grid_layout(void);

    QString get_server_string(void);
    QString get_client_string(void);

    void show_hex_data(QByteArray &data);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // WIFI_FRAME_HPP
