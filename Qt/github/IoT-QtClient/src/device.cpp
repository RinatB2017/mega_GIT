/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "device.h"

#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include "device_p.h"

namespace iot
{

Device::Device(QObject *parent) : QObject(parent), d_ptr(new DevicePrivate(this))
{
}

Device::Device(DevicePrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd)
{

}

Device::~Device()
{
}

Device* Device::fromJson(const QJsonObject& object)
{
    Device* device = new Device();
    if (!device->d_ptr->initialize(object)) {
        qDebug() << "Device initialization from jsonobject failed";
        delete device;
        device = nullptr;
    }
    return device;
}

QString Device::name() const
{
    Q_D(const Device);
    return d->m_name;
}

QString Device::manufacturer() const
{
    Q_D(const Device);
    return d->m_manufacturer;
}

QString Device::type() const
{
    Q_D(const Device);
    return d->m_type;
}

QString Device::description() const
{
    Q_D(const Device);
    return d->m_description;
}

QString Device::deviceId() const
{
    Q_D(const Device);
    return d->m_deviceId;
}

QString Device::href() const
{
    Q_D(const Device);
    return d->m_href;
}

void Device::setName(const QString &name)
{
    Q_D(Device);
    if (d->m_name != name) {
        d->m_name = name ;
        emit nameChanged(d->m_name);
    }
}

void Device::setManufacturer(const QString &manufacturer)
{
    Q_D(Device);
    if (d->m_manufacturer != manufacturer) {
        d->m_manufacturer = manufacturer;
        emit manufacturerChanged(d->m_manufacturer);
    }
}

void Device::setType(const QString &type)
{
    Q_D(Device);
    if (d->m_type != type) {
        d->m_type = type;
        emit typeChanged(d->m_type);
    }
}

void Device::setDescription(const QString &description)
{
    Q_D(Device);
    if (d->m_description != description) {
        d->m_description = description;
        emit descriptionChanged(d->m_description);
    }
}

QVariantMap Device::attributes() const
{
    Q_D(const Device);
    return d->m_attributes;
}

QDateTime Device::creationTime() const
{
    Q_D(const Device);
    return d->m_creationTime;
}

QList<DataNode*> Device::dataNodes() const
{
    Q_D(const Device);
    return d->m_dataNodes;
}

Error* Device::registerError()
{
    Q_D(Device);
    return &d->m_registerError;
}

Error* Device::getDetailsError()
{
    Q_D(Device);
    return &d->m_getError;
}

Error *Device::getDataNodesError()
{
    Q_D(Device);
    return &d->m_getDataNodesError;
}

void Device::addDataNode(DataNode* node)
{
    Q_D(Device);
    if (!d->m_dataNodes.contains(node))
        d->m_dataNodes << node;
}

void Device::addDataNodes(QList<DataNode*> nodes)
{
    foreach(auto node, nodes) {
        addDataNode(node);
    }
}

void Device::removeDataNode(DataNode* node)
{
    Q_D(Device);
    d->m_dataNodes.removeOne(node);
}

void Device::removeDataNodes(QList<DataNode*> nodes)
{
    foreach(auto node, nodes) {
        removeDataNode(node);
    }
}

void Device::readDataNodesLatestValue(QList<DataNode*> nodes)
{
    Q_D(Device);
    d->readDataNodeValues(nodes.isEmpty() ? d->m_dataNodes : nodes, QDateTime(), QDateTime());
}

void Device::readDataNodeValues(const QDateTime& startTime, const QDateTime& endTime, QList<DataNode*> nodes)
{
    Q_D(Device);
    d->readDataNodeValues(nodes.isEmpty() ? d->m_dataNodes : nodes, startTime, endTime);
}

void Device::readDataNodeValues(const QDateTime &startTime, QList<DataNode *> nodes)
{
    Q_D(Device);
    d->readDataNodeValues(nodes.isEmpty() ? d->m_dataNodes : nodes, startTime, QDateTime());
}

void Device::setAttributes(const QVariantMap& attributes)
{
    Q_D(Device);
    if (d->m_attributes != attributes) {
        d->m_attributes = attributes;
        emit attributesChanged(d->m_attributes);
    }
}

void Device::setDeviceId(const QString& deviceId)
{
    Q_D(Device);
    if (d->m_deviceId != deviceId) {
        d->m_deviceId = deviceId;
        emit deviceIdChanged(d->m_deviceId);
    }
}

void Device::setHref(const QString& href)
{
    Q_D(Device);
    if (d->m_href != href) {
        d->m_href = href;
        emit hrefChanged(d->m_href);
    }
}

void Device::setCreationTime(const QDateTime& creationTime)
{
    Q_D(Device);
    if (d->m_creationTime != creationTime) {
        d->m_creationTime = creationTime;
        emit creationTimeChanged(d->m_creationTime);
    }
}

bool Device::operator ==(const Device &other) const
{
    return  name() == other.name() &&
        manufacturer() == other.manufacturer() &&
        type() == other.type() &&
        description() == other.description() &&
        deviceId() == other.deviceId() &&
        attributes() == other.attributes() &&
        creationTime().toString(Qt::ISODate) == other.creationTime().toString(Qt::ISODate) &&
        href() == other.href();
}

void Device::registerDevice()
{
    Q_D(Device);
    d->registerDevice();
}

void Device::getDetails(const QString& deviceId)
{
    Q_D(Device);
    d->getDetails(deviceId);
}

void Device::getDataNodes(const QString& deviceId)
{
    Q_D(Device);
    d->getDataNodes(deviceId);
}

QDebug operator<<(QDebug dbg, const Device& device)
{
    dbg << "Name: " << device.name();
    dbg << "DeviceId: " << device.deviceId();
    dbg << "Description: " << device.description();
    dbg << "Manufacturer: " << device.manufacturer();
    dbg << "Type: " << device.type();
    dbg << "Attributes: " << device.attributes();
    dbg << "CreatedAt: " << device.creationTime();
    dbg << "HRef: " << device.href();
    return dbg;
}



}//eon
