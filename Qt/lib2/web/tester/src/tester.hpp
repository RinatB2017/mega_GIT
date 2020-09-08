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
#ifndef TESTER_HPP
#define TESTER_HPP
//--------------------------------------------------------------------------------
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
//--------------------------------------------------------------------------------
class Tester : public QObject
{
    Q_OBJECT
public:
    explicit Tester(int index, QObject *parent = nullptr);

    void init(const QString &proxy_str);
    void run(const QUrl &url);
    int get_index(void);
    
signals:
    void info(const QString &);
    void debug(const QString &);

    void error(const QString &);
    void finished(void);

private slots:
    void readyRead(void);
    void metaDataChanged(void);
    void error(QNetworkReply::NetworkError errno);
    void slot_finished(void);

private:
    QString proxy_str;

    int index;
    bool is_loaded;
    QNetworkReply *currentReply;
    QNetworkAccessManager manager;

};
//--------------------------------------------------------------------------------
#endif // TESTER_HPP
