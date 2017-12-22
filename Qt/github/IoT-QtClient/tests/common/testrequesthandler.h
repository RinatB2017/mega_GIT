#ifndef TESTREQUESTHANDLER_H
#define TESTREQUESTHANDLER_H

#include "requesthandler.h"
#include "testnetworkreply.h"

#include <QString>
#include <QPointer>
#include <QByteArray>

namespace iot
{

class TestRequestHandler : public RequestHandler
{
public:

    Response* getRequest(const QString& path) override {
        m_getRequestPath = path;
        m_getReply = new TestResponse();
        return m_getReply;
    }

    Response* postRequest(const QString& path, const QByteArray& data) override {
        m_postRequestPath = path;
        m_postData = data;
        m_postReply = new TestResponse();
        return m_postReply;
    }

    TestResponse* getRequestResponse() {
        return m_getReply;
    }

    TestResponse* postRequestResponse() {
        return m_postReply;
    }

    QString getRequestPath() const { return m_getRequestPath; }
    QString postRequestPath() const { return m_postRequestPath; }
    QByteArray postData() const { return m_postData; }

private:

    QPointer<TestResponse> m_getReply;
    QPointer<TestResponse> m_postReply;
    QByteArray m_postData;
    QString m_getRequestPath;
    QString m_postRequestPath;

};


} // namespace iot

#endif // TESTREQUESTHANDLER_H
