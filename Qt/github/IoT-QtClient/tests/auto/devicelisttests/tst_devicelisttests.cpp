#include <QString>
#include <QVariant>
#include <QtTest>
#include <QSignalSpy>
#include <QPointer>
#include <QJsonArray>

#include "device.h"
#include "devicelist.h"
#include "testrequesthandlerprovider.h"
#include "testrequesthandler.h"
#include "testnetworkreply.h"
#include "error.h"

using namespace iot;

class DeviceListTests : public QObject
{
    Q_OBJECT

public:
    DeviceListTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void getOneDevice();
    void getZeroDevices();
    void getManyDevices();
    void getFailesWithNetworkError();
    void getFailesWithServerErrorResponse();
    void getErrorClearsAfterSuccess();
    void getReturnsDifferentAmountOfDevices();

private:

    void configureDefaultDevice(Device& device, const QVariantMap& attributes = QVariantMap(), const QString& deviceId = QString());
    void setDeviceListSignalSpies(DeviceList& device);
    void clearDeviceListSignalSpies();
    QJsonDocument responseDocumentFromDeviceList(QList<Device*> devices);
    QJsonObject deviceToJsonObject(const Device& device);

    TestRequestHandler* m_requestHandler;

    QScopedPointer<QSignalSpy> m_getFinishedSpy;

};

#define VERIFY_NO_SIGNAL_EMITTED(spy) QVERIFY(spy.count() == 0)
#define VERIFY_ONE_SIGNAL_EMITTED(spy) QVERIFY(spy.count() == 1)
#define VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, attribute) QVERIFY(spy.count() == 1 && spy.takeFirst().at(0).toString() == attribute)
#define COMPARE_KEY_VALUE(doc, key, keyvalue) QVERIFY(doc.object().value(key) == keyvalue)

DeviceListTests::DeviceListTests() : m_requestHandler(nullptr),
    m_getFinishedSpy(nullptr)
{
}

void DeviceListTests::initTestCase()
{
    // Setup SUT ready for testing. We use stub that does not really
    // make network requests instead we simulate responses etc.
    m_requestHandler = new TestRequestHandler;
    TestRequestHandlerProvider::overrideRequestHandler(m_requestHandler);
}

void DeviceListTests::cleanupTestCase()
{
    delete m_requestHandler;
}

void DeviceListTests::getOneDevice()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices"), m_requestHandler->getRequestPath());

    // Now lets construct a response
    Device device1;
    configureDefaultDevice(device1);
    m_requestHandler->getRequestResponse()->offerResponse(200, responseDocumentFromDeviceList( { &device1 } ) );

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE(list.devices().count(), 1);
    QCOMPARE(*list.devices()[0], device1);
}

void DeviceListTests::getZeroDevices()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices"), m_requestHandler->getRequestPath());

    m_requestHandler->getRequestResponse()->offerResponse(200, responseDocumentFromDeviceList(QList<Device*>()));

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE(list.devices().count(), 0);
}

void DeviceListTests::getManyDevices()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices"), m_requestHandler->getRequestPath());

    // Now lets construct a response
    Device device1;
    Device device2;
    Device device3;
    configureDefaultDevice(device1, { {"aaa", 1} }, "device1" );
    configureDefaultDevice(device2, { {"aaa", 2} }, "device2");
    configureDefaultDevice(device3, { {"aaa", 3} }, "device3");
    m_requestHandler->getRequestResponse()->offerResponse(200, responseDocumentFromDeviceList( { &device1, &device2, &device3 } ) );

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE(list.devices().count(), 3);
    QCOMPARE(*list.devices()[0], device1);
    QCOMPARE(*list.devices()[1], device2);
    QCOMPARE(*list.devices()[2], device3);
}

void DeviceListTests::getFailesWithNetworkError()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    m_requestHandler->getRequestResponse()->offerResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(list.getError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(list.getError()->networkError() == QNetworkReply::TimeoutError);
}

void DeviceListTests::getFailesWithServerErrorResponse()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    m_requestHandler->getRequestResponse()->offerResponse(400);

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(list.getError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(list.getError()->httpStatusCode() == 400);
}

void DeviceListTests::getErrorClearsAfterSuccess()
{
    DeviceList list;
    setDeviceListSignalSpies(list);

    list.get();
    m_requestHandler->getRequestResponse()->offerResponse(400);

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), false );

    m_getFinishedSpy->clear();

    list.get();

    // Now lets construct a response
    Device device1;
    configureDefaultDevice(device1, { {"aaa", 1} }, "device1" );
    m_requestHandler->getRequestResponse()->offerResponse(200, responseDocumentFromDeviceList( { &device1 } ) );

    QCOMPARE( m_getFinishedSpy->count(), 1 );
    QCOMPARE( m_getFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE(list.devices().count(), 1);
    QCOMPARE(*list.devices()[0], device1);
    QVERIFY(list.getError()->errorType() == Error::ErrorType::NoError);
    QVERIFY(list.getError()->httpStatusCode() == -1);

}

void DeviceListTests::getReturnsDifferentAmountOfDevices()
{

}


void DeviceListTests::setDeviceListSignalSpies(DeviceList& deviceList)
{
    m_getFinishedSpy.reset(new QSignalSpy(&deviceList, SIGNAL(getFinished(bool))));
}

void DeviceListTests::clearDeviceListSignalSpies()
{
    if (!m_getFinishedSpy.isNull()) m_getFinishedSpy->clear();
}

QJsonDocument DeviceListTests::responseDocumentFromDeviceList(QList<Device*> devices)
{
    QJsonObject obj;
    obj["fullSize"] = devices.count();
    obj["limit"] = devices.count();
    obj["offset"] = 0;

    QJsonArray devicesJsonArray;
    for (auto dev : devices) {
        devicesJsonArray << deviceToJsonObject(*dev);
    }

    obj["items"] = devicesJsonArray;
    return QJsonDocument(obj);
}

QJsonObject DeviceListTests::deviceToJsonObject(const Device& device)
{
    QJsonArray attributesArray;
    for( auto key: device.attributes().keys()) {
        attributesArray << QJsonObject( { {key, QJsonValue::fromVariant(device.attributes()[key])} } );
    }

    QJsonObject obj
    {
        {"name", device.name()},
        {"manufacturer", device.manufacturer()},
        {"type", device.type()},
        {"description", device.description()},
        {"attributes", attributesArray},
        {"deviceId", device.deviceId()},
        {"createdAt", device.creationTime().toString(Qt::ISODate)},
        {"href", device.href()}
    };

    return obj;
}

void DeviceListTests::configureDefaultDevice(Device &device, const QVariantMap& attributes, const QString& deviceId)
{
    device.setName("name");
    device.setManufacturer("manufacturer");
    device.setType("type");
    device.setDescription("description");
    device.setHref("http://address/" + deviceId);
    device.setAttributes(attributes);
    if (!deviceId.isEmpty()) device.setDeviceId(deviceId);
}


QTEST_APPLESS_MAIN(DeviceListTests)

#include "tst_devicelisttests.moc"
