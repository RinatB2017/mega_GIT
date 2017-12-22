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

#include "device_p.h"
#include "datanode.h"
#include "requesthandlerprovider.h"
#include "requesthandler.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QDebug>
#include <QNetworkReply>

namespace iot
{

DevicePrivate::~DevicePrivate()
{
    qDeleteAll(m_dataNodes);
}

void DevicePrivate::registerDevice() {
    QJsonArray attributesArray;
    for( auto key: m_attributes.keys()) {
        attributesArray << QJsonObject( { {key, QJsonValue::fromVariant(m_attributes[key])} } );
    }
    QJsonObject obj = {
        {"name", m_name},
        {"manufacturer", m_manufacturer},
        {"type", m_type},
        {"description", m_description},
        {"attributes", attributesArray}
    };

    QJsonDocument doc(obj);
    m_createResponse.reset( RequestHandlerProvider::instance()->postRequest("/devices/", doc.toJson()) );
    QObject::connect( m_createResponse.data(), &Response::finished, this, &DevicePrivate::onCreateFinished);
}

void DevicePrivate::getDetails(const QString& deviceId) {
    QString id = deviceId.isEmpty() ? m_deviceId : deviceId;
    m_getDetailsResponse.reset( RequestHandlerProvider::instance()->getRequest("/devices/" + id) );
    QObject::connect( m_getDetailsResponse.data(), &Response::finished, this, &DevicePrivate::onGetDetailsFinished);
}

void DevicePrivate::getDataNodes(const QString& deviceId)
{
    QString id = deviceId.isEmpty() ? m_deviceId : deviceId;
    m_getDataNodesResponse.reset( RequestHandlerProvider::instance()->getRequest("/devices/" + id + "/datanodes") );
    QObject::connect( m_getDataNodesResponse.data(), &Response::finished, this, &DevicePrivate::onGetDataNodesFinished);
}

bool DevicePrivate::initialize(const QJsonObject& object)
{
    Q_Q(Device);
    bool error = false;
    QJsonValue value = object.value("attributes");
    if (value.isArray()) {
        QJsonArray array = value.toArray();
        QVariantMap attributesMap;
        for (auto jsonVal : array) {
            if (jsonVal.isObject()) {
                QJsonObject obj = jsonVal.toObject();
                QStringList keys = obj.keys();
                for (auto key : keys) {
                    attributesMap[key] = obj.value(key).toVariant();
                }
            } else {
                error = true;
                qDebug() << "Error while parsing response document";
                return error;
            }
        }
        q->setAttributes(attributesMap);
    } else {
        // Attributes aren't mandatory
    }

    const bool NOT_MANDATORY_ATTRIBUTE = false;

    q->setHref( getValue(object, "href", QJsonValue::String, NOT_MANDATORY_ATTRIBUTE).toString() );
    q->setCreationTime( QDateTime::fromString( getValue(object, "createdAt", QJsonValue::String ).toString(), Qt::ISODate) );
    q->setDeviceId( getValue(object, "deviceId", QJsonValue::String).toString() );
    q->setName( getValue(object, "name", QJsonValue::String).toString() );
    q->setDescription( getValue(object, "description", QJsonValue::String, NOT_MANDATORY_ATTRIBUTE).toString() );
    q->setType( getValue(object, "type", QJsonValue::String).toString() );
    q->setManufacturer( getValue(object, "manufacturer", QJsonValue::String, NOT_MANDATORY_ATTRIBUTE).toString() );
    return !error;
}

void DevicePrivate::readDataNodeValues(QList<DataNode*> nodes, const QDateTime& startTime, const QDateTime& endTime)
{
    if (nodes.isEmpty()) {
        qDebug() << "Node list empty, nothing to do";
        return;
    }

    m_readNodes = nodes;

    // Build node id list from nodes
    QString nodeIds;
    for(int i = 0; i < nodes.count(); i++) {
        nodeIds += QString("%1/%2").arg(nodes[i]->path(), nodes[i]->name());
        // Dont add separetor if we are processing last node
        if (i < nodes.count()-1) nodeIds.append(",");
    }

    QString path = QString("/process/read/%1?datanodes=%2").arg(m_deviceId, nodeIds);
    if (startTime.isValid() && endTime.isValid())
        path += QString("&fromdate=%1&todate=%2").arg(startTime.toMSecsSinceEpoch()).arg(endTime.toMSecsSinceEpoch());
    else if (startTime.isValid())
        path += QString("&fromdate=%1").arg(startTime.toMSecsSinceEpoch());
    m_readDataNodeValuesResponse.reset( RequestHandlerProvider::instance()->getRequest(path));
    QObject::connect( m_readDataNodeValuesResponse.data(), &Response::finished, this, &DevicePrivate::onReadDataNodeValuesFinished);
}

DataNode* DevicePrivate::findDataNode(const QList<DataNode*>& nodes, const QString& name, const QString& path)
{
    foreach(auto node, nodes) {
        if (node->path() == path && node->name() == name) {
            return node;
        }
    }

    return nullptr;
}

void DevicePrivate::onCreateFinished()
{
    Q_Q(Device);
    m_registerError.reset();
    bool error = true;
    try {
        if (isValidResponse(*m_createResponse, m_registerError, 201)) {
            error = false;
            QJsonObject rootObject = getObject(m_createResponse->document());
            q->setDeviceId( getValue(rootObject, "deviceId", QJsonValue::String).toString() );
            QString creationTime = getValue(rootObject, "createdAt", QJsonValue::String).toString();
            q->setCreationTime(QDateTime::fromString(creationTime, Qt::ISODate));
        }
    } catch (std::exception& exception) {
        m_registerError.setErrorType(Error::ErrorType::GenericError);
        m_registerError.setDescription(exception.what());
        error = true;
    }

    emit q->registerFinished(!error);
}

void DevicePrivate::onGetDetailsFinished()
{
    Q_Q(Device);
    m_getError.reset();
    bool error = true;
    try {
        if (isValidResponse(*m_getDetailsResponse, m_getError, 200)) {
            error = !initialize(getObject(m_getDetailsResponse->document()));
        }
    } catch (std::exception& exception) {
        m_getError.setErrorType(Error::ErrorType::GenericError);
        m_getError.setDescription(exception.what());
        error = true;
    }

    emit q->getDetailsFinished(!error);
}

void DevicePrivate::onGetDataNodesFinished()
{
    Q_Q(Device);
    m_getDataNodesError.reset();
    bool error = true;
    try {
        if (isValidResponse(*m_getDataNodesResponse, m_getDataNodesError, 200)) {
            error = false;
            QJsonObject rootObject = getObject(m_getDataNodesResponse->document());
            // TODO handle partial response
            int offset = getValue(rootObject, "offset", QJsonValue::Double).toInt();
            Q_UNUSED(offset);
            int limit = getValue(rootObject,"limit", QJsonValue::Double).toInt();
            Q_UNUSED(limit);
            int fullSize = getValue(rootObject, "fullSize", QJsonValue::Double).toInt();
            Q_UNUSED(fullSize);
            QJsonArray items = getValue(rootObject, "items", QJsonValue::Array, fullSize != 0).toArray();
            if (!items.isEmpty()) {
                foreach (auto item, items) {
                    DataNode* dataNode = DataNode::fromJson(getObject(item));
                    if (dataNode) {
                        dataNode->setDeviceId(m_deviceId);
                        m_dataNodes << dataNode;
                    } else {
                        throw JsonError();
                    }
                }
            }
        }
    } catch (std::exception& exception) {
        qDebug() << "exception " << exception.what();
        m_getError.setErrorType(Error::ErrorType::GenericError);
        m_getError.setDescription(exception.what());
        error = true;
    }

    emit q->getDataNodesFinished(!error);
}

void DevicePrivate::onReadDataNodeValuesFinished()
{
    Q_Q(Device);

    bool error = true;
    m_readDataNodeValuesError.reset();

    try {
        if (isValidResponse(*m_readDataNodeValuesResponse, m_readDataNodeValuesError, 200)) {
            error = false;
            const QJsonObject rootObject = getObject(m_readDataNodeValuesResponse->document());
            const QJsonArray results = getValue(rootObject, "datanodeReads", QJsonValue::Array).toArray();
            foreach( auto val, results) {
                QJsonObject valueObject = getObject(val);
                QString name = valueObject["name"].toString();
                QString path = valueObject["path"].toString();
                DataNode* node = findDataNode(m_readNodes, name, path);
                if (node) {
                    node->addReadValues( getValue(valueObject, "values", QJsonValue::Array).toArray() );
                }
            }
        }
    } catch (std::exception& exception) {
        m_readDataNodeValuesError.setErrorType(Error::ErrorType::GenericError);
        m_readDataNodeValuesError.setDescription(exception.what());
        error = true;
    }

    emit q->readDataNodeValuesFinished(!error);
}

} // namespace iot
