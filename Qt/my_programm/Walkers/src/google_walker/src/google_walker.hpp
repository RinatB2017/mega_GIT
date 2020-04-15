/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef GOOGLE_WALKER_HPP
#define GOOGLE_WALKER_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineView>
//--------------------------------------------------------------------------------
class Google_walker : public QWidget
{
    Q_OBJECT

public:
    explicit Google_walker(QWidget *parent = nullptr);
    ~Google_walker();

    void setUrl(QUrl url);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send(const QString &);

private slots:
    void test_JS(bool);
    void find_url(const QString &text);

    void load_url(void);
    void run_js(const QString &javascript);

private:
    QPointer<QWebEngineView> viewer;
    QList<QString> urls;

    QString current_url;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // GOOGLE_WALKER_HPP
