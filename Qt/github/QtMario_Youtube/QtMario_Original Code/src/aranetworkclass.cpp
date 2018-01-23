#include "aranetworkclass.h"

AraNetworkClass::AraNetworkClass(QObject *parent) : QObject(parent)
{
    this->qnam = new QNetworkAccessManager(this);
    QObject::connect(this->qnam, SIGNAL(finished(QNetworkReply*)),this, SLOT(readyRead(QNetworkReply*)));
}

void AraNetworkClass::makeRequest(QString endpointRequest)
{
    qnam->get(QNetworkRequest(QUrl(endpointRequest)));
}

void AraNetworkClass::readyRead(QNetworkReply * reply){
    QByteArray myData;
    myData = reply->readAll();
    emit this->dataReadyRead(myData);
}
