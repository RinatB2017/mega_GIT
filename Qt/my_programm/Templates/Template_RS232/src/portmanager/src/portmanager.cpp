/*********************************************************************************
**                                                                              **
**     Copyright (C) 2024                                                       **
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
#include "portmanager.hpp"
//--------------------------------------------------------------------------------
PortManager::PortManager(QComboBox *comboBox, QObject *parent)
    : QObject(parent), m_comboBox(comboBox)
{
    updatePorts(); // Изначальное обновление списка портов

    // Настройка таймера для периодической проверки
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PortManager::checkPorts);
    m_timer->start(5000); // Проверка каждые 5 секунд
}
//--------------------------------------------------------------------------------
PortManager::~PortManager()
{
    if(m_timer)
    {
        delete m_timer;
    }
}
//--------------------------------------------------------------------------------
void PortManager::updatePorts(void)
{
    QList<QSerialPortInfo> currentPorts = QSerialPortInfo::availablePorts();
    QStringList currentPortNames;
    for (const QSerialPortInfo &port : currentPorts)
    {
        currentPortNames << port.portName();
    }

    // Обновление QComboBox
    m_comboBox->clear();
    m_comboBox->addItems(currentPortNames);
}
//--------------------------------------------------------------------------------
void PortManager::checkPorts(void)
{
    QStringList currentPortNames;
    QList<QSerialPortInfo> currentPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : currentPorts)
    {
        currentPortNames << port.portName();
    }

    // Сравнение с предыдущим состоянием
    if (currentPortNames != m_portNames)
    {
        m_portNames = currentPortNames; // Обновляем текущее состояние
        updateComboBox();               // Обновляем QComboBox
    }
}
//--------------------------------------------------------------------------------
void PortManager::updateComboBox(void)
{
    m_comboBox->clear();
    m_comboBox->addItems(m_portNames);
}
//--------------------------------------------------------------------------------
