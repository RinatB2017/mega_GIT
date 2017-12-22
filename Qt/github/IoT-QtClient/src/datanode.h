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

#ifndef DATANODE_H
#define DATANODE_H

#include "iotlib_global.h"

#include <QObject>

namespace iot
{

// forward declarations
class DataNodePrivate;
class Error;

//! \brief The DataNode class can be used to read/write device data from/to Iot-Ticket server.
//!
//! DataNode can read or write one patch of values at a time. DataNode is registered to the Iot-Ticket server
//! on the first time values are written.
class IOTLIBSHARED_EXPORT DataNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(DataType dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(QString href READ href NOTIFY hrefChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(bool readyToRead READ isReadyToRead NOTIFY readyToReadChanged)
    Q_PROPERTY(bool readyToWrite READ isReadyToWrite NOTIFY readyToWriteChanged)

public:

    enum DataType {
        Double,
        Long,
        String,
        Boolean,
        Binary,
        Unkown
    };

    Q_ENUM(DataType)

    //! \brief Constructor.
    //! \param parent Parent qobject, passed to QObject constructor.
    explicit DataNode(QObject *parent = 0);

    //! \brief Destructor.
    virtual ~DataNode();

    //! \brief Return datanode's name.
    //! \return Name.
    QString name() const;

    //! \brief Return datanode's unit.
    //! \return Unit.
    QString unit() const;

    //! \brief Return datanode's data type.
    //! \return DataType.
    DataType dataType() const;

    //! \brief Return datanode's href that can be used to read process data from the Iot-Ticket server.
    //! \return Href.
    QString href() const;

    //! \brief Return datanode's path.
    //! \return Path.
    QString path() const;

    //! \brief Return datanode's deviceId.
    //! \return DeviceId.
    QString deviceId() const;

    //! \brief Return list of process values that have been read from the Iot-Ticket server.
    //! \return Process values.
    QList< QPair<QVariant, QDateTime> > values() const;

    //! \brief Return latest process value that have been read from the Iot-Ticket server.
    //! \return Latest process value.
    QPair<QVariant, QDateTime> latestValue() const;

    //! \brief Write process value to the Iot-Ticket server.
    //!
    //! This is async operation. writeFinished(bool) will be emitted once operation is done.
    //! DataNode needs to be configured before values can be written via setDeviceId(), setPath() and setName(). DataNode is created
    //! on the first time values are written to the IotTicket server. So on the first write unit should be set too.
    //!
    //! \param val Value that will be written.
    //! \param cache Whether value will be cached to internal buffer or written immediately.
    //! \return If value is written immediately returns true if writing starts (datanode is ready to write). If values is cached returns always true.
    bool writeValue(const QVariant& val, bool cache = false);

    //! \brief Write process value and its timestamp to the Iot-Ticket server.
    //!
    //! This is async operation. writeFinished(bool) will be emitted once operation is done.
    //! DataNode needs to be configured before values can be written via setDeviceId(), setPath() and setName(). DataNode is created
    //! on the first time values are written to the IotTicket server. So on the first write unit should be set too.
    //!
    //! \param val Value that will be written.
    //! \param cache Whether value will be cached to internal buffer or written immediately.
    //! \return If value is written immediately returns true if writing starts (datanode is ready to write). If value is cached returns always true.
    bool writeValue(const QPair<QVariant, qint64>& val, bool cache = false);

    //! \brief Write list of process values to the Iot-Ticket server.
    //!
    //! This is async operation. writeFinished(bool) will be emitted once operation is done.
    //! DataNode needs to be configured before values can be written via setDeviceId(), setPath() and setName(). DataNode is created
    //! on the first time values are written to the IotTicket server. So on the first write unit should be set too.
    //!
    //! \param list Values that will be written.
    //! \param cache Whether values will be cached to internal buffer or written immediately.
    //! \return If values are written immediately returns true if writing starts (datanode is ready to write). If values are cached returns always true.
    bool writeValues(const QVariantList& list, bool cache = false);

    //! \brief Write list of process values and timestamps to the Iot-Ticket server.
    //!
    //! This is async operation. writeFinished(bool) will be emitted once operation is done.
    //! DataNode needs to be configured before values can be written via setDeviceId(), setPath() and setName(). DataNode is created
    //! on the first time values are written to the IotTicket server. So on the first write unit should be set too.
    //!
    //! \param list Values that will be written.
    //! \param cache Whether values will be cached to internal buffer or written immediately.
    //! \return If values are written immediately returns true if writing starts (datanode is ready to write). If values are cached returns always true.
    bool writeValues(const QList< QPair<QVariant, qint64> >& list, bool cache = false);

    //! \brief Write values in the internal buffer to the Iot-Ticket server.
    //!
    //! This is async operation. writeFinished(bool) will be emitted once operation is done.
    //! DataNode needs to be configured before values can be written via setDeviceId(), setPath() and setName(). DataNode is created
    //! on the first time values are written to the IotTicket server. So on the first write unit should be set too.
    //!
    //! \return True if writing starts (datanode is ready to write isReadyToWrite())
    //!
    //! \code
    //! iot::DataNode node;
    //! node.setDeviceId("deviceId");
    //! node.setPath("engine";
    //! node.setName("fanspeed");
    //! node.setUnit("rpm");
    //! node->writeValue(1, true);
    //! node->writeValue(2, true);
    //! // Values in cache (1 and 2) are send to the IotTicket server
    //! node->writeCachedValues();
    //! \endcode
    bool writeCachedValues();

    //! \brief Clears internal buffer.
    void clearCachedValues();

    //! \brief Return list of values currently in the internal buffer.
    //! \return List of values.
    QList< QPair<QVariant, qint64> >& cachedValues();

    //! \brief Read latest process value from the Iot-Ticket server.
    //!
    //! This is async operation. readFinished() will be emitted once operation is done.
    //!
    //! \return True if reading starts (datanode is ready to read).
    bool readLatestValue();

    //! \brief Read process values from specified time range from the Iot-Ticket server.
    //!
    //! This is async operation. readFinished() will be emitted once operation is done.
    //!
    //! \param startTime Start time for the range.
    //! \param endTime End time for the range.
    //! \return True if reading starts (datanode is ready to read).
    bool readValues(const QDateTime& startTime, const QDateTime& endTime);

    //! \brief Read process values from specified start time to current time from the Iot-Ticket server.
    //!
    //! This is async operation. readFinished() will be emitted once operation is done.
    //!
    //! \param startTime Start time for the range.
    //! \return True if reading starts (datanode is ready to read).
    bool readValues(const QDateTime& startTime);

    //! \brief Is datanode ready to read values from IotTicket server. DataNode can read only a one patch of values at a time.
    //! \return True if ready to read.
    bool isReadyToRead() const;

    //! \brief Is datanode ready to send values to the IotTicket server. DataNode can write only a one patch of values at a time.
    //! \return True if ready to write.
    bool isReadyToWrite() const;

    //! \brief Return details from the latest write operation error.
    //! \return Error details.
    Error* writeError();

    //! \brief Return details from the latest read operation error.
    //! \return Error details.
    Error* readError();

    //! \brief Equality operator.
    //! \return Are datanode properties identical.
    bool operator ==(const DataNode& other) const;

    //! \brief Inequality operator.
    //! \return Are datanode properties inequal.
    bool operator !=(const DataNode& other) const { return !operator==(other); }

public slots:

    //! \brief Set datanode name.
    //! \param name Name.
    void setName(const QString& name);

    //! \brief Set datanode name.
    //! \param unit Unit.
    void setUnit(const QString& unit);

    //! \brief Set datanode's data type.
    //!
    //! Not a mandatory attribute. IotTicket server tries to resolve the data type automatically from the
    //! written values if not set.
    //!
    //! \param dataType DataType.
    void setDataType(DataType dataType);

    //! \brief Set datanode href that can be used to read process data from the Iot-Ticket server.
    //! \param href Href.
    void setHref(const QString& href);

    //! \brief Set datanode path.
    //! \param path Path.
    void setPath(const QString& path);

    //! \brief Set deviceId.
    //! \param deviceId DeviceId.
    void setDeviceId(const QString& deviceId);

signals:

    void nameChanged(QString name);
    void unitChanged(QString unit);
    void dataTypeChanged(DataType dataType);
    void hrefChanged(QString href);
    void pathChanged(QString path);
    void deviceIdChanged(QString deviceId);
    void writeFinished(bool success);
    void readFinished(bool success);
    void readyToReadChanged(bool ready);
    void readyToWriteChanged(bool ready);

protected:

    QScopedPointer<DataNodePrivate> const d_ptr;

    DataNode(DataNodePrivate& dd, QObject* parent = 0);

private:

    friend class DevicePrivate;

    static DataNode* fromJson(const QJsonObject& object);

    void addReadValues(const QJsonArray& values);

    Q_DECLARE_PRIVATE(DataNode);
    friend IOTLIBSHARED_EXPORT QDebug operator<<(QDebug, const DataNode&);

};

} // namespace iot

#endif // DATANODE_H
