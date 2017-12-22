#ifndef TST_TESTBASE_H
#define TST_TESTBASE_H

// Includes that all sub tests require
#include "testrequesthandlerprovider.h"
#include "testrequesthandler.h"
#include "testnetworkreply.h"
#include "error.h"
#include "device.h"
#include "datanode.h"

#include <QString>
#include <QVariant>
#include <QtTest>
#include <QSignalSpy>
#include <QPointer>
#include <QObject>

using namespace iot;

class TestBase : public QObject
{
    Q_OBJECT

public:
    TestBase() : m_requestHandler(nullptr)
    {
    }

private slots:

    // initTestCase and cleanupTestCase are run before test group is run
    void initTestCase()
    {
        // Setup SUT ready for testing. We use stub that does not really
        // make network requests instead we simulate responses etc.
        m_requestHandler = new TestRequestHandler;
        TestRequestHandlerProvider::overrideRequestHandler(m_requestHandler);
    }
    void cleanupTestCase()
    {
        delete m_requestHandler;
    }

protected:

    void simulateGetResponse(QNetworkReply::NetworkError error) { m_requestHandler->getRequestResponse()->offerResponse(error); }
    void simulateGetResponse(int httpStatusCode, const QJsonDocument& doc) { m_requestHandler->getRequestResponse()->offerResponse(httpStatusCode, doc); }
    void simulateGetResponse(int httpStatusCode) { m_requestHandler->getRequestResponse()->offerResponse(httpStatusCode, QJsonDocument()); }

    TestRequestHandler* m_requestHandler;

};

#endif // TST_TESTBASE_H
