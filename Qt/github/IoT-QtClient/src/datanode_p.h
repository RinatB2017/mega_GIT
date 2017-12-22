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

#ifndef DATANODE_P_H
#define DATANODE_P_H

#include "datanode.h"
#include "item_p.h"
#include "response.h"
#include "error.h"

namespace iot
{

class DataNodePrivate : public ItemPrivate
{
    Q_OBJECT
public:
    explicit DataNodePrivate(DataNode* parent);
    virtual ~DataNodePrivate() {}

    QString m_name;
    QString m_unit;
    DataNode::DataType m_dataType; // Review, mky, 20160329: Is this actually used somewhere or is it always DataNode::Unkown
    QString m_href;
    QString m_path;
    QString m_deviceId;
    int m_writeCount; // Review, mky, 20160329: Should be added to init list
    QScopedPointer<Response> m_writeResponse;
    QScopedPointer<Response> m_readResponse;
    Error m_writeError;
    Error m_readError;
    QList<QPair<QVariant, qint64>> m_cachedValues;
    bool m_writingCachedValues;
    QList<QPair<QVariant, QDateTime>> m_values;

    bool initialize(const QJsonObject& object);
    bool writeValues(const QList<QPair<QVariant, qint64>>& list);
    bool writeCachedValues();
    bool readValues(const QDateTime& startTime = QDateTime(), const QDateTime& endTime = QDateTime());
    void addReadValues(const QJsonArray& values, bool notify = true);
    bool isReadyToWrite() const;
    bool isReadyToRead() const;

protected slots:

    virtual void onWriteFinished();
    virtual void onReadFinished();

protected:

    Q_DECLARE_PUBLIC(DataNode)
    DataNode* const q_ptr;
};

} // namespace iot

#endif // DATANODE_P_H
