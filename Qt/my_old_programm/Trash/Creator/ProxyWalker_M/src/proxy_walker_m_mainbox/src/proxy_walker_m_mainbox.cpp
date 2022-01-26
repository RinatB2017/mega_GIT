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
#include "ui_proxy_walker_m_mainbox.h"
//--------------------------------------------------------------------------------
#include "proxy_walker_m_mainbox.hpp"
#include "defines.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    create_menu();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QTabWidget *tw = new QTabWidget(this);
    for(int n=0; n<MAX_TAB; n++)
    {
        web_struct web;
        web.progressBar = new QProgressBar(this);
        web.progressBar->setToolTip(tr("web progress"));
        web.webview = new WebView(this);
        connect(web.webview, SIGNAL(loadStarted()), this, SLOT(web_started()));
        connect(web.webview, SIGNAL(loadFinished(bool)), this, SLOT(web_finished(bool)));
        connect(web.webview, SIGNAL(loadProgress(int)), web.progressBar, SLOT(setValue(int)));
        connect(web.webview, SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));

        QFrame *frame = new QFrame(this);
        QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget(web.progressBar);
        vbox->addWidget(web.webview);
        frame->setLayout(vbox);
        tw->addTab(frame, QString(tr("webview %1")).arg(n));

        web.main_frame = web.webview->page()->mainFrame();
        web.is_loaded = false;
        QRandomGenerator generator;
        web.count = 10 + (generator.generate() % 10);

        webviews.append(web);
    }
    proxy_list = new QListWidget(this);

#if 1
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->addWidget(proxy_list);
    splitter->addWidget(tw);

    ui->hbox->addWidget(splitter);
#else
    ui->hbox->addWidget(proxy_list, 1);
    ui->hbox->addWidget(tw, 7);
#endif

    if(current_index > 0)
    {
        proxy_list->setCurrentRow(current_index);
    }

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::create_menu(void)
{
#if 1   //FIXME
    save_action = new QAction(this);
    save_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(save_action, SIGNAL(triggered()), this, SLOT(save_setting()));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    mw->add_filemenu_action(0, save_action);
    mw->add_filemenu_separator(1);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

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

    result_progressBar = new QProgressBar(this);
    result_progressBar->setToolTip(tr("result progress"));

    testbar->addWidget(result_progressBar);

    connect(btn_get_proxies_list,   SIGNAL(clicked(bool)), this, SLOT(get_proxies_list()));
    connect(btn_check_proxies_list, SIGNAL(clicked(bool)), this, SLOT(check_proxies_list()));
    connect(btn_walker,             SIGNAL(clicked(bool)), this, SLOT(run_walker()));
    connect(btn_stop,               SIGNAL(clicked(bool)), this, SLOT(stop()));
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    global_stop_flag = true;
    for(int n=0; n<webviews.count(); n++)
    {
        webviews[n].is_loaded = true;
        webviews[n].webview->stop();
        webviews[n].progressBar->reset();
    }
}
//--------------------------------------------------------------------------------
void MainBox::get_proxies(QStringList *proxies,
                          const QString &address)
{
    // данные получаем на 0 странице
    if(global_stop_flag) return;

    webviews[0].time.start();
    load_url(0, address, true);
    QWebElement select = webviews[0].main_frame->findFirstElement("a[class=select]");
    if(select.isNull())
    {
        emit error("select not found");
        return;
    }
    JavaScript::click_element(select);
    Waiting::sec(1);

    QWebElement textarea = webviews[0].main_frame->findFirstElement("textarea[name=t1]");
    if(textarea.isNull())
    {
        emit error("textarea not found");
        return;
    }

    QVariant result = webviews[0].main_frame->evaluateJavaScript("document.getElementsByName('t1')[0].value");
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
bool MainBox::append_proxy(int index,
                           const QString &ip_and_port)
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

    emit info(QString(tr("Proxy %1:%2")).arg(Ip).arg(port));

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(Ip);
    proxy.setPort(port);
    webviews[index].webview->page()->networkAccessManager()->setProxy(proxy);
    //QNetworkProxy::setApplicationProxy(proxy);

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::get_proxies_list(void)
{
    int button = QMessageBox::question(this, tr("Коллекция прокси"), tr("Собираем коллекцию прокси?"), QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
    {
        return;
    }

    global_stop_flag = false;

    QStringList new_proxy_list;

    //---
#if 1
    QFile file("socks5_all.txt");
    bool ok = file.open(QIODevice::ReadOnly);
    if(ok)
    {
        while (!file.atEnd())
        {
            QString temp = file.readLine();
            if(!temp.isEmpty())
            {
                //emit info(temp);
                new_proxy_list.append(temp);
            }
        }
    }
#else
    QStringList old_proxy_list;
    QStringList found_proxy_list;

    old_proxy_list.clear();
    for(int n=0; n<proxy_list->count(); n++)
    {
        old_proxy_list.append(proxy_list->item(n)->text());
    }
    emit debug(QString("found %1 old proxy").arg(old_proxy_list.count()));
    //---

    //---
    found_proxy_list.clear();
    for(int n=1; n<13; n++)
    {
        webviews[0].time.start();
        load_url(0, QString("http://www.freeproxylists.net/ru/?c=&pt=&pr=HTTP&a[]=0&a[]=1&a[]=2&u=0&page=%1").arg(n), true);
        QWebElementCollection proxy_collection = webviews[0].main_frame->findAllElements("tr[class=Odd]");
        foreach (QWebElement element, proxy_collection)
        {
            QWebElementCollection td_collection = element.findAll("td");
            if(td_collection.count() == 10)
            {
                QString proxy = td_collection.at(0).toPlainText();
                QString port  = td_collection.at(1).toPlainText();
                QString uri = QString("%1:%2")
                        .arg(proxy)
                        .arg(port);
                found_proxy_list.append(uri);
            }
        }
        QWebElementCollection proxy_collection2 = webviews[0].main_frame->findAllElements("tr[class=Even]");
        foreach (QWebElement element, proxy_collection2)
        {
            QWebElementCollection td_collection = element.findAll("td");
            if(td_collection.count() == 10)
            {
                QString proxy = td_collection.at(0).toPlainText();
                QString port  = td_collection.at(1).toPlainText();
                QString uri = QString("%1:%2")
                        .arg(proxy)
                        .arg(port);
                found_proxy_list.append(uri);
            }
        }
        Waiting::sec(5);
    }

    get_proxies(&found_proxy_list, "http://nntime.com/");
    for(int n=1; n<12; n++)
    {
        get_proxies(&found_proxy_list, QString("http://nntime.com/proxy-list-%1.htm").arg(n, 2, 10, QLatin1Char('0')));
    }

    emit debug(QString("found %1 proxy").arg(found_proxy_list.count()));
    //---

    //---
    new_proxy_list.clear();
    foreach (QString old_proxy, old_proxy_list)
    {
        bool found = false;
        foreach (QString new_proxy, found_proxy_list)
        {
            if(new_proxy == old_proxy)
            {
                found = true;
                break;
            }
        }
        if(found == false)
        {
            new_proxy_list.append(old_proxy);
        }
    }
#endif
    //---

    //---
#if 0
    new_proxy_list.clear();
    for(int n=0; n<proxy_list->count(); n++)
    {
        new_proxy_list.append(proxy_list->item(n)->text());
    }
    emit debug(QString("found %1 old proxy").arg(new_proxy_list.count()));

    QStringList temp_proxy_list;
    temp_proxy_list.clear();
    for(int n=0; n<proxy_list->count(); n++)
    {
        temp_proxy_list.append(new_proxy_list.at(n).toAscii());
    }
    new_proxy_list.clear();
    foreach (QString temp_proxy, temp_proxy_list)
    {
        bool found = false;
        foreach (QString new_proxy, new_proxy_list)
        {
            if(new_proxy == temp_proxy)
            {
                found = true;
                break;
            }
        }
        if(found == false)
            new_proxy_list.append(temp_proxy);
    }
    emit debug(QString("found %1 new proxy").arg(new_proxy_list.count()));
#endif
    //---

    proxy_list->clear();
    foreach (QString proxy, new_proxy_list)
    {
        proxy_list->addItem(proxy);
    }
    //proxy_list->sortItems();
    emit debug(QString("found %1 proxy").arg(new_proxy_list.count()));
    emit debug("The end!");
}
//--------------------------------------------------------------------------------
int MainBox::check_url(int index,
                       const QString &check_string)
{
    if(webviews[index].time.elapsed() >= MAX_TIMEOUT)
    {
        return E_ERROR_TIMEOUT;
    }

    if(webviews[index].is_block)
    {
        return E_ERROR_IS_BLOCK;
    }

    if(webviews[index].is_loaded == true)
    {
        QWebElement title = webviews[index].main_frame->findFirstElement("title");
        if(title.isNull() == false)
        {
            if(title.toPlainText() == check_string)
            {
                int time_elapsed = webviews[index].time.elapsed();
                QString proxy = proxy_list->item(current_index)->text();
                emit info(QString("proxy %1 is valid! elapsed %2 msec ")
                          .arg(proxy)
                          .arg(time_elapsed));
                return E_NO_ERROR;
            }
            else
            {
                QString hostname = webviews[index].webview->page()->networkAccessManager()->proxy().hostName();
                int port = webviews[index].webview->page()->networkAccessManager()->proxy().port();
                emit error(QString("proxy %1:%2 is NOT valid!")
                           .arg(hostname)
                           .arg(port));
                return E_ERROR_NO_PAGE;
            }
        }
    }

    return E_ERROR_NOT_LOADED;
}
//--------------------------------------------------------------------------------
int MainBox::next_proxy(int index)
{
    if(index < (proxy_list->count() - 1))
    {
        return (index + 1);
    }
    return -1;
}
//--------------------------------------------------------------------------------
void MainBox::init_webview(int index,
                           const QString &proxy,
                           QUrl url)
{
    webviews[index].is_block = false;
    webviews[index].is_loaded = false;
    webviews[index].progressBar->reset();
    append_proxy(index, proxy);
    webviews[index].time.start();
    load_url(index, url.toString(), true, false);
}
//--------------------------------------------------------------------------------
void MainBox::check_proxies_list(void)
{
    int button = QMessageBox::question(this, tr("Коллекция прокси"), tr("Проверяем коллекцию прокси?"), QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
    {
        return;
    }

    result_progressBar->setMaximum(proxy_list->count());
    for(int n=0; n<MAX_TAB; n++)
    {
        init_webview(n, proxy_list->item(n)->text(), QUrl(CHECK_PAGE));
    }
    result_progressBar->setValue(MAX_TAB);

    current_index = MAX_TAB;
    proxy_list->setCurrentRow(current_index);

    QStringList clean_proxies_list;
    clean_proxies_list.clear();
    QMap <int, QString> map;
    int good_proxy = 0;
    int bad_proxy = 0;
    int res = 0;
    QString proxy = "";
    int time_elapsed = 0;
    int index = 0;

    while(true)
    {
        if(global_stop_flag)
        {
            return;
        }

        int err = check_url(index, "Google");
        switch (err)
        {
        case E_NO_ERROR:
            if(webviews[index].is_block == false)
            {
                good_proxy++;
                proxy = proxy_list->item(current_index)->text();
                time_elapsed = webviews[index].time.elapsed();
                map.insert(time_elapsed, proxy);
                clean_proxies_list << proxy;
                res = next_proxy(current_index);
                if(res > 0)
                {
                    current_index = res;
                    proxy_list->setCurrentRow(current_index);
                    result_progressBar->setValue(current_index);
                    init_webview(index, proxy_list->item(current_index)->text(), QUrl(CHECK_PAGE));
                }
                else
                {
                    webviews[index].is_block = true;
                }
            }
            break;

        case E_ERROR_TIMEOUT:
        case E_ERROR_NO_PAGE:
            if(webviews[index].is_block == false)
            {
                bad_proxy++;
                emit error(QString("index %1 err %2 good_proxy %3 bad_proxy %4")
                           .arg(index)
                           .arg(err)
                           .arg(good_proxy)
                           .arg(bad_proxy));
                res = next_proxy(current_index);
                if(res > 0)
                {
                    current_index = res;
                    proxy_list->setCurrentRow(current_index);
                    result_progressBar->setValue(current_index);
                    init_webview(index, proxy_list->item(current_index)->text(), QUrl(CHECK_PAGE));
                }
                else
                {
                    webviews[index].is_block = true;
                }
            }
            break;

        case E_ERROR_NOT_LOADED:
            break;

        default:
            break;
        }

        if(index < (MAX_TAB - 1)) index++;
        else index = 0;
        QCoreApplication::processEvents();
        //emit info("The end!");
        if(is_not_working())
        {
            emit info(QString(tr("all proxy %1")).arg(proxy_list->count()));
            emit info(QString(tr("good proxy %1")).arg(good_proxy));
            emit info(QString(tr("bad proxy %1")).arg(bad_proxy));

            if(global_stop_flag) return;
            QMapIterator<int, QString> i(map);
            proxy_list->clear();
            while (i.hasNext())
            {
                i.next();
                //emit info(QString("time %1 proxy %2").arg(i.key()).arg(i.value()));
                proxy_list->addItem(i.value());
            }
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_walker(void)
{
    QRandomGenerator generator;

    int button = QMessageBox::question(this, tr("run walker"), tr("Run walker?"), QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
    {
        return;
    }

    for(int n=0; n<MAX_TAB; n++)
    {
        webviews[n].is_block = false;
        webviews[n].is_loaded = false;
        webviews[n].progressBar->reset();
        webviews[n].count = 5 + (generator.generate() % 5);
        append_proxy(n, proxy_list->item(n)->text());
        webviews[n].time.start();
        load_url(n, URL_MAIN_PAGE, true, false);
    }

    result_progressBar->setMaximum(proxy_list->count());
    int index = 0;
    current_index = MAX_TAB;
    while(true)
    {
        if(global_stop_flag) return;

        if(webviews[index].time.elapsed() < MAX_TIMEOUT)
        {
            if(webviews[index].is_loaded == true)
            {
                if(webviews[index].count == 0)
                {
                    if(webviews[index].is_block == false)
                    {
                        webviews[index].count = 5 + (generator.generate() % 5);
                        append_proxy(index, proxy_list->item(current_index)->text());
                        load_url(index, URL_MAIN_PAGE, true, false);
                        proxy_list->setCurrentRow(current_index);
                        if(current_index < (proxy_list->count() - 1))
                            current_index ++;
                        else
                            webviews[index].is_block = true;
                        result_progressBar->setValue(current_index);

                    }
                }
                else
                {
                    webviews[index].count --;
                    load_url(index, get_random_url(index), true, false);
                }
            }
        }
        else
        {
            emit error(QString(tr("%1 error TIMEOUT")).arg(index));
            webviews[index].is_loaded = true;
            webviews[index].time.start();
        }

        if(index < (MAX_TAB - 1)) index++;
        else index = 0;
        QCoreApplication::processEvents();
        if(is_not_working())
        {
            emit info("The end!");
            return;
        }
    }

#if 0
    //URL_MAIN_PAGE "http://linux.ucoz.com/index/0-2"

    global_stop_flag = false;

    //QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, false);

    timer_count=0;
    main_timer->start(1000);
    for(int n=0; n<MAX_TAB; n++)
    {
        append_proxy(webviews[n].webview, proxy_list->item(n+1)->text());
        webviews[n].time.start();
        //load_url(webviews[n].webview, "http://linux.ucoz.com/index/0-2", true, false);
        load_url(webviews[n].webview, "http://leader.ru/secure/who.html", true, false);
    }
#endif

#if 0
    result_progressBar->setMaximum(proxy_list->count());
    for(int n=current_index; n<proxy_list->count(); n++)
    {
        if(global_stop_flag) continue;
        result_progressBar->setValue(n);
        proxy_list->setCurrentRow(n);
        QString proxy = proxy_list->currentItem()->text();
        if(proxy.isEmpty() == false)
        {
            emit debug(QString(tr("Обрабатываем %1 из %2 [%3]"))
                       .arg(n)
                       .arg(proxy_list->count())
                       .arg(proxy));
            bool ok = append_proxy(0, proxy);
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
    QWebView *webview = (QWebView *)sender();
    for(int n=0; n<webviews.count(); n++)
    {
        if(webview == webviews[n].webview)
        {
            webviews[n].is_loaded = false;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::web_finished(bool state)
{
    if(state == false) return;
    QWebView *webview = (QWebView *)sender();
    for(int n=0; n<webviews.count(); n++)
    {
        if(webview == webviews[n].webview)
        {
            QString hostname = webviews[n].webview->page()->networkAccessManager()->proxy().hostName();
            if(hostname.isNull()) hostname = "no proxy";
            int port = webviews[n].webview->page()->networkAccessManager()->proxy().port();
            emit debug(QString(tr("finished %1 time elapsed %2 proxy %3:%4")
                               .arg(n)
                               .arg(webviews[n].time.elapsed())
                               .arg(hostname)
                               .arg(port)));
            webviews[n].is_loaded = true;
            webviews[n].progressBar->reset();
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    if(global_stop_flag) return;

    QString current_url = URL_MAIN_PAGE;
    QUrl beginUrl = QUrl(current_url);

    int n=0;
    QRandomGenerator generator;
    int max_n = 10 + (generator.generate() % 10);
    while(n < max_n)
    {
        if(global_stop_flag) return;
        webviews[0].time.start();
        load_url(0, current_url, true);

        QUrl baseUrl = webviews[0].main_frame->baseUrl();

        QWebElementCollection links_collection = webviews[0].main_frame->findAllElements(":link");
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
            int index = generator.generate() % sl.count();
            current_url = sl.at(index);
        }
        else
        {
            current_url = beginUrl.toString();
        }
        int waiting_time = 1 + (generator.generate() % 10);
        emit info(QString(tr("waiting %1 sec")).arg(waiting_time));
        Waiting::sec(waiting_time);
        n++;
        emit info(QString(tr("left %1")).arg(max_n - n));
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
    if(save_action)
    {
        save_action->setText(QObject::tr("save"));
    }
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_url(int index,
                       const QString &address,
                       bool show_address,
                       bool waiting_load)
{
    if(global_stop_flag)
    {
        return;
    }

    if(webviews[index].webview == 0)
    {
        emit error("webview not init");
        return;
    }

    if(show_address)
    {
        emit info(QString(tr("Загружаем %1")).arg(address));
    }
    webviews[index].is_loaded = false;
    webviews[index].webview->load(address);
    webviews[index].webview->show();

    if(waiting_load)
    {
        Waiting::is_loaded(&webviews[index].is_loaded);
    }
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
    int button = QMessageBox::question(this,
                                       tr("save"),
                                       tr("Save proxy list?"),
                                       QMessageBox::Yes,
                                       QMessageBox::No);
    if(button != QMessageBox::Yes)
    {
        return;
    }

    if(proxy_list->count() == 0) return;
    emit debug("save_setting");

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
}
//--------------------------------------------------------------------------------
QString MainBox::get_random_url(int index)
{
    QString current_url = URL_MAIN_PAGE;
    QUrl beginUrl = QUrl(current_url);
    QUrl baseUrl = webviews[index].main_frame->baseUrl();
    QWebElementCollection links_collection = webviews[index].main_frame->findAllElements(":link");
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
        QRandomGenerator generator;
        int index = generator.generate() % sl.count();
        current_url = sl.at(index);
    }
    else
    {
        current_url = beginUrl.toString();
    }
    return current_url;
}
//--------------------------------------------------------------------------------
bool MainBox::is_not_working(void)
{
    for(int n=0; n<MAX_TAB; n++)
    {
        if(webviews[n].is_block == false) return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
