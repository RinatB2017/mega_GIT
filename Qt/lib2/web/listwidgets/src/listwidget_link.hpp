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
#ifndef ListWidget_Link_HPP
#define ListWidget_Link_HPP
//--------------------------------------------------------------------------------
#include <QWebElement>
#include <QListWidget>
//--------------------------------------------------------------------------------
class QListWidgetItem;
class QWebView;
//--------------------------------------------------------------------------------
class ListWidget_Link : public QListWidget
{
    Q_OBJECT
public:
    ListWidget_Link(QWebView *webview,
                    QWidget *parent = nullptr);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void get_links(QModelIndex);

private slots:
    void log(const QString &text);
    void itemChanged(QListWidgetItem *item);
    void add_link(QAction *action);
    void doubleClicked(QModelIndex index);

private:
    QWebView *web;
    QString get_link(QWebElement element);

    void add_item(const QString &name,
                  const QString &address,
                  bool is_checked = false);
};
//--------------------------------------------------------------------------------
#endif // ListWidget_Link_HPP
