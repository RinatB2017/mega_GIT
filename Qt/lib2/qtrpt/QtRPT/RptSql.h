/*
Name: QtRpt
Version: 3.1.1
Web-site: https://qtrpt.sourceforge.io
Programmer: Oleksii Osypov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2025 Oleksii Osypov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <QObject>
#include <QSqlQuery>
#include <QImage>
#include "RptDsAbstract.h"

struct RptSqlConnection
{
    bool active;
    int pageReportNo;
    QString dsName;
    QString dbType;
    QString dbName;
    QString dbHost;
    QString dbUser;
    QString dbPassword;
    int dbPort;
    QString dbConnectionName;
    QString sqlQuery;
    QString dbCoding;
    QString charsetCoding;
};

class RptSql : public RptDsAbstract
{
    Q_OBJECT
public:
    explicit RptSql(QObject *parent = nullptr);
    bool openQuery();
    virtual void loadXML(QDomElement dsElement) override;
    virtual QString getFieldValue(QString fieldName, int recNo) override;
    QImage getFieldImage(QString fieldName, int recNo);
    void setConnection(RptSqlConnection sqlConnection);

private:
    QSqlDatabase db;
    QSqlQuery *query;
    RptSqlConnection m_sqlConnection;

signals:

public slots:

};
