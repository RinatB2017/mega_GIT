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
#include <QWebElementCollection>
#include <QListWidgetItem>
#include <QApplication>
#include <QWebFrame>
#include <QSqlQuery>
#include <QWebView>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "listwidget_link.hpp"
#include "database.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
ListWidget_Link::ListWidget_Link(QWebView *webview,
                                 QWidget *parent) :
    QListWidget(parent),
    web(webview)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parent, SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }

    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}
//--------------------------------------------------------------------------------
void ListWidget_Link::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void ListWidget_Link::doubleClicked(QModelIndex index)
{
    QString address = item(index.row())->data(Qt::UserRole).toString();
    web->load(address);
    emit info(QString("%1").arg(address));
}
//--------------------------------------------------------------------------------
void ListWidget_Link::add_item(const QString &name,
                               const QString &address,
                               bool is_checked)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::DisplayRole, name);
    item->setData(Qt::UserRole, address);
    item->setData(Qt::CheckStateRole, is_checked ? Qt::Checked : Qt::Unchecked);
    addItem(item);
}
//--------------------------------------------------------------------------------
void ListWidget_Link::itemChanged(QListWidgetItem *item)
{
    emit debug(QString("%1 %2")
               .arg(item->text())
               .arg(item->data(Qt::CheckStateRole).toBool()));
}
//--------------------------------------------------------------------------------
void ListWidget_Link::add_link(QAction *action)
{
    emit error(tr("Пока не реализовано!"));
    Q_UNUSED(action)
}
//--------------------------------------------------------------------------------
QString ListWidget_Link::get_link(QWebElement element)
{
    QUrl baseUrl = web->page()->mainFrame()->baseUrl();
    QWebElement temp = element.findFirst(":link");
    QString href = temp.attribute("href");
    QUrl relativeUrl(href);
    QUrl absoluteUrl = baseUrl.resolved(relativeUrl);

    return absoluteUrl.toString();
}
//--------------------------------------------------------------------------------
void ListWidget_Link::get_links(QModelIndex)
{
    clear();

    Database *db = new Database("QSQLITE", "customdb.db", this);

    if(db->open() == false)
        return;
    QSqlQuery temp_sql;

    QWebFrame *main_frame = web->page()->mainFrame();
    QUrl baseUrl = main_frame->baseUrl();

    if(baseUrl == QUrl("http://www.opennet.ru/"))
    {
        QWebElementCollection news_collection = main_frame->findAllElements("td[class=tnews]");
        emit info(QString(tr("Нашли %1 ссылок")).arg(news_collection.count()));
        foreach (QWebElement link, news_collection)
        {
            QWebElement span = link.findFirst("span[class=cnt]");
            QString clean_name = link.toPlainText().remove(span.toPlainText()).simplified();
            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'")
                          .arg(clean_name));
            add_item(clean_name,
                     get_link(link),
                     temp_sql.first());
        }
        db->close();
        return;
    }

    if(baseUrl == QUrl("http://www.opennet.ru/#mini"))
    {
        QWebElementCollection news_collection = main_frame->findAllElements("td[class=tnews]");
        emit info(QString(tr("Нашли %1 ссылок")).arg(news_collection.count()));
        foreach (QWebElement link, news_collection)
        {
            QWebElement span = link.findFirst("span[class=cnt]");
            QString clean_name = link.toPlainText().remove(span.toPlainText()).simplified();
            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'")
                          .arg(clean_name));
            add_item(clean_name,
                     get_link(link),
                     temp_sql.first());
        }
        db->close();
        return;
    }

    if(baseUrl == QUrl("http://www.securitylab.ru/"))
    {
        QWebElementCollection main_content = main_frame->findAllElements("div[class=main_content]");
        emit info(QString(tr("Нашли %1 ссылок")).arg(main_content.count()));
        foreach (QWebElement el, main_content)
        {
            QWebElementCollection h1 = el.findAll("h1");
            foreach (QWebElement link, h1)
            {
                QString clean_name = link.toPlainText().simplified();
                temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'")
                              .arg(clean_name));
                add_item(clean_name,
                         get_link(link),
                         temp_sql.first());
            }
            QWebElementCollection h2 = el.findAll("h2");
            foreach (QWebElement link, h2)
            {
                QString clean_name = link.toPlainText().simplified();
                temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'")
                              .arg(clean_name));
                add_item(clean_name,
                         get_link(link),
                         temp_sql.first());
            }
        }
        db->close();
        return;
    }

    emit error(QString(tr("url %1 unknown"))
               .arg(baseUrl.toString()));
    db->close();
}
//--------------------------------------------------------------------------------
