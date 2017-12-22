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

class DataNodeReadTests : public TestBase
{
    Q_OBJECT
public:

    DataNodeReadTests() : TestBase(),
        m_dataNode(nullptr),
        m_readFinishedSpy(nullptr){}

private slots:

    void init() {
        m_dataNode = new DataNode();
        m_dataNode->setDeviceId("1234567890");
        m_dataNode->setName("node1");
        m_readFinishedSpy = new QSignalSpy(m_dataNode, SIGNAL(readFinished(bool)));
    }

    void cleanup() {
        delete m_dataNode; m_dataNode = nullptr;
        delete m_readFinishedSpy; m_readFinishedSpy = nullptr;
    }

    void verifyRequestPathWhenNodePathProvided();
    void verifyRequestPathOnReadValuesFromAndToDateProvided();
    void verifyRequestPathOnReadValuesFromDateProvided();
    void verifyRequestPathOnReadLatestValue();
    void readReturnZeroEntries();
    void readReturnSeveralEntries();
    void valuesClearedBetweenReads();
    void readFailesWhenNotReady();
    void readyToReadAfterReadComplete();
    void readFailesWithNetworkError();
    void readErrorClearesAfterSuccess();


private:

    QJsonObject valueToObj(const QPair<QVariant,QDateTime>& valueTimestampPair) {
        const qint64 ts = valueTimestampPair.second.toMSecsSinceEpoch();
        return QJsonObject( { { "v", QJsonValue::fromVariant(valueTimestampPair.first) },  { "ts",  ts} } );
    }

    QString expectedRequestPath(const QDateTime& startTime = QDateTime(), const QDateTime& endTime = QDateTime()) const {
        QString expectedPath = "/process/read/" + m_dataNode->deviceId() + "?datanodes=";
        if (!m_dataNode->path().isEmpty()) expectedPath.append( m_dataNode->path() );
        expectedPath.append( "/" + m_dataNode->name() );
        if (startTime.isValid() && endTime.isValid())
            expectedPath += "&fromdate=" + QString::number(startTime.toMSecsSinceEpoch()) + "&todate=" + QString::number(endTime.toMSecsSinceEpoch());
        else if (startTime.isValid())
            expectedPath += "&fromdate=" + QString::number(startTime.toMSecsSinceEpoch());

        return expectedPath;
    }

    DataNode* m_dataNode;
    QSignalSpy* m_readFinishedSpy;
};

void DataNodeReadTests::verifyRequestPathWhenNodePathProvided()
{
    m_dataNode->setPath("/path");
    m_dataNode->readLatestValue();
    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath());
}

void DataNodeReadTests::verifyRequestPathOnReadValuesFromAndToDateProvided()
{
    QDateTime startTime = QDateTime::currentDateTime().addDays(-1);
    QDateTime endTime = QDateTime::currentDateTime();
    m_dataNode->readValues(startTime, endTime);

    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath(startTime, endTime));
}

void DataNodeReadTests::verifyRequestPathOnReadValuesFromDateProvided()
{
    QDateTime startTime = QDateTime::currentDateTime().addDays(-1);
    m_dataNode->readValues(startTime);

    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath(startTime));
}

void DataNodeReadTests::verifyRequestPathOnReadLatestValue()
{
    m_dataNode->readLatestValue();

    QCOMPARE(m_requestHandler->getRequestPath(), expectedRequestPath());
}

void DataNodeReadTests::readReturnZeroEntries()
{
    m_dataNode->readValues(QDateTime::currentDateTime().addDays(-1));
    QJsonObject obj;
    obj["datanodeReads"] = QJsonArray();
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));
    QCOMPARE( m_readFinishedSpy->count(), 1 );
    QCOMPARE( m_readFinishedSpy->takeFirst().at(0).toBool(), true );
}

void DataNodeReadTests::readReturnSeveralEntries()
{
    m_dataNode->readValues(QDateTime::currentDateTime().addDays(-1));

    QJsonArray valuesArray;
    QPair<QVariant, QDateTime> val1(1,QDateTime::currentDateTime().addSecs(-5));
    QPair<QVariant, QDateTime> val2(2,QDateTime::currentDateTime());
    QPair<QVariant, QDateTime> val3(3,QDateTime::currentDateTime().addSecs(+5));
    valuesArray << valueToObj(val1);
    valuesArray << valueToObj(val2);
    valuesArray << valueToObj(val3);

    QJsonObject nodeObj;
    nodeObj["name"] = m_dataNode->name();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNode->dataType());
    nodeObj["values"] = valuesArray;
    QJsonArray nodeObjArray;
    nodeObjArray << nodeObj;

    QJsonObject obj;
    obj["datanodeReads"] = nodeObjArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));
    QCOMPARE( m_readFinishedSpy->count(), 1 );
    QCOMPARE( m_readFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->values().count(), 3 );
    QCOMPARE( m_dataNode->values()[0], val1 );
    QCOMPARE( m_dataNode->values()[1], val2 );
    QCOMPARE( m_dataNode->values()[2], val3 );
}

void DataNodeReadTests::valuesClearedBetweenReads()
{
    readReturnSeveralEntries();

    // Now lets read second set of values. Values function should return only the
    // second set of values
    m_dataNode->readValues(QDateTime::currentDateTime());

    QPair<QVariant, QDateTime> val4(4,QDateTime::currentDateTime().addSecs(+10));
    QPair<QVariant, QDateTime> val5(5,QDateTime::currentDateTime().addSecs(+15));
    QJsonArray valuesArray2;
    valuesArray2 << valueToObj(val4);
    valuesArray2 << valueToObj(val5);

    QJsonObject nodeObj;
    nodeObj["name"] = m_dataNode->name();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNode->dataType());
    nodeObj["values"] = valuesArray2;
    QJsonArray nodeObjArray2;
    nodeObjArray2 << nodeObj;

    QJsonObject obj;
    obj["datanodeReads"] = nodeObjArray2;
    obj["href"] = "foo";

    // Simulate second response
    m_readFinishedSpy->clear();
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE( m_readFinishedSpy->count(), 1 );
    QCOMPARE( m_readFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE( m_dataNode->values().count(), 2 );
    QCOMPARE( m_dataNode->values()[0], val4 );
    QCOMPARE( m_dataNode->values()[1], val5 );
}

void DataNodeReadTests::readFailesWhenNotReady()
{
    QCOMPARE(m_dataNode->isReadyToRead(), true);
    QCOMPARE(m_dataNode->readLatestValue(), true);
    QCOMPARE(m_dataNode->isReadyToRead(), false);
    QCOMPARE (m_dataNode->readLatestValue(), false);
}

void DataNodeReadTests::readyToReadAfterReadComplete()
{
    QCOMPARE(m_dataNode->readLatestValue(), true);

    QPair<QVariant, QDateTime> val3(3,QDateTime::currentDateTime().addSecs(+5));
    QJsonArray valuesArray;
    valuesArray << valueToObj(val3);

    QJsonObject nodeObj;
    nodeObj["name"] = m_dataNode->name();
    nodeObj["dataType"] = QMetaEnum::fromType<DataNode::DataType>().valueToKey(m_dataNode->dataType());
    nodeObj["values"] = valuesArray;
    QJsonArray nodeObjArray;
    nodeObjArray << nodeObj;

    QJsonObject obj;
    obj["datanodeReads"] = nodeObjArray;
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));

    QCOMPARE(m_dataNode->isReadyToRead(), true);
    QCOMPARE(m_dataNode->readLatestValue(), true);
    QCOMPARE(m_dataNode->isReadyToRead(), false);
}

void DataNodeReadTests::readFailesWithNetworkError()
{
    QCOMPARE(m_dataNode->readLatestValue(), true);

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_readFinishedSpy->count(), 1);
    QCOMPARE(m_readFinishedSpy->takeFirst().at(0).toBool(), false);
    QVERIFY(m_dataNode->readError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_dataNode->readError()->networkError() == QNetworkReply::TimeoutError);

}

void DataNodeReadTests::readErrorClearesAfterSuccess()
{
    QCOMPARE(m_dataNode->readLatestValue(), true);

    simulateGetResponse(QNetworkReply::TimeoutError);

    QCOMPARE(m_readFinishedSpy->count(), 1);
    QCOMPARE(m_readFinishedSpy->takeFirst().at(0).toBool(), false);

    m_dataNode->readLatestValue();
    QJsonObject obj;
    obj["datanodeReads"] = QJsonArray();
    obj["href"] = "foo";
    simulateGetResponse(200, QJsonDocument(obj));
    QCOMPARE( m_readFinishedSpy->count(), 1 );
    QCOMPARE( m_readFinishedSpy->takeFirst().at(0).toBool(), true );
    QVERIFY(m_dataNode->readError()->errorType() == Error::ErrorType::NoError);
    QVERIFY(m_dataNode->readError()->networkError() == QNetworkReply::NoError);
}

QTEST_MAIN(DataNodeReadTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_datanodereadtests.moc"
