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
#if 0
https://8500.ru/user-agent/

Mozilla Firefox 36 (Win 8.1 x64): Mozilla/5.0 (Windows NT 6.3; WOW64; rv:36.0) Gecko/20100101 Firefox/36.0
Google Chrome 53 (Win 10 x64): Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36
Google Chrome 40 (Win 8.1 x64): Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2214.115 Safari/537.36
Opera 40 (Win 10 x64): Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.101 Safari/537.36 OPR/40.0.2308.62
Opera 12.17 (Win 8 x64): Opera/9.80 (Windows NT 6.2; WOW64) Presto/2.12.388 Version/12.17
Apple Safari 5.1 (Win 8 x64): Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2
Internet Explorer 11 (Win 10 x64): Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; .NET4.0E; rv:11.0) like Gecko
Internet Explorer 11 (Win 8.1 x64): Mozilla/5.0 (Windows NT 6.3; WOW64; Trident/7.0; ASU2JS; rv:11.0) like Gecko
Microsoft Edge (Win 10 x64): Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2486.0 Safari/537.36 Edge/13.10586

Мобильные браузеры:

iPhone: Mozilla/5.0 (iPhone; CPU iPhone OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25
Android 2.3.5 (Philips W632): Mozilla/5.0 (Linux; U; Android 2.3.5; ru-ru; Philips W632 Build/GRJ90) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1

Планшеты:

iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25

Поисковые системы:

Яндекс: Mozilla/5.0 (compatible; YandexBot/3.0; +http://yandex.com/bots)
Google: Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)
Bing: Mozilla/5.0 (compatible; bingbot/2.0; +http://www.bing.com/bingbot.htm)
Yahoo! Slurp: Mozilla/5.0 (compatible; Yahoo! Slurp; http://help.yahoo.com/help/us/ysearch/slurp)
Mail.ru: Mozilla/5.0 (compatible; Linux x86_64; Mail.RU_Bot/Fast/2.0; +http://go.mail.ru/help/robots)
Sputnik: Mozilla/5.0 (compatible; SputnikBot/2.3; +http://corp.sputnik.ru/webmaster)

Ahrefs: Mozilla/5.0 (compatible; AhrefsBot/5.0; +http://ahrefs.com/robot/)
Majestic: Mozilla/5.0 (compatible; MJ12bot/v1.4.5; http://www.majestic12.co.uk/bot.php?+)
SMTBot: Mozilla/5.0 (compatible; SMTBot/1.0; +http://www.similartech.com/smtbot)
linkdex: Mozilla/5.0 (compatible; linkdexbot/2.2; +http://www.linkdex.com/bots/)
Exabot: Mozilla/5.0 (compatible; Exabot/3.0; +http://www.exabot.com/go/robot)
StatOnline.ru: Mozilla/5.0 (compatible; StatOnlineRuBot/1.0)
#endif
//--------------------------------------------------------------------------------
//TODO подмена ОС
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
        //myReq.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
        //myReq.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
        myReq.setRawHeader("User-Agent", "iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");

#if 0
        //myReq.setRawHeader("Accept-Encoding", "gzip,deflate");
        myReq.setRawHeader("Accept-Language", "en-us,en;q=0.5");
        myReq.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
        myReq.setRawHeader("Keep-Alive", "115");
        myReq.setRawHeader("Connection", "keep-alive");
        myReq.setRawHeader("Referer", "http://mysite.ru");
#endif

        // и прочие заголовки ...
        return QNetworkAccessManager::createRequest( op, myReq, outgoingData );
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
    void wheelEvent(QWheelEvent*);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void run_websettingbox(void);
    void go_home(void);
    void slot_finished(bool state);
    void goto_in(void);

private:
    PersistentCookieJar *cookies;
    QVector<ATTR> attributes;
    QUrl home_page;

    void init(void);
    void load_setting(void);
    void save_setting(void);

protected:
    QWebView *createWindow(QWebPage::WebWindowType type);
};
//--------------------------------------------------------------------------------
#endif // WEBVIEW_H
