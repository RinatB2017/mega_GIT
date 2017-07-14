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
#ifndef MODBUS_CLIENT_HPP
#define MODBUS_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QModbusDataUnit>
#include <QModbusDevice>
//--------------------------------------------------------------------------------
namespace Ui {
    class MODBUS_client;
}
//--------------------------------------------------------------------------------
class QModbusClient;
class QModbusReply;
//--------------------------------------------------------------------------------
class MODBUS_client : public QWidget
{
    Q_OBJECT

public:
    explicit MODBUS_client(QWidget *parent = 0);
    ~MODBUS_client();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private slots:
    void log(QString data);
    void readReady(void);
    void errorOccurred(QModbusDevice::Error);
    void stateChanged(QModbusDevice::State state);

    void connect_device(void);
    void refresh(void);
    void test(void);

private:
    Ui::MODBUS_client *ui;

    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
};
//--------------------------------------------------------------------------------
#endif
