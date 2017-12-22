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

#include "error.h"

namespace iot
{

Error::Error(QObject *parent) :
    QObject(parent),
    m_httpStatusCode(0),
    m_networkError(QNetworkReply::NoError),
    m_apiVersion(0),
    m_errorType(ErrorType::NoError),
    m_serverErrorCode(0)
{
}

Error::~Error()
{

}

QString Error::description() const
{
    return m_description;
}

void Error::setDescription(const QString& description)
{
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged(m_description);
    }
}

int Error::httpStatusCode() const
{
    return m_httpStatusCode;
}

void Error::setHttpStatusCode(int code)
{
    if (m_httpStatusCode != code) {
        m_httpStatusCode = code;
        emit codeChanged(m_httpStatusCode);
    }
}

QString Error::moreInfo() const
{
    return m_moreInfo;
}

void Error::setMoreInfo(const QString& moreInfo)
{
    if (m_moreInfo != moreInfo) {
        m_moreInfo = moreInfo;
        emit moreInfoChanged(m_moreInfo);
    }
}

int Error::apiVersion() const
{
    return m_apiVersion;
}

void Error::setApiVersion(int apiVersion)
{
    if (m_apiVersion != apiVersion) {
        m_apiVersion = apiVersion;
        emit apiVersionChanged(m_apiVersion);
    }
}

auto Error::errorType() const -> ErrorType
{
    return m_errorType;
}

void Error::setErrorType(ErrorType type)
{
    if (m_errorType != type) {
        m_errorType = type;
        emit errorTypeChanged(m_errorType);
    }
}

QNetworkReply::NetworkError Error::networkError() const
{
    return m_networkError;
}

void Error::setNetworkError(QNetworkReply::NetworkError networkError)
{
    if (m_networkError != networkError) {
        m_networkError = networkError;
        emit networkErrorChanged(m_networkError);
    }
}

int Error::serverErrorCode() const
{
    return m_serverErrorCode;
}

void Error::setServerErrorCode(int errorCode)
{
    if (m_serverErrorCode != errorCode) {
        m_serverErrorCode = errorCode;
        emit serverErrorCodeChanged(m_serverErrorCode);
    }
}

void Error::reset()
{
    setHttpStatusCode(-1);
    setNetworkError(QNetworkReply::NoError);
    setApiVersion(-1);
    setErrorType(ErrorType::NoError);
    setDescription(QString());
    setMoreInfo(QString());
}


} // namespace iot
