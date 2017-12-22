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

#ifndef DEVICELIST_H
#define DEVICELIST_H

#include "iotlib_global.h"

#include <QObject>

namespace iot
{

// forward declaration
class DeviceListPrivate;
class Device;
class Error;

//! \brief The DeviceList class can be used to fetch list of devices from the Iot-Ticket server.
//!
//! get() function starts the async operation to fetch the list of devices. getFinished() is
//! emitted once the operaton completes and after that devices() function can be used to access
//! the fetched devices.
//!
//! Fetched devices are configured properly, so those object are immediately ready to be used
//! for example to fetching device datanodes.
//!
class IOTLIBSHARED_EXPORT DeviceList : public QObject
{
    Q_OBJECT
public:

    //! \brief Constructor.
    //! \param parent Parent qobject, passed to QObject constructor.
    explicit DeviceList(QObject *parent = 0);

    //! \brief Destructor.
    virtual ~DeviceList();

    //! \brief Return list of devices.
    //!
    //! Before device list is valid, devices needs to be fetched from the server using get() function.
    //!
    //! \return Device list.
    QList<Device*> devices();

    //! \brief Take devices list.
    //!
    //! Ownership is transferred and DeviceList' devices list is cleared.
    //!
    //! \return Device list.
    QList<Device*> takeDevices();

    //! \brief Return details from get error.
    //! \return Error details.
    Error* getError();

signals:

    //! \brief Signal is emitted when async get operation completes.
    //! \param success True if operation finished successfully.
    void getFinished(bool success);

public slots:

    //! \brief Get devices.
    //! This is async operation. Signal getFinished() is emitted when operation completes.
    //! devices() function can be used to access the
    void get();

protected:

    QScopedPointer<DeviceListPrivate> const d_ptr;
    DeviceList(DeviceListPrivate& dd, QObject* parent = 0);

private:

    Q_DECLARE_PRIVATE(DeviceList);
    friend IOTLIBSHARED_EXPORT QDebug operator<<(QDebug, const DeviceList&);

};

} // namespace iot

#endif // DEVICELIST_H
