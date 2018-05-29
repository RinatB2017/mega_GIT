#ifndef TestMessage_H
#define TestMessage_H

#include <QObject>

class TestMessage : public QObject
{
    Q_OBJECT
public:
    TestMessage(QObject *parent = NULL);

private slots:
    void testNamespaceOptions();
    void testMessageToString();
    
};

#endif // TestMessage_H
