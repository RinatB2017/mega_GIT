#ifndef ARANETWORKCLASS_H
#define ARANETWORKCLASS_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class AraNetworkClass : public QObject
{
    Q_OBJECT
public:
    explicit AraNetworkClass(QObject *parent = 0);
    void makeRequest(QString);
signals:
    void dataReadyRead(QByteArray);
public slots:
    void readyRead(QNetworkReply *);
private:
    QNetworkAccessManager * qnam;

};

#endif // MYNETWORKCLASS_H
