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
#include <QDialogButtonBox>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QHBoxLayout>
#include <QWebElement>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QLineEdit>
#include <QWebFrame>
#include <QLabel>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "findlinks.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
FindLinks::FindLinks(QWebFrame *frame, QWidget *parent) :
    QDialog(parent),
    main_frame(frame)
{
    init();
}
//--------------------------------------------------------------------------------
void FindLinks::connect_log(void)
{
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parentWidget(), SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void FindLinks::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void FindLinks::accept(void)
{
    if(le->text().isEmpty())
    {
        emit error(tr("selector не определен"));
        QDialog::accept();
        return;
    }
    if(le2->text().isEmpty())
    {
        emit error(tr("selector не определен"));
        QDialog::accept();
        return;
    }

    QWebElement main_content = main_frame->findFirstElement(le->text());
    if(main_content.isNull())
    {
        emit error("main_content not found!");
        QDialog::accept();
        return;
    }

    QSqlDatabase db;
    QString driver_name = "QSQLITE";
    QString database_name = "links.db";
    if(db.databaseName() != database_name)
    {
        db = QSqlDatabase::addDatabase(driver_name);
        db.setDatabaseName(database_name);
    }
    bool ok = db.open();
    if(ok == false)
    {
        emit error(QString(tr("База данных %1 не открыта!")).arg(database_name));
        return;
    }

    QUrl baseUrl = main_frame->baseUrl();
    QWebElementCollection search_collection = main_content.findAll(le2->text());
    emit info(QString(tr("Нашли %1 ссылок")).arg(search_collection.count()));
    if(search_collection.count() != 0)
    {
        QSqlQuery sql;

        QStringList tables = db.tables();
        if(tables.contains("links"))
            emit error("links already exists!");
        else
        {
            emit info(tr("Создаем таблицу links"));
            sql.prepare("create table links ("
                        "title varchar(200), "
                        "link varchar(200))");
            ok = sql.exec();
            if(!ok)
            {
                emit error(QString(tr("query [%1] not exec!")).arg(sql.lastQuery()));
                return;
            }
        }
        QSqlTableModel model;
        model.setTable("links");
        bool result = model.select();
        if(result == false)
        {
            emit error(tr("Ошибка model.select()"));
            return;
        }
        model.clear();

        int n=0;
        foreach (QWebElement element, search_collection)
        {
            QWebElement link = element.findFirst(":link");
            QString href = link.attribute("href");
            if (!href.isEmpty())
            {
                QUrl relativeUrl(href);
                QUrl absoluteUrl = baseUrl.resolved(relativeUrl);
                emit info(element.toPlainText());
                emit debug(absoluteUrl.toString());

                QSqlRecord record = model.record(0);
                record.setValue("title", element.toPlainText());
                record.setValue("link", absoluteUrl.toString());
                model.insertRecord(n, record);
                model.submitAll();
                n++;
            }
        }
    }

    if(!db.isValid()) return;
    db.close();
    db.removeDatabase(database_name);

    QDialog::accept();
}
//--------------------------------------------------------------------------------
void FindLinks::init(void)
{
    connect_log();

    setWindowTitle(tr("Find"));

    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *label = new QLabel(tr("selector"));
    le  = new QLineEdit("div[id=search]");
    le2 = new QLineEdit("h3");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setCenterButtons(true);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    hbox->addWidget(label);
    hbox->addWidget(le);
    hbox->addWidget(le2);
    hbox->addWidget(buttonBox);
    setLayout(hbox);

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
