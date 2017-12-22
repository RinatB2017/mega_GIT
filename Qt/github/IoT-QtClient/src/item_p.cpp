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

#include "item_p.h"
#include "response.h"
#include "error.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace iot
{

ItemPrivate::ItemPrivate() : QObject()
{

}

bool ItemPrivate::isValidResponse(const Response& response, Error& error, int expectedHttpStatusCode)
{
    bool okResponse = true;
    if (response.networkError() != QNetworkReply::NoError) {
        qDebug() << "Network error: " << response.networkError();
        okResponse = false;
        error.reset();
        error.setNetworkError(response.networkError());
        error.setErrorType(Error::ErrorType::NetworkError);
    }
    else if (response.httpStatusCode() != expectedHttpStatusCode) {
        qDebug() << "Not expected http status code: " << response.httpStatusCode();
        okResponse = false;
        error.reset();
        QJsonDocument responseDoc = response.document();
        if (responseDoc.isObject()) {
            QJsonObject errorObject = responseDoc.object();
            error.setDescription(errorObject.value("description").toString());
            error.setApiVersion(errorObject.value("apiver").toInt());
            error.setMoreInfo(errorObject.value("moreInfo").toString());
            error.setServerErrorCode(errorObject.value("code").toInt());
        }
        error.setHttpStatusCode(response.httpStatusCode());
        error.setErrorType(Error::ErrorType::HttpError);
    }

    return okResponse;
}

QJsonValue ItemPrivate::getValue(const QJsonObject& obj, const QString& key, QJsonValue::Type expectedType, bool mandatory) const
{
    QJsonValue val = obj.value(key);
    if (val.isUndefined()) {
        if (mandatory)
            throw JsonError(QString("Key '%1' not found from the document").arg(key));
        else
            qDebug() << QString("Key '%1' not found from the document").arg(key);
    } else if (val.type() != expectedType) {
            throw JsonError(QString("Key %1 value is not what was expected").arg(key));
    }

    return val;
}

QJsonValue ItemPrivate::getSingleValue(const QJsonObject& obj, const QString& key) const
{
    QJsonValue val = obj.value(key);
    if (val.isUndefined() || val.isNull()) {
        throw JsonError(QString("Key '%1' not found from the document").arg(key));
    } else if (val.type() == QJsonValue::Array) {
        throw JsonError(QString("Key '%1' value is not what was expected").arg(key));
    }

    return val;
}

QJsonObject ItemPrivate::getObject(const QJsonValue& value) const
{
    if (!value.isObject()) throw JsonError("JSonValue is not object");
    return value.toObject();
}

QJsonObject ItemPrivate::getObject(const QJsonDocument& doc) const
{
    if (!doc.isObject()) throw JsonError("Jsondocument it not object");
    return doc.object();
}

bool ItemPrivate::resolveKeyAndCallSetter(const QJsonObject& obj, const QString& key, std::function<void(const QString& val)> setterFunc)
{
    bool error = true;
    QJsonValue val = obj.value(key);
    if (!val.isUndefined()) {
        if (val.isString()) {
            setterFunc(val.toString());
            error = false;
        } else {
            qDebug() << QString("key: %1 wrong json value type").arg(key) << val;
        }
    } else {
        qDebug() << QString("key: %1 not found from the jsonobject").arg(key);
    }

    return error;
}

} // namespace iot

