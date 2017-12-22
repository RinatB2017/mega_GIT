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

#include "response.h"

#include <QJsonDocument>

namespace iot
{

Response::Response(QNetworkReply* reply, QObject *parent) :
    QObject(parent), m_reply(reply)
{
    QObject::connect( reply, &QNetworkReply::finished, this, &Response::finished);
}

Response::Response(QObject *parent) : QObject(parent), m_reply(0)
{

}

QNetworkReply::NetworkError Response::networkError() const
{
    Q_ASSERT(m_reply != nullptr);
    QNetworkReply::NetworkError error = QNetworkReply::NoError;
    if (m_reply) {
        error = m_reply->error();
    } else {
        // Review, mky, 20160329: This will never get printed since with debug build the above assert has already crashed the app
        qDebug() << "Unable to get network error. Reply pointer not valid";
    }

    return error;
}

int Response::httpStatusCode() const
{
    Q_ASSERT(m_reply != nullptr);
    int httpResponseCode = -1;
    if (m_reply) {
        const QVariant statusCodeVariant = m_reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        if (statusCodeVariant.isValid()) {
            httpResponseCode = statusCodeVariant.toInt();
        } // Review, mky, 20160329: if statusCodeVariant can be invalid this should probably be logged or handled
    } else {
        // Review, mky, 20160329: This will never get printed since with debug build the above assert has already crashed the app
        qDebug() << "Unable to get http status code. Reply pointer not valid";
    }

    return httpResponseCode;
}

QJsonDocument Response::document() const
{
    QJsonDocument doc;
    if (networkError() == QNetworkReply::NoError) {
        if (m_reply) {
            const QByteArray data = m_reply->readAll();
            doc = QJsonDocument::fromJson(data);
        } else {
            qDebug() << "Unable to read reply document. Reply pointer not valid";
        }
    }

    return doc;
}

} // namespace iot
