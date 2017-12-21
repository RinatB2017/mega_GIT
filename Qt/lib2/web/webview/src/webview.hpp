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
#ifndef WEBVIEW_H
#define WEBVIEW_H
//--------------------------------------------------------------------------------
#include <QCheckBox>
#include <QWebView>
#include <QVector>
//--------------------------------------------------------------------------------
#include <QNetworkAccessManager>
#include <QNetworkReply>
//--------------------------------------------------------------------------------
struct ATTR
{
    QWebSettings::WebAttribute attribute;
    QCheckBox *object;
    QString name;
    QString tooltip;
    bool value;
};
//--------------------------------------------------------------------------------
class QBotNetworkAccessManager : public QNetworkAccessManager
{
public:
    QNetworkReply * createRequest(Operation op,
                                  const QNetworkRequest & req,
                                  QIODevice * outgoingData)
    {
        // копируем исходный QNetworkRequest & req
        QNetworkRequest myReq(req);

        // изменяем необходимые http-заголовки на свои значения
        //myReq.setRawHeader("User-Agent", "Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
        //myReq.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
        //myReq.setRawHeader("User-Agent", "Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
        myReq.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
        //myReq.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
        myReq.setRawHeader("Accept-Language", "en-us,en;q=0.5");
        //myReq.setRawHeader("Accept-Encoding", "gzip,deflate");
        myReq.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
        myReq.setRawHeader("Keep-Alive", "115");
        myReq.setRawHeader("Connection", "keep-alive");
        myReq.setRawHeader("Referer", "http://mysite.ru");

        // и прочие заголовки ...
        return QNetworkAccessManager::createRequest( op, myReq, outgoingData);
    }
};
//--------------------------------------------------------------------------------
class PersistentCookieJar;
class QContextMenuEvent;
//--------------------------------------------------------------------------------
class WebView : public QWebView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = 0);
    void load(const QUrl &url);
    PersistentCookieJar *get_cookies(void);
    QWebFrame *get_mainframe(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

protected:
    virtual void wheelEvent(QWheelEvent*);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void run_websettingbox(void);
    void go_home(void);
    void slot_finished(bool state);
    void goto_in(void);

private:
    PersistentCookieJar *cookies = 0;
    QVector<ATTR> attributes;
    QUrl home_page;

    void init(void);
    void load_setting(void);
    void save_setting(void);

protected:
    virtual QWebView *createWindow(QWebPage::WebWindowType type);
};
//--------------------------------------------------------------------------------
#endif // WEBVIEW_H
