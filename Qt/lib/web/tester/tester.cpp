/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QStringList>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "tester.hpp"
//--------------------------------------------------------------------------------
Tester::Tester(int index,
               QObject *parent) :
    QObject(parent),
    index(index),
    is_loaded(false)
{

}
//--------------------------------------------------------------------------------
void Tester::init(const QString &proxy_str)
{
    if(proxy_str.isEmpty())
    {
        emit error(tr("no data"));
        return;
    }

    QStringList sl = proxy_str.split(":");
    if(sl.count() != 2)
    {
        emit error(tr("unknown format"));
        return;
    }

    QString Ip = sl.at(0);
    if(Ip.isEmpty())
    {
        emit error(tr("Ip not found"));
        return;
    }

    bool ok;
    int port = sl.at(1).toInt(&ok);
    if(ok == false)
    {
        emit error(tr("port error"));
        return;
    }

    QUrl url(Ip);
    if(url.isValid() == false)
    {
        emit error(tr("error url"));
        return;
    }

    //emit info(QString(tr("Proxy %1:%2")).arg(Ip).arg(port));

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(Ip);
    proxy.setPort(port);
    QNetworkProxy::setApplicationProxy(proxy);
}
//--------------------------------------------------------------------------------
void Tester::run(const QUrl &url)
{
    is_loaded = false;
    QNetworkRequest request(url);

    currentReply = manager.get(request);
    connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(currentReply, SIGNAL(finished()), this, SLOT(slot_finished()));
}
//--------------------------------------------------------------------------------
void Tester::readyRead(void)
{
    int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
    {
        currentReply->readAll();
        //emit debug("readyRead");
    }
    else
    {
        emit error(QString("statusCode = %1").arg(statusCode));
    }
}
//--------------------------------------------------------------------------------
void Tester::metaDataChanged(void)
{
    QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid())
    {
        run(redirectionTarget);
    }
}
//--------------------------------------------------------------------------------
void Tester::error(QNetworkReply::NetworkError errno)
{
    emit error(QString("index %1: error %2")
               .arg(index)
               .arg(errno));
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
    is_loaded = true;

    emit finished();
}
//--------------------------------------------------------------------------------
void Tester::slot_finished(void)
{
    is_loaded = true;
    emit finished();
}
//--------------------------------------------------------------------------------
int Tester::get_index(void)
{
    return index;
}
//--------------------------------------------------------------------------------
