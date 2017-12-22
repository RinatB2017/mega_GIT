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

#ifndef CONNECTION_H
#define CONNECTION_H

#include "iotlib_global.h"

#include <QObject>
#include <QUrl>
#include <QNetworkProxy>

namespace iot
{

// Forward declaration
class ConnectionPrivate;

//! \brief The Connection class is used to configure Iot-Ticket server connection.
//!
//! Connection is a singleton. Authentication credentials are set
//! via setUserName() and setPassword(). Usually there is no need to touch the serverUrl.
//!
//! Example:
//! \code
//! iot::Connection* connection = iot::Connection::instance();
//! connection->setUserName("foo");
//! connection->setPassword("bar);
//! // Now connection is configured and ready to be used.
//! \endcode
//!
class IOTLIBSHARED_EXPORT Connection : public QObject
{
    Q_OBJECT    
    Q_PROPERTY( QUrl serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY( QString username READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY( QNetworkProxy proxy READ proxy WRITE setProxy NOTIFY proxyChanged)

public:

    //! \brief Getter for singleton instance.
    //! \return Pointer to singleton instance. Ownership not transferred.
    static Connection* instance();

    //! \brief Returns Iot-Ticket server url.
    //! \return Iot-Ticket server url.
    QUrl serverUrl() const;

    //! \brief Set Iot-Ticket server url.
    //! \param url Iot-Ticket server url.
    void setServerUrl(const QUrl& url);

    //! \brief Returns user name that is used with the Iot-Ticket server communication.
    //! \return User name.
    QString userName() const;

    //! \brief Set user name that is used with the Iot-Ticket server communication.
    //! \param userName User name.
    void setUserName(const QString& userName);

    //! \brief Returns password that is used with the Iot-Ticket server communication.
    //! \return Password.
    QString password() const;

    //! \brief Set password that is used with the Iot-Ticket server communication.
    //! \param password Password.
    void setPassword(const QString& password);

    //! \brief Returns the networkproxy that is used with the Iot-Ticket server communication.
    //! \return networkproxy.
    QNetworkProxy proxy() const;

    //! \brief Set the networkproxy that is used with the Iot-Ticket server communication.
    //! \param networkProxy NetworkProxy.
    void setProxy(const QNetworkProxy& networkProxy);

signals:

    //! \brief Emitted when serverUrl changes.
    void serverUrlChanged(const QUrl& url);
    //! \brief Emitted when userName changes.
    void userNameChanged(const QString& userName);
    //! \brief Emitted when password changes.
    void passwordChanged(const QString& password);
    //! \brief Emitted when proxy changes.
    void proxyChanged(const QNetworkProxy& proxy);

protected:

    QScopedPointer<ConnectionPrivate> const d_ptr;

    Connection();
    Connection(ConnectionPrivate& dd);
    virtual ~Connection();

private:

    Q_DECLARE_PRIVATE(Connection);

};

} // namespace iot

#endif // CONNECTION_H
