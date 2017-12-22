#include "bus.h"

#include <datanode.h>
#include <device.h>

#include <QDebug>

Bus::Bus(iot::Device* device, QObject *parent) :
    QObject(parent),
    m_device(device),
    m_speedDataNode(new iot::DataNode(this)),
    m_fuelConsumptionDataNode(new iot::DataNode(this)),
    m_minSpeed(0),
    m_maxSpeed(120),
    m_currentSpeed(0),
    m_minFuelConsumption(0),
    m_maxFuelConsumption(20),
    m_accelerationMinTime(5),
    m_accelerationMaxTime(20)
{
    m_speedDataNode->setDeviceId(m_device->deviceId());
    m_speedDataNode->setName("speed");
    m_speedDataNode->setUnit("km/h");
    m_speedDataNode->setDataType(iot::DataNode::Double);
    m_device->addDataNode(m_speedDataNode);

    m_fuelConsumptionDataNode->setDeviceId(m_device->deviceId());
    m_fuelConsumptionDataNode->setName("fuelConsumption");
    m_fuelConsumptionDataNode->setUnit("l/h");
    m_fuelConsumptionDataNode->setDataType(iot::DataNode::Double);
    m_device->addDataNode(m_fuelConsumptionDataNode);

    connect(m_speedDataNode, &iot::DataNode::writeFinished, [this](bool success) {
        if (success) {
            qDebug() << m_device->name() << " speed value written";
        } else {
            qDebug() << "writing value failed";
        }
    });

    connect(&m_timer, &QTimer::timeout, this, &Bus::generateDataToIotServer);
}

void Bus::setSpeedRange(qreal min, qreal max)
{
    m_minSpeed = min;
    m_maxSpeed = max;
}

void Bus::setFuelConsumptionRange(qreal min, qreal max)
{
    m_minFuelConsumption = min;
    m_maxFuelConsumption = max;
}

void Bus::setAccelerationTimeRange(quint32 minTime, quint32 maxTime)
{
    m_accelerationMinTime = minTime;
    m_accelerationMaxTime = maxTime;
}

void Bus::startSimulation()
{
//    if (m_device->name() == "Bus1") {
        updateTargets();
        m_timer.start(1000);
//    }
}

void Bus::updateTargets()
{
    m_targetSpeed = qMax((int)m_minSpeed,qrand() % (int)m_maxSpeed);
//    qDebug() << "target speed " << m_targetSpeed;
    int accelerationTime = qMax((int)m_accelerationMinTime, qrand() % (int)m_accelerationMaxTime);
//    qDebug() << "acceleraton time: " << accelerationTime;
    m_speedIncrement = (m_targetSpeed - m_currentSpeed) / accelerationTime;
//    qDebug() << m_speedIncrement;
//    qreal targetConsumption =
}

void Bus::generateDataToIotServer()
{
    m_currentSpeed += m_speedIncrement;
//    qDebug() << "speed " << m_currentSpeed;
    m_currentFuelConsumption = m_minFuelConsumption + (m_currentSpeed / m_maxSpeed)*(m_maxFuelConsumption);
//    qDebug() << "consumption " << m_currentFuelConsumption;

    m_speedDataNode->writeValue(QPair<QVariant, qint64>(m_currentSpeed, QDateTime::currentDateTime().toMSecsSinceEpoch()));
    m_fuelConsumptionDataNode->writeValue(QPair<QVariant, qint64>(m_currentFuelConsumption, QDateTime::currentDateTime().toMSecsSinceEpoch()));
//    m_device->addDataNode();
    if (m_speedIncrement > 0) {
        if (m_currentSpeed > m_targetSpeed)
            updateTargets();
    } else {
        if (m_currentSpeed > m_targetSpeed)
            updateTargets();
    }
}











