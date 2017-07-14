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
#include <QWebView>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "listwidget_link.hpp"
//--------------------------------------------------------------------------------
ListWidget_Link::ListWidget_Link(QWebView *webview,
                                 QWidget *parent) :
    QListWidget(parent),
    web(webview)
{
    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
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
void ListWidget_Link::get_links(QModelIndex index)
{
    clear();

    QUrl baseUrl = web->page()->mainFrame()->baseUrl();
    QWebElementCollection news_collection = web->page()->mainFrame()->findAllElements("td[class=tnews]");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_collection.count()));
    foreach (QWebElement element, news_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl absoluteUrl = baseUrl.resolved(relativeUrl);

            add_item(element.toPlainText(),
                     absoluteUrl.toString(),
                     false);
        }
    }
    Q_UNUSED(index)
}
//--------------------------------------------------------------------------------
