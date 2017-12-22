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

#include "datanode.h"
#include "datanode_p.h"

namespace iot
{

DataNode::DataNode(QObject *parent) : QObject(parent), d_ptr(new DataNodePrivate(this))
{

}

DataNode::DataNode(DataNodePrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd)
{

}

DataNode::~DataNode()
{

}

DataNode* DataNode::fromJson(const QJsonObject& object)
{
    DataNode* dataNode= new DataNode();
    if (!dataNode->d_ptr->initialize(object)) {
        qDebug() << "DataNode initialization from jsonobject failed";
        delete dataNode;
        dataNode = nullptr;
    }

    return dataNode;
}

void DataNode::addReadValues(const QJsonArray& values)
{
    Q_D(DataNode);
    d->addReadValues(values);
}

QString DataNode::name() const
{
    Q_D(const DataNode);
    return d->m_name;
}

void DataNode::setName(const QString& name)
{
    Q_D(DataNode);
    if (d->m_name != name) {
        d->m_name = name;
        emit nameChanged(d->m_name);
    }
}

QString DataNode::unit() const
{
    Q_D(const DataNode);
    return d->m_unit;
}

void DataNode::setUnit(const QString& unit)
{
    Q_D(DataNode);
    if (d->m_unit != unit) {
        d->m_unit = unit;
        emit unitChanged(d->m_unit);
    }
}

DataNode::DataType DataNode::dataType() const
{
    Q_D(const DataNode);
    return d->m_dataType;
}

void DataNode::setDataType(DataType dataType)
{
    Q_D(DataNode);
    if (d->m_dataType != dataType) {
        d->m_dataType = dataType;
        emit dataTypeChanged(d->m_dataType);
    }
}

QString DataNode::href() const
{
    Q_D(const DataNode);
    return d->m_href;
}

void DataNode::setHref(const QString& href)
{
    Q_D(DataNode);
    if (d->m_href != href) {
        d->m_href = href;
        emit hrefChanged(d->m_href);
    }
}

QString DataNode::path() const
{
    Q_D(const DataNode);
    return d->m_path;
}

void DataNode::setPath(const QString& path)
{
    Q_D(DataNode);
    if (d->m_path != path) {
        d->m_path = path;
        emit pathChanged(d->m_path);
    }
}

QString DataNode::deviceId() const
{
    Q_D(const DataNode);
    return d->m_deviceId;
}

void DataNode::setDeviceId(const QString& deviceId)
{
    Q_D(DataNode);
    if (d->m_deviceId != deviceId) {
        d->m_deviceId = deviceId;
        emit deviceIdChanged(d->m_deviceId);
    }
}

QList<QPair<QVariant, QDateTime>> DataNode::values() const
{
    Q_D(const DataNode);
    return d->m_values;
}

QPair<QVariant, QDateTime> DataNode::latestValue() const
{
    Q_D(const DataNode);
    return d->m_values.last();
}

bool DataNode::writeValue(const QVariant& val, bool cache)
{
    return writeValue( {val, -1}, cache );
}

bool DataNode::writeValue(const QPair<QVariant, qint64>& val, bool cache)
{
    return writeValues( {{ val }}, cache );
}

bool DataNode::writeValues(const QVariantList& list, bool cache)
{
    QList<QPair<QVariant, qint64>> pairList;
    foreach(const QVariant& var, list) {
        pairList << QPair<QVariant,qint64>(var, -1); // Review, mky, 20160329: qMakePair macro could be used here
    }
    return writeValues(pairList, cache);
}

bool DataNode::writeValues(const QList<QPair<QVariant, qint64>>& list, bool cache)
{
    Q_D(DataNode);
    if (cache) {
        d->m_cachedValues += list;
        return true;
    } else {
        return d->writeValues(list);
    }
}

bool DataNode::writeCachedValues()
{
    Q_D(DataNode);
    return d->writeCachedValues();
}

void DataNode::clearCachedValues()
{
    Q_D(DataNode);
    d->m_cachedValues.clear();
}

QList<QPair<QVariant, qint64>> &DataNode::cachedValues()
{
    Q_D(DataNode);
    return d->m_cachedValues;
}

bool DataNode::readLatestValue()
{
    Q_D(DataNode);
    return d->readValues();
}

bool DataNode::readValues(const QDateTime& startTime, const QDateTime& endTime)
{
    Q_D(DataNode);
    return d->readValues(startTime, endTime);
}

bool DataNode::readValues(const QDateTime& startTime)
{
    Q_D(DataNode);
    return d->readValues(startTime);
}

bool DataNode::isReadyToRead() const
{
    Q_D(const DataNode);
    return d->isReadyToRead();
}

bool DataNode::isReadyToWrite() const
{
    Q_D(const DataNode);
    return d->isReadyToWrite();
}

Error* DataNode::writeError()
{
    Q_D(DataNode);
    return &d->m_writeError;
}

Error* DataNode::readError()
{
    Q_D(DataNode);
    return &d->m_readError;
}

bool DataNode::operator ==(const DataNode &other) const
{
    return  name() == other.name() &&
        href() == other.href() &&
        dataType() == other.dataType() &&
        unit() == other.unit() &&
        path() == other.path();
}

QDebug operator<<(QDebug dbg, const DataNode& dataNode)
{
    dbg << "Name: " << dataNode.name();
    dbg << "Unit: " << dataNode.unit();
    dbg << "DataType: " << dataNode.dataType();
    dbg << "Href: " << dataNode.href();
    dbg << "Path: " << dataNode.path();
    dbg << "DeviceId: " << dataNode.deviceId();
    return dbg;
}

} // namespace iot
