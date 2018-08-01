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
#ifndef YOUTUBE_WALKER_HPP
#define YOUTUBE_WALKER_HPP
//--------------------------------------------------------------------------------
#include <QWebEngineView>
#include <QWidget>
//--------------------------------------------------------------------------------
class Youtube_walker : public QWidget
{
    Q_OBJECT

public:
    explicit Youtube_walker(QWidget *parent = 0);
    ~Youtube_walker();

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

private:
    QWebEngineView *viewer = nullptr;
    QList<QString> urls;

    QString current_url;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // YOUTUBE_WALKER_HPP
