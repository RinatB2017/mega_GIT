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

#include "requesthandler.h"
#include "response.h"

#include <QAuthenticator>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace iot
{

RequestHandler::RequestHandler(QObject* parent) : QObject(parent)
{
    m_serverUrl.setScheme("https");
    m_serverUrl.setHost("my.iot-ticket.com");
    m_serverUrl.setPath("/api/v1");

//    m_networkaccessmanager.connectToHost(m_serverUrl.toString());

    connect( &m_networkaccessmanager, SIGNAL(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator*)),
             this, SLOT(onProxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)));
}

void RequestHandler::configureRequest(QNetworkRequest& request, const QString& path)
{
    const QUrl url(m_serverUrl.toString() + path);
    request.setUrl(url);
    request.setRawHeader("Authorization", "Basic " +
                          QByteArray(QString("%1:%2").arg(m_userName).arg(m_password).toLatin1()).toBase64());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=UTF-8");

    //ignore certificate verify
    QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(conf);
}

RequestHandler::~RequestHandler()
{

}

Response* RequestHandler::getRequest(const QString& path)
{
    QNetworkRequest request;
    configureRequest(request, path);
    QNetworkReply* reply = m_networkaccessmanager.get(request);
    return new Response(reply);
}

Response* RequestHandler::postRequest(const QString& path, const QByteArray& data)
{
    QNetworkRequest request;
    configureRequest(request, path);
    QNetworkReply* reply = m_networkaccessmanager.post(request, data);
    return new Response(reply);
}

QUrl RequestHandler::serverUrl() const
{
    return m_serverUrl;
}

void RequestHandler::setServerUrl(const QUrl& serverUrl)
{
    if (m_serverUrl != serverUrl) {
        m_serverUrl = serverUrl;
        emit serverUrlChanged(m_serverUrl);
    }
}

QString RequestHandler::userName() const
{
    return m_userName;
}

void RequestHandler::setUserName(const QString& userName)
{
    if (m_userName != userName) {
        m_userName = userName;
        emit userNameChanged(m_userName);
    }
}
QString RequestHandler::password() const
{
    return m_password;
}

void RequestHandler::setPassword(const QString& password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged(m_password);
    }
}

void RequestHandler::setProxy(const QNetworkProxy& proxy)
{
    if (m_networkaccessmanager.proxy() != proxy) {
        m_networkaccessmanager.setProxy(proxy);
        emit proxyChanged(proxy);
    }
}

QNetworkProxy RequestHandler::proxy() const
{
    return m_networkaccessmanager.proxy();
}

void RequestHandler::onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
    // TODO. need to test that this works. Also SSL needs to be tested.
    Q_UNUSED(proxy);
    authenticator->setUser(m_userName);
    authenticator->setPassword(m_password);
}

} // namespace iot

