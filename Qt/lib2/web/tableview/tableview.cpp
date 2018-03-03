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
#include <QSqlQueryModel>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlRecord>
#include <QLineEdit>
#include <QDialog>
//--------------------------------------------------------------------------------
#include "tableview.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
    setWindowTitle("view table");
    setMinimumSize(1024, 600);
}
//--------------------------------------------------------------------------------
TableView::~TableView()
{
#ifdef QT_DEBUG
    qDebug() << "~TableView()";
#endif
    db.close();
    db.removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
void TableView::log(const QString &text)
{
#ifdef QT_DEBUG
    qDebug() << text;
#else
    Q_UNUSED(text);
#endif
}
//--------------------------------------------------------------------------------
void TableView::init(const QString &query)
{
    sql_query = query;
    driver_name = "QSQLITE";
    database_name = "customdb.db";

    if(db.databaseName() != database_name)
    {
        if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            db = QSqlDatabase::database();
        }
        else
        {
            db = QSqlDatabase::addDatabase(driver_name);
            db.setDatabaseName(database_name);
        }
    }

    bool ok = db.open();
    if(ok == false)
    {
        emit error(QString(tr("База данных %1 не открыта!")).arg(database_name));
        return;
    }

    model = new QSqlQueryModel;
    model->setQuery(QString(sql_query));

    setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    header->setResizeMode(QHeaderView::ResizeToContents);
#endif
    setHorizontalHeader(header);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}
//--------------------------------------------------------------------------------
void TableView::doubleClicked(QModelIndex index)
{
    emit debug(QString(tr("index %1")).arg(index.row()));
    QSqlRecord record = model->record(index.row());

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("dialog");
    dialog->setMinimumSize(800, 600);

    QVBoxLayout *vbox = new QVBoxLayout;
    QLineEdit *title = new QLineEdit(this);
    QTextBrowser *content = new QTextBrowser(this);

    title->setText(record.value("title").toString());
    content->setText(record.value("content").toString());

    vbox->addWidget(title);
    vbox->addWidget(content);
    dialog->setLayout(vbox);
    dialog->show();
}
//--------------------------------------------------------------------------------
