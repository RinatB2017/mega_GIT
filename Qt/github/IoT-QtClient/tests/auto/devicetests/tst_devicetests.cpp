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

class DeviceTests : public QObject
{
    Q_OBJECT

public:
    DeviceTests();

private slots:
    void setName();
    void setManufacturer();
    void setType();
    void setDescription();
    void setAttributes();
    void setCreationTime();
    void setDeviceId();

};

#define VERIFY_NO_SIGNAL_EMITTED(spy) QVERIFY(spy.count() == 0)
#define VERIFY_ONE_SIGNAL_EMITTED(spy) QVERIFY(spy.count() == 1)
#define VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, attribute) QVERIFY(spy.count() == 1 && spy.takeFirst().at(0).toString() == attribute)
#define COMPARE_KEY_VALUE(doc, key, keyvalue) QVERIFY(doc.object().value(key) == keyvalue)

DeviceTests::DeviceTests() : QObject()
{
}

void DeviceTests::setName()
{
    Device device;
    QString deviceName("deviceName");
    QString deviceName2("deviceName2");
    QSignalSpy spy(&device, SIGNAL(nameChanged(QString)));
    device.setName(deviceName);
    QCOMPARE(device.name(), deviceName);
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toString() == deviceName);

    // set same name
    device.setName(deviceName);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    // set new name
    device.setName(deviceName2);
    QCOMPARE(device.name(), deviceName2);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, deviceName2);
}

void DeviceTests::setManufacturer()
{
    Device device;
    QString manufacturer("manufactorer");
    QString manufacturer2("manufactorer2");
    QSignalSpy spy(&device, SIGNAL(manufacturerChanged(QString)));
    device.setManufacturer(manufacturer);
    QCOMPARE(device.manufacturer(), manufacturer);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, manufacturer);

    device.setManufacturer(manufacturer);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    device.setManufacturer(manufacturer2);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, manufacturer2);
}

void DeviceTests::setType()
{
    Device device;
    QString type("type");
    QString type2("type2");
    QSignalSpy spy(&device, SIGNAL(typeChanged(QString)));
    device.setType(type);
    QCOMPARE(device.type(), type);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, type);

    device.setType(type);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    device.setType(type2);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, type2);
}

void DeviceTests::setDescription()
{
    Device device;
    QString description("description");
    QString description2("description2");
    QSignalSpy spy(&device, SIGNAL(descriptionChanged(QString)));
    device.setDescription(description);
    QCOMPARE(device.description(), description);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, description);

    device.setDescription(description);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    device.setDescription(description2);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, description2);
}

void DeviceTests::setAttributes()
{
    Device device;
    QVariantMap map;
    map["key"] = 1;
    QSignalSpy spy(&device, SIGNAL(attributesChanged(QVariantMap)));
    device.setAttributes(map);
    QCOMPARE(device.attributes(), map);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toMap(), map);

    device.setAttributes(map);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    QVariantMap map2;
    map2["key"] = "keyvalue";
    device.setAttributes(map2);
    QCOMPARE(device.attributes(), map2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toMap(), map2);
}

void DeviceTests::setCreationTime()
{
    Device device;
    QDateTime time = QDateTime::currentDateTime();
    QSignalSpy spy(&device, SIGNAL(creationTimeChanged(QDateTime)));
    device.setCreationTime(time);
    QCOMPARE(device.creationTime(), time);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toDateTime(), time);

    device.setCreationTime(time);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    time = time.addDays(1);
    device.setCreationTime(time);
    QCOMPARE(device.creationTime(), time);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toDateTime(), time);
}

void DeviceTests::setDeviceId()
{
    Device device;
    QString deviceId("deviceI d");
    QString deviceId2("deviceId2");
    QSignalSpy spy(&device, SIGNAL(deviceIdChanged(QString)));
    device.setDeviceId(deviceId);
    QCOMPARE(device.deviceId(), deviceId);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, deviceId);

    device.setDeviceId(deviceId);
    VERIFY_NO_SIGNAL_EMITTED(spy);

    device.setDeviceId(deviceId2);
    VERIFY_SIGNAL_AND_STRING_ATTRIBUTE(spy, deviceId2);
}

QTEST_MAIN(DeviceTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_devicetests.moc"
