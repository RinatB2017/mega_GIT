/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef MYBROWSER_HPP
#define MYBROWSER_HPP
//--------------------------------------------------------------------------------
#include <QWebEngineCookieStore>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QNetworkProxy>

#include <QNetworkCookie>
#include <QNetworkReply>
//--------------------------------------------------------------------------------
#include "custompage.h"
#include "qxmlputget.h"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MyBrowser;
}
//--------------------------------------------------------------------------------
class MyBrowser : public MyWidget
{
    Q_OBJECT

public:
    explicit MyBrowser(QWidget *parent = nullptr);
    ~MyBrowser();

    void setUrl(const QUrl &url);
    void run(void);
    void run_javascript(QString javascript);

signals:
    void send(const QString &);

    void loadStarted(void);
    void loadFinished(bool);
    void loadProgress(int progress);

public slots:
    void get_document_title(void);

private:
    Ui::MyBrowser *ui;

#ifdef USE_CUSTOMPAGE
    QPointer<CustomPage> new_page;
#else
    QPointer<QWebEnginePage> new_page;
#endif

    QPointer<QWebEngineProfile> profile;

    QPointer<QWebEngineCookieStore> m_store;
    QVector<QNetworkCookie> m_cookies;

    void init(void);

    void popup(QPoint);
    void set_attributes(void);

    void load_proxies(void);
    void s_run(void);
    void refresh_url(const QUrl &url);
    bool containsCookie(const QNetworkCookie &cookie);
    void handleCookieAdded(const QNetworkCookie &cookie);

    void click(QWebEngineView * webView, QPoint pos, Qt::MouseButton button);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};

#endif // MYBROWSER_HPP
//--------------------------------------------------------------------------------
