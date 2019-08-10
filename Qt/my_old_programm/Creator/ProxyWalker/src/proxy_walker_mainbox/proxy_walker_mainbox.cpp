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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QWebElement>
#include <QWebFrame>

#include <QLCDNumber>
#include <QProcess>
//--------------------------------------------------------------------------------
#include "ui_proxy_walker_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "javascript.hpp"
#include "qxmlputget.h"
#include "defines.hpp"
#include "webview.hpp"
#include "waiting.hpp"
#include "proxy_walker_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets(APPNAME);

    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    webview = new WebView(this);
    connect(webview, SIGNAL(titleChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(webview, SIGNAL(loadStarted()), this, SLOT(web_started()));
    connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(web_finished(bool)));
    connect(webview, SIGNAL(loadProgress(int)), web_progressBar, SLOT(setValue(int)));
    if(mw)
        connect(webview, SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));
    main_frame = webview->page()->mainFrame();

    proxy_list = new QListWidget(this);

#if 1
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->addWidget(proxy_list);
    splitter->addWidget(webview);

    ui->hbox->addWidget(splitter);
#else
    ui->hbox->addWidget(proxy_list, 1);
    ui->hbox->addWidget(webview, 7);
#endif

    //load_setting();
    if(current_index > 0)
        proxy_list->setCurrentRow(current_index);

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    // qDebug() << "(MainWindow *)" << mw;
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_get_proxies_list = add_button(testbar,
                                                   new QToolButton(this),
                                                   QIcon(),
                                                   "get_proxies_list",
                                                   "get_proxies_list");
    QToolButton *btn_check_proxies_list = add_button(testbar,
                                                     new QToolButton(this),
                                                     QIcon(),
                                                     "check_proxies_list",
                                                     "check_proxies_list");
    QToolButton *btn_walker = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "walker",
                                         "walker");
    QToolButton *btn_stop = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaStop),
                                       "stop",
                                       "stop");

    web_progressBar = new QProgressBar(this);
    web_progressBar->setToolTip(tr("web progress"));

    result_progressBar = new QProgressBar(this);
    result_progressBar->setToolTip(tr("result progress"));

    testbar->addWidget(web_progressBar);
    testbar->addWidget(result_progressBar);

    connect(btn_get_proxies_list, SIGNAL(clicked()), this, SLOT(get_proxies_list()));
    connect(btn_check_proxies_list, SIGNAL(clicked()), this, SLOT(check_proxies_list()));
    connect(btn_walker, SIGNAL(clicked()), this, SLOT(run_walker()));
    connect(btn_stop, SIGNAL(clicked()), this, SLOT(stop()));
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    global_stop_flag = true;
    is_loaded = true;
    webview->stop();
    web_progressBar->reset();
}
//--------------------------------------------------------------------------------
void MainBox::get_proxies(QStringList *proxies,
                          const QString &address)
{
    if(global_stop_flag) return;

    load_url(address, true);
    QWebElement select = webview->page()->mainFrame()->findFirstElement("a[class=select]");
    if(select.isNull())
    {
        emit error("select not found");
        return;
    }
    JavaScript::click_element(select);
    Waiting::sec(1);

    QWebElement textarea = webview->page()->mainFrame()->findFirstElement("textarea[name=t1]");
    if(textarea.isNull())
    {
        emit error("textarea not found");
        return;
    }

    QVariant result = webview->page()->mainFrame()->evaluateJavaScript("document.getElementsByName('t1')[0].value");
    QStringList sl = result.toString().split("\n");
    foreach (QString uri, sl)
    {
        if(uri.isEmpty() == false)
        {
            proxies->append(uri);
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::append_proxy(const QString &ip_and_port)
{
    if(ip_and_port.isEmpty())
    {
        emit error(tr("no data"));
        return false;
    }

    QStringList sl = ip_and_port.split(":");
    if(sl.count() != 2)
    {
        emit error(tr("unknown format"));
        return false;
    }

    QString Ip = sl.at(0);
    if(Ip.isEmpty())
    {
        emit error(tr("Ip not found"));
        return false;
    }

    bool ok;
    int port = sl.at(1).toInt(&ok);
    if(ok == false)
    {
        emit error(tr("port error"));
        return false;
    }

    QUrl url(Ip);
    if(url.isValid() == false)
    {
        emit error(tr("error url"));
        return false;
    }

    //emit info(QString(tr("Proxy %1:%2")).arg(Ip).arg(port));

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(Ip);
    proxy.setPort(port);
    //    QNetworkProxy::setApplicationProxy(proxy);
    webview->page()->networkAccessManager()->setProxy(proxy);

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::get_proxies_list(void)
{
    global_stop_flag = false;

    QStringList proxies;
    proxies.clear();

    get_proxies(&proxies, "http://nntime.com/proxy-list-01.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-02.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-03.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-04.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-05.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-06.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-07.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-08.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-09.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-10.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-11.htm");
    get_proxies(&proxies, "http://nntime.com/proxy-list-12.htm");

    emit debug(QString("found %1 proxy").arg(proxies.count()));
    proxies.sort();
    proxy_list->clear();
    foreach (QString proxy, proxies)
    {
        proxy_list->addItem(proxy);
    }
    emit info("The end!");
}
//--------------------------------------------------------------------------------
void MainBox::check_proxies_list(void)
{
    global_stop_flag = false;

    if(proxy_list->count() == 0)
    {
        emit error(tr("proxies list is empty"));
        return;
    }
    emit info(QString("found %1 proxy").arg(proxy_list->count()));

    QStringList clean_proxies_list;
    clean_proxies_list.clear();
    result_progressBar->setMaximum(proxy_list->count());
    if(current_index < 0) current_index = 0;

    QMap <int, QString> map;
    int full_time = 0;
    for(int n=current_index; n<proxy_list->count(); n++)
    {
        if(global_stop_flag) continue;
        result_progressBar->setValue(n);
        proxy_list->setCurrentRow(n);
        QString proxy = proxy_list->currentItem()->text();
        if(proxy.isEmpty() == false)
        {
            emit info(QString(tr("Обрабатываем %1 из %2 [%3]"))
                      .arg(n)
                      .arg(proxy_list->count())
                      .arg(proxy));
            bool ok = append_proxy(proxy);
            if(ok)
            {
                QTime time;
                time.start();
                load_url("https://www.google.ru/"); //URL_MAIN_PAGE);
                QWebElement title = main_frame->findFirstElement("title");
                if(title.isNull() == false)
                {
                    if(title.toPlainText() == "Google") //"Персональный сайт - Главная страница")
                    {
                        int time_elapsed = time.elapsed();
                        full_time += time_elapsed;
                        emit info(QString("proxy %1 is valid! elapsed %2 msec ")
                                  .arg(proxy)
                                  .arg(time_elapsed));
                        map.insert(time_elapsed, proxy);
                        clean_proxies_list << proxy;
                    }
                    else
                    {
                        emit error(QString("proxy %1 is NOT valid!").arg(proxy));
                    }
                }
            }
        }
    }
    emit info(QString(tr("Прошло %1")).arg(full_time));
    QMapIterator<int, QString> i(map);
    while (i.hasNext())
    {
        i.next();
        emit info(QString("time %1 proxy %2")
                  .arg(i.key())
                  .arg(i.value()));
    }

    if(global_stop_flag) return;
    proxy_list->clear();
    clean_proxies_list.sort();
    foreach (QString proxy, clean_proxies_list)
    {
        proxy_list->addItem(proxy);
    }
}
//--------------------------------------------------------------------------------
class CorrectNetworkAccessManager : public QNetworkAccessManager
{
protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
    {
        QNetworkRequest myReq(request);
        myReq.setRawHeader("User-Agent",  "myAgent");
        myReq.setRawHeader("Referer", "http://www.prog.org.ru/index.php?topic=11629.msg71840#msg71840");
        return createRequest(op, myReq, outgoingData);
    }
};
//--------------------------------------------------------------------------------
#include <QWebView>
void MainBox::run_walker(void)
{
#if 0
    QWebView *view = new QWebView();
    view->load(QUrl("http://psixpom.jimdo.com/"));
    view->show();
    return;
#endif

#if 1
    load_url("http://psixpom.jimdo.com/");
    return;
#endif

#if 0
    //webview->page()->setNetworkAccessManager(new CorrectNetworkAccessManager());
    //URL_MAIN_PAGE "http://linux.ucoz.com/index/0-2"

    global_stop_flag = false;

    //QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, false);

    for(int n=0; n<4; n++)
    {
        //append_proxy(proxy_list->item(n)->text());
        //load_url(webviews[n].webview, "http://linux.ucoz.com/index/0-2", true, true);
        load_url("http://leader.ru/secure/who.html", true);
    }
#endif

#if 0
    global_stop_flag = false;

    result_progressBar->setMaximum(proxy_list->count());
    if(current_index < 0) current_index = 0;
    for(int n=current_index; n<proxy_list->count(); n++)
    {
        if(global_stop_flag) return;
        result_progressBar->setValue(n);
        proxy_list->setCurrentRow(n);
        QString proxy = proxy_list->currentItem()->text();
        if(proxy.isEmpty() == false)
        {
            emit debug(QString(tr("Обрабатываем %1 из %2 [%3]"))
                       .arg(n)
                       .arg(proxy_list->count())
                       .arg(proxy));
            bool ok = append_proxy(proxy);
            if(ok)
            {
                test();
            }
        }
    }
#endif    
}
//--------------------------------------------------------------------------------
void MainBox::web_started(void)
{
    is_loaded = false;
}
//--------------------------------------------------------------------------------
void MainBox::web_finished(bool state)
{
    if(state == false) return;
    web_progressBar->reset();
    is_loaded = true;
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    if(global_stop_flag) return;

    QString current_url = URL_MAIN_PAGE;
    QUrl beginUrl = QUrl(current_url);

    int n=0;
    int max_n = 10 + (qrand() % 20);
    while(n < max_n)
    {
        if(global_stop_flag) return;
        emit info(QString(tr("left %1")).arg(max_n - n));
        load_url(current_url, true);

        QUrl baseUrl = main_frame->baseUrl();

        QWebElementCollection links_collection = main_frame->findAllElements(":link");
        QStringList sl;
        sl.clear();
        foreach (QWebElement element, links_collection)
        {
            QString href = element.attribute("href");
            if (!href.isEmpty())
            {
                QUrl relativeUrl(href);
                QUrl absoluteUrl = baseUrl.resolved(relativeUrl);

                int res = beginUrl.toString().compare(absoluteUrl.toString().left(beginUrl.toString().length()));
                if(res == 0)
                {
                    sl << absoluteUrl.toString();
                }
            }
        }
        if(sl.count() != 0)
        {
            int index = qrand() % sl.count();
            current_url = sl.at(index);
        }
        else
        {
            current_url = beginUrl.toString();
        }
        int waiting_time = 1 + (qrand() % 10);
        emit info(QString(tr("waiting %1 sec")).arg(waiting_time));
        Waiting::sec(waiting_time);
        n++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_url(const QString &address,
                       bool show_address)
{
    if(global_stop_flag) return;

    if(webview == 0)
    {
        emit error("webview not init");
        return;
    }

    if(show_address)
        emit info(QString(tr("Загружаем %1")).arg(address));
    is_loaded = false;
    webview->load(address);
    webview->show();

    Waiting::is_loaded(&is_loaded);
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    bool ok = false;
    QXmlGet xmlGet;
    if(!QFile(PROXIES_XML).exists())
    {
        emit error(QString(tr("file %1 not exists")).arg(PROXIES_XML));
        return;
    }

    QString error_message;
    int error_line;
    int error_column;
    ok = xmlGet.load(PROXIES_XML, &error_message, &error_line, &error_column);
    if(ok == false)
    {
        emit error(QString(tr("file %1 not load ERROR: %2 line %3 column %4"))
                   .arg(PROXIES_XML)
                   .arg(error_message)
                   .arg(error_line)
                   .arg(error_column));
        return;
    }
    if (xmlGet.find("index"))
    {
        current_index = xmlGet.getString().toInt(&ok);
        if(ok == false) current_index = 0;
    }

    while(xmlGet.findNext("proxy"))
    {
        xmlGet.descend();
        if (xmlGet.find("uri"))
        {
            QString uri = xmlGet.getString();
            if(uri.isEmpty() == false)
            {
                proxy_list->addItem(uri);
            }
        }
        xmlGet.rise();
    }
    emit info(QString(tr("load %1 proxy")).arg(proxy_list->count()));
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
#if 1
    if(proxy_list->count() == 0) return;

    qDebug() << "save_setting";

    QXmlPut *xmlPut = new QXmlPut("Proxies");
    current_index = proxy_list->currentRow();
    if(current_index < 0) current_index = 0;
    xmlPut->putString("index", QString("%1").arg(current_index));
    for(int n=0; n<proxy_list->count(); n++)
    {
        xmlPut->descend("proxy");
        xmlPut->putString("uri", proxy_list->item(n)->text());
        xmlPut->rise();
    }
    xmlPut->save(PROXIES_XML);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
