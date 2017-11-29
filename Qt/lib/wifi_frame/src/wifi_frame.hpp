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
#include <QFrame>
//--------------------------------------------------------------------------------
class QPushButton;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;
class QComboBox;
class QTextEdit;
class QLineEdit;
class QLabel;
class LogBox;
//--------------------------------------------------------------------------------
class WIFI_frame : public QFrame
{
    Q_OBJECT
public:
    explicit WIFI_frame(const QString &caption,
                        bool is_server,
                        QWidget *parent);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void update_ports(void);

private slots:
    void log(const QString &data);

    void serial_open(void);
    void serial_close(void);

    void create_server(void);
    void create_client(void);
    void send_data(void);

    void read_settings(void);

    void server_port_read(void);
    void client_port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

    void readChannelFinished(void);

    void updateText(void);

private:
    QString caption;
    bool is_server;
    bool server_is_created;
    QSerialPort serial;
    QByteArray  serial_data;
    LogBox *logBox;

    QLineEdit *le_Network;
    QLineEdit *le_Password;
    QLineEdit *le_IP;
    QLineEdit *le_Gate;
    QLineEdit *le_Mask;
    QLineEdit *le_RemoteIP;
    QLineEdit *le_RemotePort;
    QComboBox *cb_EncryptType;

    bool is_ready;
    QLabel *port_caption;
    QComboBox *cb_ports;
    QPushButton *btn_open;
    QPushButton *btn_close;

    QPushButton *btn_server;
    QPushButton *btn_client;
    QPushButton *btn_send_data;

    QPushButton *btn_read_settings;

    void init(void);
    void connect_log(void);
    void connect_serial(void);
    bool send_at_command(QString cmd,
                         unsigned int wait_ms = 100,
                         bool no_response = false);
    bool send_command(QString cmd,
                      unsigned int wait_ms = 100);

    bool send_cmd_create_server(bool is_silense = false);
    bool send_cmd_create_client(bool is_silense = false);

    void wait_msec(int timeout_msec);
    QVBoxLayout *add_server_cmd_layout(void);
    QVBoxLayout *add_client_cmd_layout(void);
    QGridLayout *add_grid_layout(void);

    QString get_server_string(void);
    QString get_client_string(void);

    void show_hex_data(QByteArray &data);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // WIFI_FRAME_HPP
