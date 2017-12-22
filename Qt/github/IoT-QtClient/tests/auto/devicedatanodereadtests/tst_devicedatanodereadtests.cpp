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

class DeviceDataNodeReadTests : public TestBase
{
    Q_OBJECT
public:

    DeviceDataNodeReadTests() : TestBase(),
        m_device(nullptr)
//        m_readCompleteSpy(nullptr),
//        m_readFailedSpy(nullptr)
    {}

private slots:

    void init() {
        m_device = new Device();
//        m_readCompleteSpy = new QSignalSpy(m_devce, &DataNode::readComplete);
//        m_readFailedSpy = new QSignalSpy(m_dataNode, &DataNode::readFailed);
    }

    void cleanup() {
        delete m_device; m_device = nullptr;
        m_dataNodes.clear();
    }

    void verifyRequestPath_data();
    void verifyRequestPath();
    void verifyRequestPathFromAndToDateProvided_data();
    void verifyRequestPathFromAndToDateProvided();
    void verifyRequestPathFromDateProvided_data();
    void verifyRequestPathFromDateProvided();
    void readReturnZeroEntries_data();
    void readReturnZeroEntries();
    void readReturnSeveralEntriesForOneNode_data();
    void readReturnSeveralEntriesForOneNode();
    void valuesClearedBetweenReadsForOneNode();

private:

    QJsonObject valueToObj(const QPair<QVariant,QDateTime>& valueTimestampPair) {
        const qint64 ts = valueTimestampPair.second.toMSecsSinceEpoch();
        return QJsonObject( { { "v", QJsonValue::fromVariant(valueTimestampPair.first) },  { "ts",  ts} } );
    }

    QString expectedRequestPath(const QDateTime& startTime = QDateTime(), const QDateTime& endTime = QDateTime()) const {
        if (m_dataNodes.isEmpty()) return QString();

        QString expectedPath = "/process/read/" + m_device->deviceId() + "?datanodes=";
        for (int i = 0; i < m_dataNodes.count(); i++) {
            if (!m_dataNodes[i]->path().isEmpty()) expectedPath.append( m_dataNodes[i]->path() );
            expectedPath.append( "/" + m_dataNodes[i]->name() );
            if (i < m_dataNodes.count()-1) expectedPath.append(",");
        }
        if (startTime.isValid() && endTime.isValid())
            expectedPath += "&fromdate=" + QString::number(startTime.toMSecsSinceEpoch()) + "&todate=" + QString::number(endTime.toMSecsSinceEpoch());
        else if (startTime.isValid())
            expectedPath += "&fromdate=" + QString::number(startTime.toMSecsSinceEpoch());

        return expectedPath;
    }

    void addNodes(int count,bool addPaths, bool addToDevice) {
        for (int i = 0; i < count; i++) {
            DataNode* node = new DataNode();
            node->setName("name" + QString::number(i));
            if (addPaths) {
                node->setPath("/path" + QString::number(i));
            }
            m_dataNodes << node;
        }

        if (addToDevice) {
            m_device->addDataNodes(m_dataNodes);
        }

    }

    Device* m_device;
    QList<DataNode*> m_dataNodes;
    QSignalSpy* m_readCompleteSpy;
    QSignalSpy* m_readFailedSpy;
};

inline void testConfiguration()
{
    QTest::addColumn<int>("nodeCount");
    QTest::addColumn<bool>("usePath");
    QTest::addColumn<bool>("addToServer");

    QTest::newRow("one node") << 1 << false << false;
    QTest::newRow("one node with path") << 1 << true << false;
    QTest::newRow("one node with path added to server") << 1 << true << true;

    QTest::newRow("three nodes") << 3 << false << false;
    QTest::newRow("three nodes with path") << 3 << true << false;
    QTest::newRow("three nodes with path added to server") << 3 << true << true;
}

void DeviceDataNodeReadTests::verifyRequestPath_data()
{
    testConfiguration();
}

void DeviceDataNodeReadTests::verifyRequestPath()
{
    QFETCH(bool, usePath);
    QFETCH(int, nodeCount);
    QFETCH(bool, addToServer);
    addNodes(nodeCount, usePath, addToServer);

    if (addToServer) {
        m_device->readDataNodesLatestValue();
    } else {
        m_device->readDataNodesLatestValue(m_dataNodes);
    }
    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath());
}

void DeviceDataNodeReadTests::verifyRequestPathFromAndToDateProvided_data()
{
    testConfiguration();
}

void DeviceDataNodeReadTests::verifyRequestPathFromAndToDateProvided()
{
    QFETCH(bool, usePath);
    QFETCH(int, nodeCount);
    QFETCH(bool, addToServer);
    addNodes(nodeCount, usePath, addToServer);

    QDateTime startTime = QDateTime::currentDateTime().addDays(-1);
    QDateTime endTime = QDateTime::currentDateTime();

    if (addToServer) {
        m_device->readDataNodeValues(startTime, endTime);
    } else {
        m_device->readDataNodeValues(startTime, endTime, m_dataNodes);
    }

    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath(startTime, endTime));
}

void DeviceDataNodeReadTests::verifyRequestPathFromDateProvided_data()
{
    testConfiguration();
}

void DeviceDataNodeReadTests::verifyRequestPathFromDateProvided()
{
    QFETCH(bool, usePath);
    QFETCH(int, nodeCount);
    QFETCH(bool, addToServer);
    addNodes(nodeCount, usePath, addToServer);

    QDateTime startTime = QDateTime::currentDateTime().addDays(-1);
    if (addToServer) {
        m_device->readDataNodeValues(startTime);
    } else {
        m_device->readDataNodeValues(startTime, m_dataNodes);
    }
    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath(startTime));
}

void DeviceDataNodeReadTests::readReturnZeroEntries_data()
{
    testConfiguration();
}

void DeviceDataNodeReadTests::readReturnZeroEntries()
{
    QFETCH(bool, usePath);
    QFETCH(int, nodeCount);
    QFETCH(bool, addToServer);
    addNodes(nodeCount, usePath, addToServer);

    if (addToServer) {
        m_device->readDataNodeValues(QDateTime::currentDateTime().addDays(-1));
    } else {
        m_device->readDataNodeValues(QDateTime::currentDateTime().addDays(-1), m_dataNodes);
    }
    QJsonObject obj;
    obj["datanodeReads"] = QJsonArray();
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));
    for (int i = 0; i < nodeCount; i++) {
        QCOMPARE( m_dataNodes[i]->values().count(), 0 );
    }
}

void DeviceDataNodeReadTests::readReturnSeveralEntriesForOneNode_data()
{
    QTest::addColumn<bool>("addNodeToDevice");
    QTest::newRow("Node added to device") << true;
    QTest::newRow("Node not added to device") <<  false;
}

void DeviceDataNodeReadTests::readReturnSeveralEntriesForOneNode()
{
    QFETCH(bool, addNodeToDevice);
    addNodes(1, true, addNodeToDevice);
    if (addNodeToDevice) {
        m_device->readDataNodeValues(QDateTime::currentDateTime().addDays(-1));
    } else {
        m_device->readDataNodeValues(QDateTime::currentDateTime().addDays(-1), m_dataNodes);
    }

    QJsonArray valuesArray;
    QPair<QVariant, QDateTime> val1(1,QDateTime::currentDateTime().addSecs(-5));
    QPair<QVariant, QDateTime> val2(2,QDateTime::currentDateTime());
    QPair<QVariant, QDateTime> val3(3,QDateTime::currentDateTime().addSecs(+5));
    valuesArray << valueToObj(val1);
    valuesArray << valueToObj(val2);
    valuesArray << valueToObj(val3);

    QJsonObject nodeObj;
    nodeObj["name"] = m_dataNodes[0]->name();
    nodeObj["path"] = m_dataNodes[0]->path();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNodes[0]->dataType());
    nodeObj["values"] = valuesArray;
    QJsonArray nodeObjArray;
    nodeObjArray << nodeObj;

    QJsonObject obj;
    obj["datanodeReads"] = nodeObjArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));
//    QCOMPARE( m_readCompleteSpy->count(), 1 );
//    QCOMPARE( m_readFailedSpy->count(), 0 );
    QCOMPARE( m_dataNodes[0]->values().count(), 3 );
    QCOMPARE( m_dataNodes[0]->values()[0], val1 );
    QCOMPARE( m_dataNodes[0]->values()[1], val2 );
    QCOMPARE( m_dataNodes[0]->values()[2], val3 );
}

void DeviceDataNodeReadTests::valuesClearedBetweenReadsForOneNode()
{
    addNodes(1, true, true);
    m_device->readDataNodeValues(QDateTime::currentDateTime().addDays(-1));

    QJsonArray valuesArray;
    QPair<QVariant, QDateTime> val1(1,QDateTime::currentDateTime().addSecs(-5));
    QPair<QVariant, QDateTime> val2(2,QDateTime::currentDateTime());
    QPair<QVariant, QDateTime> val3(3,QDateTime::currentDateTime().addSecs(+5));
    valuesArray << valueToObj(val1);
    valuesArray << valueToObj(val2);
    valuesArray << valueToObj(val3);

    QJsonObject nodeObj;
    nodeObj["name"] = m_dataNodes[0]->name();
    nodeObj["path"] = m_dataNodes[0]->path();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNodes[0]->dataType());
    nodeObj["values"] = valuesArray;
    QJsonArray nodeObjArray;
    nodeObjArray << nodeObj;

    QJsonObject obj;
    obj["datanodeReads"] = nodeObjArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));

    // Now lets read second set of values. Values function should return only the
    // second set of values
    m_dataNodes[0]->readValues(QDateTime::currentDateTime());

    QPair<QVariant, QDateTime> val4(4,QDateTime::currentDateTime().addSecs(+10));
    QPair<QVariant, QDateTime> val5(5,QDateTime::currentDateTime().addSecs(+15));
    QJsonArray valuesArray2;
    valuesArray2 << valueToObj(val4);
    valuesArray2 << valueToObj(val5);

    nodeObj["name"] = m_dataNodes[0]->name();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNodes[0]->dataType());
    nodeObj["values"] = valuesArray2;
    QJsonArray nodeObjArray2;
    nodeObjArray2 << nodeObj;

    obj["datanodeReads"] = nodeObjArray2;
    obj["href"] = "foo";

    // Simulate second response
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE( m_dataNodes[0]->values().count(), 2 );
    QCOMPARE( m_dataNodes[0]->values()[0], val4 );
    QCOMPARE( m_dataNodes[0]->values()[1], val5 );
}

QTEST_MAIN(DeviceDataNodeReadTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_devicedatanodereadtests.moc"
