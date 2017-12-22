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

class RegisterDeviceTests : public TestBase
{
    Q_OBJECT
public:

    RegisterDeviceTests() : TestBase(),
        m_device(nullptr),
        m_registerFinishedSpy(nullptr),
        m_creationTimeSpy(nullptr),
        m_deviceIdSpy(nullptr){}

private slots:

    void init() {
        m_device = new Device();
        m_device->setName("name");
        m_device->setManufacturer("manufacturer");
        m_device->setType("type");
        m_device->setDescription("description");
        m_registerFinishedSpy = new QSignalSpy(m_device, SIGNAL(registerFinished(bool)));
        m_creationTimeSpy = new QSignalSpy(m_device, SIGNAL(creationTimeChanged(QDateTime)));
        m_deviceIdSpy = new QSignalSpy(m_device, SIGNAL(deviceIdChanged(QString)));
    }

    void cleanup() {
        delete m_device; m_device = nullptr;
        delete m_registerFinishedSpy; m_registerFinishedSpy = nullptr;
        delete m_creationTimeSpy; m_creationTimeSpy = nullptr;
        delete m_deviceIdSpy; m_deviceIdSpy = nullptr;
    }

    void createWithEmpyAttributes();
    void createWithAttributes();
    void creationFailesWithNetworkError();
    void creationFailesWithServerErrorResponse();
    void creationFailesWithHttpStatus_200();
    void creationErrorClearesAfterSuccess();

private:

    QJsonDocument createResponseDocument(const QString& deviceId, const QDateTime& creationTime)
    {
        return QJsonDocument( { {"deviceId", deviceId}, {"createdAt", creationTime.toString(Qt::ISODate) } } );
    }

    QJsonDocument createRequestDocument() {
        QJsonArray attributesArray;
        for( auto key: m_device->attributes().keys()) {
            attributesArray << QJsonObject( { {key, QJsonValue::fromVariant(m_device->attributes()[key])} } );
        }

        QJsonObject obj
        {
            {"name", m_device->name()},
            {"manufacturer", m_device->manufacturer()},
            {"type", m_device->type()},
            {"description", m_device->description()},
            {"attributes", attributesArray}
        };

        return QJsonDocument(obj);
    }

    Device* m_device;
    QSignalSpy* m_registerFinishedSpy;
    QSignalSpy* m_creationTimeSpy;
    QSignalSpy* m_deviceIdSpy;
};


void RegisterDeviceTests::createWithEmpyAttributes()
{
    m_device->registerDevice();

    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/"), m_requestHandler->postRequestPath());
    QCOMPARE(QJsonDocument::fromJson(m_requestHandler->postData()), createRequestDocument());

    QDateTime responseCreationTime = QDateTime::currentDateTime();
    QString responseDeviceId = "123";

    // Now lets construct a response
    m_requestHandler->postRequestResponse()->offerResponse(201, createResponseDocument(responseDeviceId, responseCreationTime));

    QCOMPARE( m_registerFinishedSpy->count(), 1 );
    QCOMPARE( m_registerFinishedSpy->takeFirst().at(0).toBool(), true );
    QVERIFY(m_creationTimeSpy->count() == 1);
    QVERIFY(m_deviceIdSpy->count() == 1);
    QCOMPARE(m_device->deviceId(), responseDeviceId);
    QCOMPARE(m_device->creationTime().toString(), responseCreationTime.toString());
}

void RegisterDeviceTests::createWithAttributes()
{
    m_device->setAttributes({ {"attribute1", "attribute1Value"}, {"attribute1", 2} });

    m_device->registerDevice();

    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/"), m_requestHandler->postRequestPath());
    QCOMPARE(QJsonDocument::fromJson(m_requestHandler->postData()), createRequestDocument());

    QDateTime responseCreationTime = QDateTime::currentDateTime();
    QString responseDeviceId = "123";

    // Now lets construct a response
    m_requestHandler->postRequestResponse()->offerResponse(201, createResponseDocument(responseDeviceId, responseCreationTime));

    QCOMPARE( m_registerFinishedSpy->count(), 1 );
    QCOMPARE( m_registerFinishedSpy->takeFirst().at(0).toBool(), true );
    QVERIFY(m_creationTimeSpy->count() == 1);
    QVERIFY(m_deviceIdSpy->count() == 1);
    QCOMPARE(m_device->deviceId(), responseDeviceId);
    QCOMPARE(m_device->creationTime().toString(), responseCreationTime.toString());
}

void RegisterDeviceTests::creationFailesWithNetworkError()
{
    m_device->registerDevice();

    m_requestHandler->postRequestResponse()->offerResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_registerFinishedSpy->count(), 1 );
    QCOMPARE( m_registerFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_device->registerError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_device->registerError()->networkError() == QNetworkReply::TimeoutError);
}

void RegisterDeviceTests::creationFailesWithServerErrorResponse()
{
    m_device->registerDevice();

    TestResponse* response = m_requestHandler->postRequestResponse();
    QJsonDocument doc( QJsonObject( { {"description", "desc"}, {"code", 8000}, {"moreInfo", "info"}, {"apiver", 1} } ) );
    response->offerResponse(400, doc);

    QCOMPARE( m_registerFinishedSpy->count(), 1 );
    QCOMPARE( m_registerFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_device->registerError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(m_device->registerError()->httpStatusCode() == 400);
    QVERIFY(m_device->registerError()->description() == "desc");
    QVERIFY(m_device->registerError()->moreInfo() == "info");
    QVERIFY(m_device->registerError()->serverErrorCode() == 8000);
}

void RegisterDeviceTests::creationFailesWithHttpStatus_200()
{
    m_device->registerDevice();

    m_requestHandler->postRequestResponse()->offerResponse(200);

    QCOMPARE( m_registerFinishedSpy->count(), 1 );
    QCOMPARE( m_registerFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_device->registerError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(m_device->registerError()->httpStatusCode() == 200);
}

void RegisterDeviceTests::creationErrorClearesAfterSuccess()
{

}

QTEST_MAIN(RegisterDeviceTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_registerdevicetests.moc"
