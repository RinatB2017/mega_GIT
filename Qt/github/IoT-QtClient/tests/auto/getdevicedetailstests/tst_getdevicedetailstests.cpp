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

#include "tst_testbase.h"

class GetDeviceDetailsTests : public TestBase
{
    Q_OBJECT
public:

    GetDeviceDetailsTests() : TestBase(),
        m_device(nullptr),
        m_expectedDevice(nullptr),
        m_getDetailsFinishedSpy(nullptr) {}

private slots:

    void init() {
        m_device = new Device();
        m_device->setDeviceId("123456789");
        m_expectedDevice = new Device();
        m_expectedDevice->setDeviceId("123456789");
        m_expectedDevice->setName("name");
        m_expectedDevice->setManufacturer("manufacturer");
        m_expectedDevice->setType("type");
        m_expectedDevice->setDescription("description");
        m_expectedDevice->setHref("http://address/");
        m_expectedDevice->setAttributes({ {"attrib1", "attrib1value"} });
        m_expectedDevice->setCreationTime(QDateTime::currentDateTime());
        m_getDetailsFinishedSpy = new QSignalSpy(m_device, SIGNAL(getDetailsFinished(bool)));
    }

    void cleanup() {
        delete m_device; m_device = nullptr;
        delete m_expectedDevice; m_expectedDevice = nullptr;
        delete m_getDetailsFinishedSpy; m_getDetailsFinishedSpy = nullptr;
    }

    void GetDeviceDetailsTestsWithDeviceId();
    void GetDeviceDetailsTestsWithDeviceIdAsGetParam();
    void GetDeviceDetailsTestsFailesWithNetworkError();
    void GetDeviceDetailsTestsFailesWithServerErrorResponse();
    void GetDeviceDetailsTestsFailesWithHttpStatus_400();
    void errorClearesAfterSuccess();

private:

    QJsonDocument getDeviceResponse() {
        QJsonArray attributesArray;
        for( auto key: m_expectedDevice->attributes().keys()) {
            attributesArray << QJsonObject( { {key, QJsonValue::fromVariant(m_expectedDevice->attributes()[key])} } );
        }

        QJsonObject obj
        {
            {"deviceId", m_expectedDevice->deviceId()},
            {"name", m_expectedDevice->name()},
            {"manufacturer", m_expectedDevice->manufacturer()},
            {"type", m_expectedDevice->type()},
            {"description", m_expectedDevice->description()},
            {"href", m_expectedDevice->href()},
            {"createdAt", m_expectedDevice->creationTime().toString(Qt::ISODate)},
            {"attributes", attributesArray}
        };

        return QJsonDocument(obj);
    }

    Device* m_device;
    Device* m_expectedDevice;
    QSignalSpy* m_getDetailsFinishedSpy;
};

void GetDeviceDetailsTests::GetDeviceDetailsTestsWithDeviceId()
{
    m_device->getDetails();

    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/" + m_expectedDevice->deviceId()), m_requestHandler->getRequestPath());

    simulateGetResponse(200, getDeviceResponse());
//    m_requestHandler->getRequestResponse()->offerResponse(200, getDeviceResponse());

    QCOMPARE(m_getDetailsFinishedSpy->count(), 1);
    QCOMPARE(m_getDetailsFinishedSpy->takeFirst().at(0).toBool(), true);
    QCOMPARE(*m_device, *m_expectedDevice);
}

void GetDeviceDetailsTests::GetDeviceDetailsTestsWithDeviceIdAsGetParam()
{
    m_device->getDetails("123");
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/123"), m_requestHandler->getRequestPath());
}

void GetDeviceDetailsTests::GetDeviceDetailsTestsFailesWithNetworkError()
{
    m_device->getDetails();

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_getDetailsFinishedSpy->count(), 1);
    QCOMPARE(m_getDetailsFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_device->getDetailsError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_device->getDetailsError()->networkError() == QNetworkReply::TimeoutError);
}

void GetDeviceDetailsTests::GetDeviceDetailsTestsFailesWithServerErrorResponse()
{

}

void GetDeviceDetailsTests::GetDeviceDetailsTestsFailesWithHttpStatus_400()
{

}

void GetDeviceDetailsTests::errorClearesAfterSuccess()
{
    m_device->getDetails();
    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_getDetailsFinishedSpy->count(), 1);
    QCOMPARE(m_getDetailsFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_device->getDetailsError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_device->getDetailsError()->networkError() == QNetworkReply::TimeoutError);

    m_getDetailsFinishedSpy->clear();
    m_device->getDetails();
    m_expectedDevice->setAttributes({ {"attrib1", "attrib1value"} });
    m_expectedDevice->setDeviceId("xxx");
    simulateGetResponse(200, getDeviceResponse());

    QCOMPARE(m_getDetailsFinishedSpy->count(), 1);
    QCOMPARE(m_getDetailsFinishedSpy->takeFirst().at(0).toBool(), true);
    QVERIFY(m_device->getDetailsError()->errorType() == Error::ErrorType::NoError);
    QVERIFY(m_device->getDetailsError()->networkError() == QNetworkReply::NoError);
    QCOMPARE(*m_device, *m_expectedDevice);
}

QTEST_MAIN(GetDeviceDetailsTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_getdevicedetailstests.moc"
