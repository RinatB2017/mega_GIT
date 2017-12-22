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

#ifndef ERROR_H
#define ERROR_H

#include "iotlib_global.h"

#include <QObject>
#include <QNetworkReply>

namespace iot
{

class IOTLIBSHARED_EXPORT Error : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(int httpStatusCode READ httpStatusCode NOTIFY codeChanged)
    Q_PROPERTY(QNetworkReply::NetworkError networkError READ networkError NOTIFY networkErrorChanged)
    Q_PROPERTY(QString moreInfo READ moreInfo NOTIFY moreInfoChanged)
    Q_PROPERTY(int apiVersion READ apiVersion NOTIFY apiVersionChanged)
    Q_PROPERTY(ErrorType errorType READ errorType NOTIFY errorTypeChanged)
    Q_PROPERTY(int serverErrorCode READ serverErrorCode NOTIFY serverErrorCodeChanged)
    Q_ENUMS(ErrorType)
public:

    enum class ErrorType { NoError, NetworkError, HttpError, GenericError };

    explicit Error(QObject *parent = 0);
    virtual ~Error();

    QString description() const;
    void setDescription(const QString& description);
    int httpStatusCode() const;
    void setHttpStatusCode(int httpStatusCode);
    QString moreInfo() const;
    void setMoreInfo(const QString& moreInfo);
    int apiVersion() const;
    void setApiVersion(int apiVersion);
    ErrorType errorType() const;
    void setErrorType(ErrorType type);
    QNetworkReply::NetworkError networkError() const;
    void setNetworkError(QNetworkReply::NetworkError networkError);
    int serverErrorCode() const;
    void setServerErrorCode(int errorCode);

signals:

    void descriptionChanged(const QString& description);
    void codeChanged(int httpStatusCode);
    void networkErrorChanged(QNetworkReply::NetworkError error);
    void moreInfoChanged(const QString& moreInfo);
    void apiVersionChanged(int apiVersion);
    void errorTypeChanged(ErrorType type);
    void serverErrorCodeChanged(int errorCode);

public slots:

    void reset();

private:

    QString m_description;
    int m_httpStatusCode;
    QNetworkReply::NetworkError m_networkError;
    QString m_moreInfo;
    int m_apiVersion;
    ErrorType m_errorType;
    int m_serverErrorCode;
};

} // namespace iot

#endif // ERROR_H
