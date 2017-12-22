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

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "iotlib_global.h"

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>

class QNetworkProxy;
class QAuthenticator;

namespace iot
{

class Response;

class IOTLIBSHARED_EXPORT RequestHandler : public QObject
{
    Q_OBJECT
public:

    friend class RequestHandlerProvider;

    virtual Response* getRequest(const QString& path);
    virtual Response* postRequest(const QString& path, const QByteArray& data);

    QUrl serverUrl() const;
    void setServerUrl(const QUrl& serverUrl);

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    void setProxy(const QNetworkProxy& proxy);
    QNetworkProxy proxy() const;

signals:

    void serverUrlChanged(const QUrl& serverUrl);
    void passwordChanged(const QString& password);
    void userNameChanged(const QString& userName);
    void proxyChanged(const QNetworkProxy& proxy);

protected slots:

    virtual void onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);

protected:

    // Only provider should be used to create instances from this class.
    RequestHandler(QObject* parent = 0);
    virtual ~RequestHandler();

    void configureRequest(QNetworkRequest& request, const QString& path);

    QNetworkAccessManager   m_networkaccessmanager;

    QUrl                    m_serverUrl;

    QString                 m_userName;
    QString                 m_password;
    QString                 m_deviceid;

};

} // namespace iot

#endif // REQUESTHANDLER_H
