#include "bus.h"

#include <device.h>
#include <datanode.h>

#include <QDebug>

Bus::Bus(iot::Device* device, QObject *parent) : QObject(parent), m_device(device)
{
    connect(&m_timer, &QTimer::timeout, device, [this]() { m_device->readDataNodesLatestValue(); });
    m_timer.setInterval(1000);

    connect(device, &iot::Device::getDataNodesFinished, [this](bool success) {
        if (success) {
            QList<iot::DataNode*> nodes = m_device->dataNodes();
            qDebug() << "Got device datanodes. Node count: " << nodes.count();
            foreach(iot::DataNode* node, nodes) {
                if (node->name() == "speed") {
                    connect(node, &iot::DataNode::readFinished, [=](bool success) {
                        if (success) {
                            setSpeed(node->latestValue());
                        }
                    });
                } else if (node->name() == "fuelConsumption") {
                    connect(node, &iot::DataNode::readFinished, [=](bool success) {
                        if (success) {
                            setFuelConsumption(node->latestValue());
                        }
                    });
                } else {
                    qDebug() << "DataNode not regocnized: " << node->name();
                }
            }

            startReading();
        } else {
            qDebug() << "Failed to get device datanodes";
        }
    });

    device->getDataNodes();
}

Bus::Bus(QObject *parent) : QObject(parent)
{

}

Bus::~Bus()
{

}

qreal Bus::speed() const
{
    return m_speed.first.toReal();
}

qreal Bus::fuelConsumption() const
{
    return m_fuelConsumption.first.toReal();
}

QString Bus::name() const
{
    return m_device->name();
}

void Bus::startReading()
{
    m_timer.start();
}

void Bus::stopReading()
{
    m_timer.stop();
}

void Bus::setFuelConsumption(const QPair<QVariant, QDateTime>& val)
{
//    qDebug() << "consumption " << val.first;
    m_fuelConsumption = val;
    QPair<QVariant,QDateTime> zeroPair(0,QDateTime::currentDateTime());
//    qDebug() << QDateTime::currentDateTime();
//    qDebug() << m_fuelConsumption.second;
//    qDebug() << QDateTime::currentDateTime().secsTo(m_fuelConsumption.second);
    if (QDateTime::currentDateTime().secsTo(m_fuelConsumption.second) < -3) {
        m_fuelConsumption.swap(zeroPair);
    }

    emit fuelConsumptionChanged(m_fuelConsumption.first.toReal());
}

void Bus::setSpeed(const QPair<QVariant, QDateTime>& val)
{
//    qDebug() << "speed " << val.first;
    m_speed = val;
    QPair<QVariant,QDateTime> zeroPair(0,QDateTime::currentDateTime());
    if (QDateTime::currentDateTime().secsTo(m_speed.second) < -3)
        m_speed.swap(zeroPair);

    emit speedChanged(m_speed.first.toReal());
}
