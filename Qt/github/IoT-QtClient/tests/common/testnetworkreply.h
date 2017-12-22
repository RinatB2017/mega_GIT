#ifndef TESTNETWORKREPLY_H
#define TESTNETWORKREPLY_H

#include <QJsonDocument>

#include "response.h"

#include <QDebug>

namespace iot
{

class TestResponse : public Response
{
public:

    TestResponse() : m_error(QNetworkReply::NoError){}

    void offerResponse(QNetworkReply::NetworkError error = QNetworkReply::NoError) { m_error = error; emit finished(); }
    void offerResponse(int httpStatusCode, const QJsonDocument& document) { m_httpStatusCode = httpStatusCode; setDocument(document); offerResponse(); }
    void offerResponse(int httpStatusCode) { m_httpStatusCode = httpStatusCode; m_document = QJsonDocument(); offerResponse(); }
    virtual QNetworkReply::NetworkError networkError() const override { return m_error; }
    virtual QJsonDocument document() const override { return m_document; }
    virtual int httpStatusCode() const override { return m_httpStatusCode; }
    void setDocument(const QJsonDocument& document) { m_document = document; }

private:

   QJsonDocument m_document;
   QNetworkReply::NetworkError m_error;
   int m_httpStatusCode;
};

} // namespace iot

#endif // TESTNETWORKREPLY_H
