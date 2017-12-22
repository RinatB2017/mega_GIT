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

#ifndef DEVICE_H
#define DEVICE_H

#include "iotlib_global.h"

#include <QObject>
#include <QHash>
#include <QDateTime>
#include <QVariant>

namespace iot
{

// forward declaration
class DevicePrivate;
class Error;
class DataNode;

//! \brief The Device class presents a root node on the Iot-Ticket server. DataNodes reference to devices they belong to.
//!
//! Device needs to be registered to the Iot-Ticket server before datanodes can be added to it.
//! Device can be used to fetch registered datanodes. Device also offers API to read and write values
//! from datanodes in one request to the Iot-Ticket server.
//!

class IOTLIBSHARED_EXPORT Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QVariantMap attributes READ attributes WRITE setAttributes NOTIFY attributesChanged)
    Q_PROPERTY(QDateTime creationTime READ creationTime WRITE setCreationTime NOTIFY creationTimeChanged)
    Q_PROPERTY(QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(QString href READ href NOTIFY hrefChanged)
    Q_PROPERTY(Error* registerError READ registerError NOTIFY registerErrorChanged)
    Q_PROPERTY(Error* getDetailsError READ getDetailsError NOTIFY getDetailsErrorChanged)
public:

    //! \brief Constructor.
    //! \param parent Parent qobject, passed to QObject constructor.
    explicit Device(QObject *parent = 0);

    //! \brief Destructor.
    virtual ~Device();

    //! \brief Return device name.
    //! \return Name.
    QString name() const;

    //! \brief Return device manufacturer.
    //! \return Manufacturer.
    QString manufacturer() const;

    //! \brief Return device type.
    //! \return Type.
    QString type() const;

    //! \brief Return device description.
    //! \return Description.
    QString description() const;

    //! \brief Return device id.
    //! \return DeviceId.
    QString deviceId() const;

    //! \brief Return device href.
    //! \return Device href.
    QString href() const;

    //! \brief Return device attributes.
    //! \return Attributes.
    QVariantMap attributes() const;

    //! \brief Return device creation time.
    //! \return Creation time.
    QDateTime creationTime() const;

    //! \brief Set device creation time.
    //! \param creationTime Creation time.
    void setCreationTime(const QDateTime& creationTime);

    //! \brief Return device datanodes.
    //! \return list of datanodes.
    QList<DataNode*> dataNodes() const;

    //! \brief Return details from creation error.
    //! \return Error details.
    Error* registerError();

    //! \brief Return details from get error.
    //! \return Error details.
    Error* getDetailsError();

    //! \brief Return details from get datanodes error.
    //! \return Error details.
    Error* getDataNodesError();

    //! \brief Add datanode to the device. Ownership is transferred to the device.
    //! \param node DataNode.
    void addDataNode(DataNode* node);

    //! \brief Add list of datanodes to the device. Ownerships are transferred to the device.
    //! \param nodes List of dataNodes.
    void addDataNodes(QList<DataNode*> nodes);

    //! \brief Removes datanode from the device. Node is not destructed. Ownership of the object is removed from the device.
    //! \param node Removed dataNode.
    void removeDataNode(DataNode* node);

    //! \brief Removes list of datanodes from the device. Nodes are not destructed. Ownerships of the objects are removed from the device.
    //! \param nodes Removed dataNode.
    void removeDataNodes(QList<DataNode*> nodes);

    //! \brief Read latest process values from list of datanodes.
    //! \param nodes If argument is empty list latest values are read from all registered datanodes.
    void readDataNodesLatestValue(QList<DataNode*> nodes = QList<DataNode*>());

    //! \brief Read process values for time range from list of datanodes.
    //! \param startTime Start time for the range.
    //! \param endTime End time for the range.
    //! \param nodes If argument is empty list latest values are read from all registered datanodes.
    void readDataNodeValues(const QDateTime& startTime, const QDateTime& endTime, QList<DataNode*> nodes = QList<DataNode*>());

    //! \brief Read process values from specified start time to current time from list of datanodes.
    //! \param startTime Start time for the range.
    //! \param endTime End time for the range.
    //! \param nodes If argument is empty list latest values are read from all registered datanodes.
    void readDataNodeValues(const QDateTime& startTime, QList<DataNode*> nodes = QList<DataNode*>());

    //! \brief Equality operator.
    //! \return Are device properties identical.
    bool operator ==(const Device& other) const;

    //! \brief Inequality operator.
    //! \return Are device properties inequal.
    bool operator !=(const Device& other) const { return !operator==(other); }

public slots:

    //! \brief Set device name.
    //! \param name Name.
    void setName(const QString& name);

    //! \brief Set device manufacturer.
    //! \param manufacturer Manufacturer.
    void setManufacturer(const QString& manufacturer);

    //! \brief Set device type.
    //! \param type Type.
    void setType(const QString& type);

    //! \brief Set device description.
    //! \param description Description.
    void setDescription(const QString& description);

    //! \brief Set device id.
    //! \param deviceId DeviceId.
    void setDeviceId(const QString& deviceId);

    //! \brief Set device href.
    //! \param href Href.
    void setHref(const QString& href);

    //! \brief Set device attributes.
    //! \param attributes Attributes.
    void setAttributes(const QVariantMap& attributes);

    //! \brief Register device.
    //!
    //! This is async operation. registerFinished() is emitted when registration is complete.
    //! Device has several attributes that can be set to identify the device. Once IotTicket server
    //! registers the device, device id is returned to the client and is accessible via deviceId() function.
    //! Datanodes required device id when they read and write process values.
    //!
    //! \code
    //! iot::Device device;
    //! device.setManufacturer("...");
    //! device.setType("...");
    //! device.setName("...");
    //! device.setDescription("...");
    //! QVariantMap attributes;
    //! device.setAttributes(attributes);
    //! device.registerDevice();
    //! \endcode
    void registerDevice();

    //! \brief Get device details that were provided when device was registered.
    //! This is async operation. getDetailsFinished() is emitted when operation is complete.
    //! \param deviceId If empty string is provided value provided via setDeviceId is used.
    //!
    //! \code
    //! iot::Device device;
    //! device.setDeviceId("...");
    //! device.getDetails();
    //! \endcode
    void getDetails(const QString& deviceId = QString());

    //! \brief Get device data nodes.
    //! This is async operation. getDataNodesFinished() is emitted when operation is complete.
    //! Once data nodes are retrieved from IotTicket server they can be accessed via dataNodes() function.
    //! \param deviceId If empty string is provided value provided via setDeviceId is used.
    //!
    //! \code
    //! iot::Device device;
    //! device.setDeviceId("...");
    //! device.getDataNodes();
    //! // Access datanodes once operation is complete.
    //! QList<iot::DataNode*> dataNodes = device.dataNodes();
    //! \endcode
    void getDataNodes(const QString& deviceId = QString());

signals:
    void nameChanged(const QString& name);
    void manufacturerChanged(const QString& manufacturer);
    void descriptionChanged(const QString& description);
    void attributesChanged(const QVariantMap& attributes);
    void typeChanged(const QString& type);
    void deviceIdChanged(const QString& deficeId);
    void hrefChanged(const QString& href);
    void creationTimeChanged(const QDateTime& creationTime);
    void registerErrorChanged(Error* error);
    void getDetailsErrorChanged(Error* error);
    void getDataNodesErrorChanged(Error* error);
    void registerFinished(bool success);
    void getDetailsFinished(bool success);
    void getDataNodesFinished(bool success);
    void readDataNodeValuesFinished(bool success);

protected:

    QScopedPointer<DevicePrivate> const d_ptr;

    Device(DevicePrivate& dd, QObject* parent = 0);

private:

    friend class DeviceListPrivate;

    static Device* fromJson(const QJsonObject& object);

    Q_DECLARE_PRIVATE(Device);
    friend IOTLIBSHARED_EXPORT QDebug operator<<(QDebug, const Device&);
};

} // namespace iot

#endif // DEVICE_H
