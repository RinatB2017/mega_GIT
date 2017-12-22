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

class GetDeviceDataNodesTests : public TestBase
{
    Q_OBJECT
public:

    GetDeviceDataNodesTests() : TestBase(),
        m_device(nullptr),
        m_getDataNodesFinishedSpy(nullptr) {}

private slots:

    void init() {
        m_device = new Device();
        m_device->setDeviceId("123456789");
        m_getDataNodesFinishedSpy = new QSignalSpy(m_device, SIGNAL(getDataNodesFinished(bool)));
        QList<DataNode*> list = { &m_dataNode1, &m_dataNode2, &m_dataNode3 };
        DataNode::DataType dataTypes[] = { DataNode::Double, DataNode::Binary, DataNode::String };
        for (int i = 0; i < list.count(); i++) {
            auto arg = [i] (const QString& str) { return str + QString::number(i); };
            list[i]->setName(arg("name"));
            list[i]->setPath(arg("path"));
            list[i]->setHref(arg("href"));
            list[i]->setUnit(arg("unit"));
            list[i]->setDataType(dataTypes[i]);
        }
    }

    void cleanup() {
        delete m_device; m_device = nullptr;
        delete m_getDataNodesFinishedSpy; m_getDataNodesFinishedSpy = nullptr;
        m_dataNodes.clear();
    }

    void getDeviceDataNodesWithDeviceId();
    void getDeviceDataNodesWithDeviceIdsdGetParam();
    void getDeviceDataNodesFailesWithNetworkError();
    void getDeviceDataNodesFailesWithServerErrorResponse();
    void getDeviceDataNodesFailesWithHttpStatus_400();
    void getDeviceDataNodesErrorClearesAfterSuccess();
    void getDeviceDataNodesReturnsZeroItem();


private:

    QJsonDocument getResponse()
    {
        QJsonObject obj;
        obj["fullSize"] = m_dataNodes.count();
        obj["limit"] = m_dataNodes.count();
        obj["offset"] = 0;

        QJsonArray dataNodesJsonArray;
        for (auto dev : m_dataNodes) {
            dataNodesJsonArray << dataNodeToJsonObject(*dev);
        }

        obj["items"] = dataNodesJsonArray;
        return QJsonDocument(obj);
    }

    QJsonObject dataNodeToJsonObject(const DataNode& dataNode)
    {
        QMetaEnum me = QMetaEnum::fromType<DataNode::DataType>();

        QJsonObject obj
        {
            {"name", dataNode.name()},
            {"unit", dataNode.unit()},
            {"dataType", me.valueToKey(dataNode.dataType())},
            {"path", dataNode.path()},
            {"href", dataNode.href()}
        };

        return obj;
    }


    Device* m_device;
    QList<DataNode*> m_dataNodes;
    DataNode m_dataNode1;
    DataNode m_dataNode2;
    DataNode m_dataNode3;
    QSignalSpy* m_getDataNodesFinishedSpy;
};

void GetDeviceDataNodesTests::getDeviceDataNodesWithDeviceId()
{
    m_device->getDataNodes();
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/%1/datanodes").arg(m_device->deviceId()), m_requestHandler->getRequestPath());

    m_dataNodes = { &m_dataNode1, &m_dataNode2, &m_dataNode3 };

    simulateGetResponse(200, getResponse());

    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), true);
    QVERIFY(m_device->dataNodes().count() == m_dataNodes.count());
    QCOMPARE(m_dataNode1, *m_device->dataNodes()[0]);
    QCOMPARE(m_dataNode2, *m_device->dataNodes()[1]);
    QCOMPARE(m_dataNode3, *m_device->dataNodes()[2]);
}

void GetDeviceDataNodesTests::getDeviceDataNodesWithDeviceIdsdGetParam()
{
    m_device->getDataNodes("333");
    QCOMPARE(QString("/devices/333/datanodes"), m_requestHandler->getRequestPath());
}

void GetDeviceDataNodesTests::getDeviceDataNodesFailesWithNetworkError()
{
    m_device->getDataNodes();
    simulateGetResponse(QNetworkReply::TimeoutError);
    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_device->dataNodes().count() == 0);
    QVERIFY(m_device->getDataNodesError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_device->getDataNodesError()->networkError() == QNetworkReply::TimeoutError);
}

void GetDeviceDataNodesTests::getDeviceDataNodesFailesWithServerErrorResponse()
{
    m_device->getDataNodes();
    QJsonDocument doc( QJsonObject( { {"description", "desc"}, {"code", 8000}, {"moreInfo", "info"}, {"apiver", 1} } ) );
    simulateGetResponse(400, doc);

    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_device->getDataNodesError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(m_device->getDataNodesError()->httpStatusCode() == 400);
    QVERIFY(m_device->getDataNodesError()->description() == "desc");
    QVERIFY(m_device->getDataNodesError()->moreInfo() == "info");
    QVERIFY(m_device->getDataNodesError()->serverErrorCode() == 8000);

}

void GetDeviceDataNodesTests::getDeviceDataNodesFailesWithHttpStatus_400()
{
    m_device->getDataNodes();
    simulateGetResponse(400);
    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_device->dataNodes().count() == 0);
    QVERIFY(m_device->getDataNodesError()->errorType() == Error::ErrorType::HttpError);
    QVERIFY(m_device->getDataNodesError()->httpStatusCode() == 400);

}

void GetDeviceDataNodesTests::getDeviceDataNodesErrorClearesAfterSuccess()
{
    m_device->getDataNodes();
    simulateGetResponse(400);
    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), false);

    m_getDataNodesFinishedSpy->clear();
    m_dataNodes = { &m_dataNode1, &m_dataNode2, &m_dataNode3 };

    simulateGetResponse(200, getResponse());

    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), true);
    QVERIFY(m_device->dataNodes().count() == m_dataNodes.count());
    QCOMPARE(m_dataNode1, *m_device->dataNodes()[0]);
    QCOMPARE(m_dataNode2, *m_device->dataNodes()[1]);
    QCOMPARE(m_dataNode3, *m_device->dataNodes()[2]);

}

void GetDeviceDataNodesTests::getDeviceDataNodesReturnsZeroItem()
{
    m_device->getDataNodes();
    // verify that path is correct and the request document is expected
    QCOMPARE(QString("/devices/%1/datanodes").arg(m_device->deviceId()), m_requestHandler->getRequestPath());

    simulateGetResponse(200, getResponse());

    QCOMPARE(m_getDataNodesFinishedSpy->count(), 1);
    QCOMPARE(m_getDataNodesFinishedSpy->takeFirst().at(0).toBool(), true);
    QVERIFY(m_device->dataNodes().count() == m_dataNodes.count());
}

QTEST_MAIN(GetDeviceDataNodesTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_getdevicedatanodestests.moc"
