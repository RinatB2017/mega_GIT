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

#include <QJsonObject>
#include <QJsonArray>
#include <QMetaEnum>
#include <QJsonDocument>

#include "datanode_p.h"
#include "requesthandlerprovider.h"
#include "requesthandler.h"

namespace iot
{

DataNodePrivate::DataNodePrivate(DataNode *parent) :
    ItemPrivate(),
    m_dataType(DataNode::Unkown),
    m_writingCachedValues(false),
    q_ptr(parent)
{

}

bool DataNodePrivate::initialize(const QJsonObject& object)
{
    Q_Q(DataNode);
    bool error = false;
    error &= resolveKeyAndCallSetter(object, "unit", [&](const QString& unit) { q->setUnit(unit); } );
    error &= resolveKeyAndCallSetter(object, "href", [&](const QString& href) { q->setHref(href); } );
    error &= resolveKeyAndCallSetter(object, "name", [&](const QString& name) { q->setName(name); } );
    // path not mandatory since its possible to register node without a path
    resolveKeyAndCallSetter(object, "path", [&](const QString& path) { q->setPath(path); } );
    error &= resolveKeyAndCallSetter(object, "dataType", [&](const QString& dataType) {
        const QMetaEnum me = QMetaEnum::fromType<DataNode::DataType>();
        bool found = true;
        // Server datatype names begin with lower letter but our enums use begin with capital letter.
        QString convertedDataType = dataType;
        convertedDataType.replace(0,1,dataType.left(1).toUpper());
        const int value = me.keyToValue(convertedDataType.toLatin1().constData(), &found);
        if (found) {
            q->setDataType(static_cast<DataNode::DataType>(value));
        } else {
            qDebug() << "unsupported datatype: " << dataType;
        }
    } );
    return !error;

}

bool DataNodePrivate::writeValues(const QList<QPair<QVariant, qint64>>& list)
{
    if (!isReadyToWrite()) return false;

    m_writeCount = list.count();
    QJsonArray dataNodeList;
    foreach(auto var, list) {
        QJsonObject obj;
        obj["name"] = m_name; // required
        obj["v"] = QJsonValue::fromVariant(var.first); // required
        if (var.second != -1) obj["ts"] = var.second; // optional
        if (!m_path.isEmpty()) obj["path"] = m_path; // optional
        if (!m_unit.isEmpty()) obj["unit"] = m_unit; // optional
        if (m_dataType != DataNode::Unkown) { // optional
            QMetaEnum me = QMetaEnum::fromType<DataNode::DataType>();
            obj["dataType"] = me.valueToKey(m_dataType);
        }
        dataNodeList << obj;
    }

    QJsonDocument doc(dataNodeList);
    m_writeResponse.reset( RequestHandlerProvider::instance()->postRequest("/process/write/" + m_deviceId, doc.toJson()) );
    QObject::connect( m_writeResponse.data(), &Response::finished, this, &DataNodePrivate::onWriteFinished);
    return true;
}

bool DataNodePrivate::writeCachedValues()
{
    bool retval = writeValues(m_cachedValues);
    if (retval) m_writingCachedValues = true;
    return retval;
}

bool DataNodePrivate::readValues(const QDateTime& startTime, const QDateTime& endTime)
{
    if (!isReadyToRead()) return false;

    // values aren't stored between reads
    QString path = QString("/process/read/%1?datanodes=%2/%3").arg(m_deviceId, m_path, m_name);
    if (startTime.isValid() && endTime.isValid())
        path += QString("&fromdate=%1&todate=%2").arg(startTime.toMSecsSinceEpoch()).arg(endTime.toMSecsSinceEpoch());
    else if (startTime.isValid())
        path += QString("&fromdate=%1").arg(startTime.toMSecsSinceEpoch());

    m_readResponse.reset( RequestHandlerProvider::instance()->getRequest(path));
    QObject::connect( m_readResponse.data(), &Response::finished, this, &DataNodePrivate::onReadFinished);
    return true;
}

void DataNodePrivate::addReadValues(const QJsonArray& values, bool notify)
{
    Q_Q(DataNode);
    foreach(auto valueVal, values) {
        const QJsonObject obj = getObject(valueVal);
        QPair<QVariant, QDateTime> valueTimePair;
        valueTimePair.first = getSingleValue(obj, "v").toVariant();
        valueTimePair.second = QDateTime::fromMSecsSinceEpoch( (qint64)getValue(obj, "ts", QJsonValue::Double).toDouble());
        m_values << valueTimePair;
    }

    if (notify) {
        emit q->readFinished(true);
    }
}

bool DataNodePrivate::isReadyToWrite() const
{
    return m_writeResponse.data() == nullptr;
}

bool DataNodePrivate::isReadyToRead() const
{
    return m_readResponse.data() == nullptr;
}

void DataNodePrivate::onWriteFinished()
{
    Q_Q(DataNode);

    bool error = true;

    m_writeError.reset();

    try {
        if (isValidResponse(*m_writeResponse, m_writeError, 200)) {
            error = false;
            const QJsonObject rootObject = getObject(m_writeResponse->document());
            const QJsonArray results = getValue(rootObject, "writeResults", QJsonValue::Array).toArray();
            if (results.count() != 1) throw JsonError("Invalid amount of result objects");

            // We can ignore href because we dont need to identify the node.

            int writtenCount = getValue(getObject(results[0]), "writtenCount", QJsonValue::Double).toInt();
            if (m_writingCachedValues) {
                for (int i = 0; i < writtenCount; i++) {
                    m_cachedValues.removeFirst();
                }
            }

            if (writtenCount != m_writeCount) {

                throw GenericError(QString("Not all values were written. Send count %1, written count=%2").arg(m_writeCount, writtenCount));
                error = true;
            }
        }
    } catch (std::exception& exception) {
        m_readError.setErrorType(Error::ErrorType::GenericError);
        m_writeError.setDescription(exception.what());
        error = true;
    }

    emit q->writeFinished(!error);

    m_writeResponse.reset(nullptr);
    emit q->readyToWriteChanged(true);
    m_writingCachedValues = false;
}

void DataNodePrivate::onReadFinished()
{
    Q_Q(DataNode);

    bool error = true;

    m_values.clear();
    m_readError.reset();

    try {
        if (isValidResponse(*m_readResponse, m_readError, 200)) {
            error = false;
            const QJsonObject rootObject = getObject(m_readResponse->document());
            const QJsonArray results = getValue(rootObject, "datanodeReads", QJsonValue::Array).toArray();
            foreach( auto val, results) {
                const QJsonArray valueArray = getValue(getObject(val), "values", QJsonValue::Array).toArray();
                addReadValues(valueArray, false);
            }
        }
    } catch (std::exception& exception) {
        m_readError.setErrorType(Error::ErrorType::GenericError);
        m_readError.setDescription(exception.what());
        error = true;
    }

    emit q->readFinished(!error);

    m_readResponse.reset(nullptr);
    emit q->readyToReadChanged(true);
}

} // namespace iot

