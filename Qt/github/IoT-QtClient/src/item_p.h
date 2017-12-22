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

#ifndef ITEM_P_H
#define ITEM_P_H

#include <functional>

#include <QObject>
#include <QJsonValue>
#include <exception>

namespace iot
{

class Response;
class Error;

class ItemPrivate : public QObject
{
public:
    ItemPrivate();

protected:

    bool isValidResponse(const Response& response, Error& error, int expectedHttpStatusCode);

    bool resolveKeyAndCallSetter(const QJsonObject& obj, const QString& key, std::function<void(const QString& val)> setterFunc);

    QJsonValue getValue(const QJsonObject& obj, const QString& key, QJsonValue::Type expectedType, bool mandatory = true) const;
    QJsonValue getSingleValue(const QJsonObject& obj, const QString& key) const;
    QJsonObject getObject(const QJsonValue& value) const;
    QJsonObject getObject(const QJsonDocument& doc) const;

};

class GenericError : public std::exception
{
public:
    GenericError(const QString& description = QString()) : exception() , m_description(description) {}

    virtual const char* what() const throw()
    {
        return m_description.toLatin1().data();
    }

private:
    QString m_description;
};

class JsonError : public GenericError
{
public:
    JsonError(const QString& description = QString()) : GenericError("Invalid Json document received. " + description) {}
};

} // namespace iot

#endif // ITEM_P_H
