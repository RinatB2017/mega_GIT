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

#include "connection.h"
#include "connection_p.h"

namespace iot
{

Connection::Connection() : d_ptr(new ConnectionPrivate(this))
{
    Q_D(Connection);
    connect(d->m_requestHandler, &RequestHandler::serverUrlChanged, this, &Connection::serverUrlChanged);
    connect(d->m_requestHandler, &RequestHandler::passwordChanged, this, &Connection::passwordChanged);
    connect(d->m_requestHandler, &RequestHandler::userNameChanged, this, &Connection::userNameChanged);
    connect(d->m_requestHandler, &RequestHandler::proxyChanged, this, &Connection::proxyChanged);
}

Connection::Connection(ConnectionPrivate& dd) : d_ptr(&dd)
{
    Q_D(Connection);
    connect(d->m_requestHandler, &RequestHandler::serverUrlChanged, this, &Connection::serverUrlChanged);
    connect(d->m_requestHandler, &RequestHandler::passwordChanged, this, &Connection::passwordChanged);
    connect(d->m_requestHandler, &RequestHandler::userNameChanged, this, &Connection::userNameChanged);
    connect(d->m_requestHandler, &RequestHandler::proxyChanged, this, &Connection::proxyChanged);
}

Connection::~Connection()
{

}

Connection* Connection::instance()
{
    return ConnectionPrivate::connectionInstance();
}

QUrl Connection::serverUrl() const
{
    Q_D(const Connection);
    return d->m_requestHandler->serverUrl();
}

void Connection::setServerUrl(const QUrl& url)
{
    Q_D(Connection);
    d->m_requestHandler->setServerUrl(url);
}

QString Connection::userName() const
{
    Q_D(const Connection);
    return d->m_requestHandler->userName();
}

void Connection::setUserName(const QString& userName)
{
    Q_D(Connection);
    d->m_requestHandler->setUserName(userName);
}

QString Connection::password() const
{
    Q_D(const Connection);
    return d->m_requestHandler->password();
}

void Connection::setPassword(const QString& password)
{
    Q_D(Connection);
    d->m_requestHandler->setPassword(password);
}

QNetworkProxy Connection::proxy() const
{
    Q_D(const Connection);
    return d->m_requestHandler->proxy();
}

void Connection::setProxy(const QNetworkProxy& networkProxy)
{
    Q_D(Connection);
    d->m_requestHandler->setProxy(networkProxy);
}

} // namespace iot
