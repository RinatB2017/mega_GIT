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

class DataNodeWriteTests : public TestBase
{
    Q_OBJECT
public:

    DataNodeWriteTests() : TestBase(),
        m_dataNode(nullptr),
        m_writeFinishedSpy(nullptr){}

private slots:

    void init() {
        m_dataNode = new DataNode();
        m_dataNode->setName("name1");
        m_writeFinishedSpy = new QSignalSpy(m_dataNode, SIGNAL(writeFinished(bool)));
    }

    void cleanup() {
        delete m_dataNode; m_dataNode = nullptr;
        delete m_writeFinishedSpy; m_writeFinishedSpy = nullptr;
    }

    void checkRequestMandatoryAttributes();
    void checkRequestOptionalAttributes();
    void writeIntValue();
    void writeIntValueWithTimeStamp();
    void writeTwoValues();
    void writeTwoValuesWithTimeStamps();
    void writeCachedValue();
    void writeCachedValues();
    void addValuesToCacheWhileSendingPreviousValues();
    void notAllValuesWritten();
    void writeFailesWithNetworkError();
    void writeCachedValueFailes();
    void rewritingCachedValueSuccess();
    void clearingCachedValues();
    void writeFailesWhenNotReady();
    void readyToWriteAfterWriteComplete();

private:

    DataNode* m_dataNode;
    QSignalSpy* m_writeFinishedSpy;
};

void DataNodeWriteTests::checkRequestMandatoryAttributes()
{
    m_dataNode->writeValue(1);
    QJsonDocument doc = QJsonDocument::fromJson(m_requestHandler->postData());
    QVERIFY(doc.isArray());
    QVERIFY(doc.array().count() == 1);
    QJsonObject obj = doc.array().at(0).toObject();
    QJsonObject expectedObj;
    expectedObj["name"] = m_dataNode->name();
    expectedObj["v"] = 1;
    QCOMPARE(obj, expectedObj);
}

void DataNodeWriteTests::checkRequestOptionalAttributes()
{
    m_dataNode->setPath("path1");
    m_dataNode->setUnit("c");
    m_dataNode->setDataType(DataNode::String);
    m_dataNode->writeValue({"1",22});
    QJsonDocument doc = QJsonDocument::fromJson(m_requestHandler->postData());
    QVERIFY(doc.isArray());
    QVERIFY(doc.array().count() == 1);
    QJsonObject obj = doc.array().at(0).toObject();
    QJsonObject expectedObj({
        {"name", m_dataNode->name()},
        {"v", "1"},
        {"path", "path1"},
        {"ts", 22},
        {"unit", "c"},
        {"dataType", "String"}
    });
    QCOMPARE(obj, expectedObj);
}

void DataNodeWriteTests::writeIntValue()
{
    m_dataNode->writeValue(1);

    QJsonObject obj;
    obj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    obj["totalWritten"] = 1;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(obj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
}

void DataNodeWriteTests::writeIntValueWithTimeStamp()
{
    m_dataNode->writeValue(QPair<QVariant,int>(3,44));

    QJsonObject obj;
    obj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    obj["totalWritten"] = 1;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(obj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
}

void DataNodeWriteTests::writeTwoValues()
{
    m_dataNode->writeValues( {1.0, 2.0} );

    QJsonObject obj;
    obj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 2} } ) } );
    obj["totalWritten"] = 2;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(obj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
}

void DataNodeWriteTests::writeTwoValuesWithTimeStamps()
{
    m_dataNode->writeValues( {QPair<QVariant,int>(1.0,22), QPair<QVariant,int>(2.0, 33)} );

    QJsonObject obj;
    obj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 2} } ) } );
    obj["totalWritten"] = 2;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(obj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
}

void DataNodeWriteTests::writeCachedValue()
{
    m_dataNode->writeValue(1, true);
    QVERIFY(m_dataNode->cachedValues().count() == 1);
    m_dataNode->writeCachedValues();

    QJsonDocument doc = QJsonDocument::fromJson(m_requestHandler->postData());
    QVERIFY(doc.isArray());
    QVERIFY(doc.array().count() == 1);
    QJsonObject obj = doc.array().at(0).toObject();
    QJsonObject expectedObj;
    expectedObj["name"] = m_dataNode->name();
    expectedObj["v"] = 1;
    QCOMPARE(obj, expectedObj);

    QJsonObject resultObj;
    resultObj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    resultObj["totalWritten"] = 1;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(resultObj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
    // Cached values are cleared once sent succesfully.
    QVERIFY(m_dataNode->cachedValues().count() == 0);
}

void DataNodeWriteTests::writeCachedValues()
{
    m_dataNode->writeValue(1, true);
    m_dataNode->writeValues( {2,3} , true);
    QVERIFY(m_dataNode->cachedValues().count() == 3);
    m_dataNode->writeCachedValues();

    QJsonDocument doc = QJsonDocument::fromJson(m_requestHandler->postData());
    QVERIFY(doc.isArray());
    QVERIFY(doc.array().count() == 3);

    QJsonObject obj = doc.array().at(0).toObject();
    QJsonObject expectedObj;
    expectedObj["name"] = m_dataNode->name();
    expectedObj["v"] = 1;
    QCOMPARE(obj, expectedObj);

    obj = doc.array().at(1).toObject();
    expectedObj["v"] = 2;
    QCOMPARE(obj, expectedObj);

    obj = doc.array().at(2).toObject();
    expectedObj["v"] = 3;
    QCOMPARE(obj, expectedObj);

    QJsonObject resultObj;
    resultObj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 3} } ) } );
    resultObj["totalWritten"] = 3;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(resultObj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
    // Cached values are cleared once sent succesfully.
    QVERIFY(m_dataNode->cachedValues().count() == 0);
}

void DataNodeWriteTests::addValuesToCacheWhileSendingPreviousValues()
{
    m_dataNode->writeValues( {"first","second", "third"} , true);
    QVERIFY(m_dataNode->cachedValues().count() == 3);
    m_dataNode->writeCachedValues();

    // Fourth value is added to the cache and it should not be removed from the cache
    // when the first write request completes
    m_dataNode->writeValue("fourth", true);
    QVERIFY(m_dataNode->cachedValues().count() == 4);
    QJsonObject resultObj;
    resultObj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 3} } ) } );
    resultObj["totalWritten"] = 3;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(resultObj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
    QCOMPARE(m_dataNode->cachedValues().count(), 1);
    QCOMPARE(m_dataNode->cachedValues()[0].first.toString(), QString("fourth"));
}

void DataNodeWriteTests::notAllValuesWritten()
{
    m_dataNode->writeValues( {1.0, 2.0} );

    QJsonObject obj;
    obj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    obj["totalWritten"] = 1;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(obj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), false );
}

void DataNodeWriteTests::writeFailesWithNetworkError()
{
    m_dataNode->writeValues( {1.0, 2.0} );

    m_requestHandler->postRequestResponse()->offerResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_dataNode->writeError()->errorType() == Error::ErrorType::NetworkError);
    QVERIFY(m_dataNode->writeError()->networkError() == QNetworkReply::TimeoutError);
}

void DataNodeWriteTests::writeCachedValueFailes()
{
    m_dataNode->writeValue( "value", true);
    m_dataNode->writeCachedValues();
    m_requestHandler->postRequestResponse()->offerResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_dataNode->cachedValues().count() == 1);
    QVERIFY(m_dataNode->cachedValues().at(0).first == "value");

}

void DataNodeWriteTests::rewritingCachedValueSuccess()
{
    m_dataNode->writeValue( "value", true);
    m_dataNode->writeCachedValues();
    m_requestHandler->postRequestResponse()->offerResponse(QNetworkReply::TimeoutError);

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), false );
    QVERIFY(m_dataNode->cachedValues().count() == 1);
    QVERIFY(m_dataNode->cachedValues().at(0).first == "value");

    m_writeFinishedSpy->clear();

    // Resend cached values
    m_dataNode->writeCachedValues();

    QJsonObject resultObj;
    resultObj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    resultObj["totalWritten"] = 1;

    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(resultObj));

    QCOMPARE( m_writeFinishedSpy->count(), 1 );
    QCOMPARE( m_writeFinishedSpy->takeFirst().at(0).toBool(), true );
    // Cached values are cleared once sent succesfully.
    QVERIFY(m_dataNode->cachedValues().count() == 0);
}

void DataNodeWriteTests::clearingCachedValues()
{
    m_dataNode->writeValue( "value", true);

    QVERIFY(m_dataNode->cachedValues().count() == 1);
    QVERIFY(m_dataNode->cachedValues().at(0).first == "value");

    m_dataNode->clearCachedValues();
    QVERIFY(m_dataNode->cachedValues().count() == 0);
}

void DataNodeWriteTests::writeFailesWhenNotReady()
{
    QCOMPARE(m_dataNode->isReadyToWrite(), true);
    QCOMPARE(m_dataNode->writeValue( "value" ), true);
    QCOMPARE(m_dataNode->isReadyToWrite(), false);
    QCOMPARE (m_dataNode->writeValue("value"), false);
}

void DataNodeWriteTests::readyToWriteAfterWriteComplete()
{
    QCOMPARE(m_dataNode->writeValue( "value" ), true);

    QJsonObject resultObj;
    resultObj["writeResults"] = QJsonArray( { QJsonObject( { {"href", "ref"}, {"writtenCount", 1} } ) } );
    resultObj["totalWritten"] = 1;

    QCOMPARE(m_dataNode->isReadyToWrite(), false);
    m_requestHandler->postRequestResponse()->offerResponse(200, QJsonDocument(resultObj));
    QCOMPARE(m_dataNode->isReadyToWrite(), true);
    QCOMPARE(m_dataNode->writeValue( "value" ), true);
}

QTEST_MAIN(DataNodeWriteTests)

// To tricker mock tool because our header is in cpp file.
#include "tst_datanodewritetests.moc"
