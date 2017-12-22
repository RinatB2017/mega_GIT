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

#ifndef DEVICE_P_H
#define DEVICE_P_H

#include "device.h"
#include "response.h"
#include "error.h"
#include "item_p.h"

#include <QScopedPointer>

namespace iot
{

class DataNode;

class DevicePrivate : public ItemPrivate
{
    Q_OBJECT
public:
    DevicePrivate(Device* parent) : ItemPrivate(), q_ptr(parent) {}
    virtual ~DevicePrivate();

    QString m_name;
    QString m_manufacturer;
    QString m_type;
    QString m_description;
    QString m_deviceId;
    QString m_href;
    QDateTime m_creationTime;
    QVariantMap m_attributes;
    QList<DataNode*> m_dataNodes;
    QList<DataNode*> m_writeNodes;
    QList<DataNode*> m_readNodes;
    Error m_registerError;
    Error m_getError;
    Error m_getDataNodesError;
    Error m_readDataNodeValuesError;

    QScopedPointer<Response> m_createResponse;
    QScopedPointer<Response> m_getDetailsResponse;
    QScopedPointer<Response> m_getDataNodesResponse;
    QScopedPointer<Response> m_readDataNodeValuesResponse;

    void registerDevice();
    void getDetails(const QString& deviceId);
    void getDataNodes(const QString& deviceId);
    bool initialize(const QJsonObject& object);
    void readDataNodeValues(QList<DataNode*> nodes, const QDateTime& startTime, const QDateTime& endTime);
    DataNode* findDataNode(const QList<DataNode*>& nodes, const QString& name, const QString& path);

protected slots:

    void onCreateFinished();
    void onGetDetailsFinished();
    void onGetDataNodesFinished();
    void onReadDataNodeValuesFinished();

private:

    Q_DECLARE_PUBLIC(Device)
    Device* const q_ptr;

};

} // namespace qiot

#endif // DEVICE_P_H
