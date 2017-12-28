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
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>

#include <QWebElementCollection>
#include <QStringListModel>
#include <QInputDialog>
#include <QProgressBar>
#include <QWebSettings>
#include <QTextBrowser>
#include <QMessageBox>
#include <QWebHistory>
#include <QWebElement>
#include <QHeaderView>
#include <QToolButton>
#include <QPushButton>
#include <QByteArray>
#include <QTableView>
#include <QLineEdit>
#include <QWebFrame>
#include <QTextEdit>
#include <QToolBar>
#include <QWidget>
#include <QDebug>
#include <QTest>
#include <QtSql>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "persistentcookiejar.hpp"
#include "helpbrowser.hpp"
#include "javascript.hpp"
#include "mainwindow.hpp"
#include "tableview.hpp"
#include "findlinks.hpp"
#include "database.hpp"
#include "qxmlputget.h"
#include "mainbox.hpp"
#include "waiting.hpp"
#include "webview.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#include "listwidget_site.hpp"
#include "listwidget_link.hpp"
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include <stdarg.h>
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    main_frame(0),
    leAddress(0),
    table_view(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(table_view)
    {
        table_view->close();
        table_view->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    ui->setupUi(this);

    createUrlBar();

    browser = new WebView(this);
    ui->web_layout->addWidget(browser);

    ListWidget_Site *sites = new ListWidget_Site(browser, this);

    sites->add_item("http://www.opennet.ru");
    sites->add_item("http://www.opennet.ru/#mini");
    sites->add_item("http://www.securitylab.ru");
    sites->add_item("http://www.youtube.com");

    ListWidget_Link *links = new ListWidget_Link(browser, this);

    connect(sites, SIGNAL(ready(QModelIndex)), links, SLOT(get_links(QModelIndex)));

    LogBox *log = new LogBox("Creator", this);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(sites);
    hbox->addWidget(links);
    hbox->addWidget(log);

    hbox->setStretchFactor(sites, 1);
    hbox->setStretchFactor(links, 3);
    hbox->setStretchFactor(log, 1);

    ui->web_layout->addLayout(hbox);

    main_frame = browser->page()->mainFrame();

    //QFont font("Mono");
    //QFont font("Times", 10, QFont::Bold);
    //ui->webView->setFont(font);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    connect(browser, SIGNAL(loadStarted()), this, SLOT(started()));
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(finished(bool)));
    connect(browser, SIGNAL(loadProgress(int)), progressBar, SLOT(setValue(int)));
    if(mw)
    {
        connect(browser, SIGNAL(titleChanged(QString)), mw, SLOT(setWindowTitle(QString)));
        connect(browser, SIGNAL(statusBarMessage(QString)), mw, SLOT(set_status1_text(QString)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::itemChanged(QListWidgetItem *item)
{
    emit debug(QString("%1 %2")
               .arg(item->text())
               .arg(item->data(Qt::CheckStateRole).toBool()));
}
//--------------------------------------------------------------------------------
void MainBox::createUrlBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *urlbar = new QToolBar(tr("urlbar"));
    urlbar->setObjectName("urlbar");

    mw->addToolBar(Qt::TopToolBarArea, urlbar);

    leAddress = new QLineEdit;
    leAddress->setText("http://www.home.net/svow");
    //leAddress->setMaximumWidth(400);

    urlbar->addWidget(leAddress);

    QToolButton *btn_open_url = add_button(urlbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                           "open url",
                                           "open url");

    connect(btn_open_url, SIGNAL(clicked()), this, SLOT(open_url()));
    connect(leAddress, SIGNAL(returnPressed()), this, SLOT(open_url()));

    progressBar = new QProgressBar;
    progressBar->setMaximumWidth(100);
    urlbar->addWidget(progressBar);

    urlbar->addSeparator();

    QToolButton *btn_run = add_button(urlbar,
                                      new QToolButton(this),
                                      qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                                      "run",
                                      "run");
    QToolButton *btn_test = add_button(urlbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       "test",
                                       "test");
    QToolButton *btn_show_pages = add_button(urlbar,
                                             new QToolButton(this),
                                             QIcon(),
                                             "show pages",
                                             "show pages");
    QToolButton *btn_show_content_data = add_button(urlbar,
                                                    new QToolButton(this),
                                                    QIcon(QLatin1String(":/mainwindow/database.png")),
                                                    "show content_data",
                                                    "show content_data");
    QToolButton *btn_show_cookies = add_button(urlbar,
                                               new QToolButton(this),
                                               QIcon(QLatin1String(":/mainwindow/cookie.png")),
                                               "show cookies",
                                               "show cookies");
    QToolButton *btn_test_login = add_button(urlbar,
                                             new QToolButton(this),
                                             QIcon(QLatin1String(":/mainwindow/preferences-system-login.png")),
                                             "test login",
                                             "test login");

    QToolButton *btn_find_and_append = add_button(urlbar,
                                                  new QToolButton(this),
                                                  QIcon(),
                                                  "find_and_append",
                                                  "find_and_append");

    QToolButton *btn_add_news = add_button(urlbar,
                                           new QToolButton(this),
                                           QIcon(QLatin1String(":/mainwindow/ucoz_favicon.ico")),
                                           "add news",
                                           "add news");

    QToolButton *btn_del_news = add_button(urlbar,
                                           new QToolButton(this),
                                           QIcon(QLatin1String(":/mainwindow/ucoz_favicon.ico")),
                                           "del news",
                                           "del news");

    urlbar->addSeparator();

    QToolButton *btn_get_rank = add_button(urlbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation),
                                           "get rank site",
                                           "get rank site");

    connect(btn_run, SIGNAL(clicked()), this, SLOT(run()));
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_show_pages, SIGNAL(clicked()), this, SLOT(show_pages()));
    connect(btn_show_content_data, SIGNAL(clicked()), this, SLOT(show_content_data()));
    connect(btn_show_cookies, SIGNAL(clicked()), this, SLOT(show_cookies()));
    connect(btn_test_login, SIGNAL(clicked()), this, SLOT(test_login()));
    connect(btn_find_and_append, SIGNAL(clicked()), this, SLOT(find_and_append()));
    connect(btn_get_rank, SIGNAL(clicked()), this, SLOT(get_rank()));
    connect(btn_add_news, SIGNAL(clicked()), this, SLOT(add_news()));
    connect(btn_del_news, SIGNAL(clicked()), this, SLOT(del_news()));
}
//--------------------------------------------------------------------------------
void MainBox::open_url(void)
{
    if(leAddress->text().isEmpty())
    {
        emit error(tr("Нет адреса"));
        return;
    }
    browser->load(leAddress->text());
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    is_loaded = false;
    //emit debug("started");
}
//--------------------------------------------------------------------------------
void MainBox::finished(bool state)
{
    if(state == false) return;
    //emit info(QString("bytes received = %1").arg(ui->webView->page()->bytesReceived()));
    progressBar->reset();
    //emit debug("finished");
    is_loaded = true;

    //attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::load_url(const QString &address,
                       bool show_address)
{
    if(show_address)
        emit info(QString(tr("Загружаем %1")).arg(address));
    is_loaded = false;
    browser->load(address);
    Waiting::is_loaded(&is_loaded);
}
//--------------------------------------------------------------------------------
bool MainBox::create_content_data(void)
{
    bool ok;
    QSqlQuery sql;

    emit info(tr("Создаем таблицу content_data"));
    sql.prepare("create table content_data ("
                "id integer primary key, "
                "published boolean, "
                "title varchar(200), "
                "content memo)");
    ok = sql.exec();
    if(!ok)
    {
        emit error(QString(tr("query [%1] not exec!")).arg(sql.lastQuery()));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::show_cookies(void)
{
    browser->get_cookies()->debug();
}
//--------------------------------------------------------------------------------
void MainBox::show_pages(void)
{
    if(pages.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }

    QTabWidget *tab = new QTabWidget;
    tab->setWindowTitle("show pages");
    tab->setWindowIcon(QIcon(ICON_PROGRAMM));
    tab->setMinimumSize(1024, 600);

    emit info(tr("Отобразим полученные данные"));
    for(int n=0; n<pages.count(); n++)
    {
        QWidget *frame = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout;
        QLineEdit *le_title = new QLineEdit;
        QTextEdit *te_content = new QTextEdit;

        le_title->setText(pages[n].title);
        te_content->setText(pages[n].content);

        vbox->addWidget(le_title);
        vbox->addWidget(te_content);
        frame->setLayout(vbox);

        tab->addTab(frame, pages[n].url);
    }
    tab->show();
}
//--------------------------------------------------------------------------------
void MainBox::show_content_data(void)
{
    table_view = new TableView;
    connect(table_view, SIGNAL(info(QString)),  this, SIGNAL(info(QString)));
    connect(table_view, SIGNAL(debug(QString)), this, SIGNAL(debug(QString)));
    connect(table_view, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    table_view->init("SELECT * FROM content_data");
    table_view->show();
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    int button = QMessageBox::question(this, tr("Коллекция ссылок"), tr("Собираем коллекцию ссылок?"), QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
        return;

    while(true)
    {
        bool r = find_exit("http://www.home.net/svow",
                           "Выход");
        if(!r)
            test_login();
        else
            break;
        Waiting::sec(1);
    }
    load_url("http://www.home.net/svow");

    Database *db = new Database("QSQLITE", "customdb.db", this);

    if(db->open() == false)
        return;
    QStringList tables = db->tables();
    if(tables.isEmpty())
    {
        create_content_data();
    }
    QList<QUrl> urls_collection_opennet;
    QList<QUrl> urls_collection_securitylab;
    QSqlQuery temp_sql;

    emit info(tr("Собираем коллекцию ссылок"));
    pages.clear();
    //-----------------------------------------------
    urls_collection_opennet.clear();
    load_url("http://www.opennet.ru");
    QUrl baseUrl = main_frame->baseUrl();
    QWebElementCollection news_collection = main_frame->findAllElements("td[class=tnews]");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_collection.count()));
    foreach (QWebElement element, news_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl absoluteUrl = baseUrl.resolved(relativeUrl);

            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'")
                          .arg(link.toPlainText().trimmed()));
            bool result = temp_sql.first();
            if(result == false)
            {
                emit debug(link.toPlainText());
                urls_collection_opennet.append(absoluteUrl);
            }
        }
    }

    foreach (QUrl url, urls_collection_opennet)
    {
        load_url(url.toString());

        QString selectorQuery = "span[id=r_title]";
        QWebElement title = main_frame->findFirstElement(selectorQuery);
        if(title.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        selectorQuery = "td[class=chtext]";
        QWebElement content = main_frame->findFirstElement(selectorQuery);
        if(content.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        page pg;
        pg.url     = main_frame->url().toString();
        pg.title   = title.toPlainText().trimmed();
        pg.content = QString("%1<br><a href='%2'>Источник</a>")
                .arg(content.toInnerXml())
                .arg(pg.url);
        pages.append(pg);
    }
    if(pages.count() == 0)
    {
        emit info(tr("Новых данных нет"));
    }
    else
    {
        emit info(QString(tr("Собрали коллекцию из %1 ссылок")).arg(pages.count()));
    }
    //-----------------------------------------------
    urls_collection_securitylab.clear();
    load_url("http://www.securitylab.ru");
    baseUrl = main_frame->baseUrl();
    QWebElement main_content = main_frame->findFirstElement("div[class=main_content]");
    if(main_content.isNull())
    {
        emit error("main_content not found!");
        return;
    }
    QWebElementCollection news_h1_collection = main_content.findAll("h1");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_h1_collection.count()));
    foreach (QWebElement element, news_h1_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl h1_absoluteUrl = baseUrl.resolved(relativeUrl);

            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'").arg(link.toPlainText().trimmed()));
            bool result = temp_sql.first();
            if(result == false)
            {
                emit debug(link.toPlainText());
                urls_collection_securitylab.append(h1_absoluteUrl);
            }
        }
    }
    QWebElementCollection news_h2_collection = main_content.findAll("h2");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_h2_collection.count()));
    foreach (QWebElement element, news_h2_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl h2_absoluteUrl = baseUrl.resolved(relativeUrl);

            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'").arg(link.toPlainText().trimmed()));
            bool result = temp_sql.first();
            if(result == false)
            {
                emit debug(link.toPlainText());
                urls_collection_securitylab.append(h2_absoluteUrl);
            }
        }
    }

    foreach (QUrl url, urls_collection_securitylab)
    {
        load_url(url.toString());

        QString selectorQuery = "h1[itemprop=name]";
        QWebElement title = main_frame->findFirstElement(selectorQuery);
        if(title.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        selectorQuery = "div[itemprop=articleBody]";
        QWebElement content = main_frame->findFirstElement(selectorQuery);
        if(content.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        page pg;
        pg.url     = main_frame->url().toString();
        pg.title   = title.toPlainText().trimmed();
        pg.content = QString("%1<br><a href='%2'>Источник</a>")
                .arg(content.toInnerXml())
                .arg(pg.url);
        pages.append(pg);
    }
    if(pages.count() == 0)
    {
        emit info(tr("Новых данных нет"));
    }
    else
    {
        emit info(QString(tr("Собрали коллекцию из %1 ссылок")).arg(pages.count()));
    }
    //-----------------------------------------------
    if(pages.count() == 0)
    {
        emit info(tr("Новых данных нет"));
        return;
    }
    if(!db->isOpen())
        db->open();

    QSqlQuery sql;
    sql.prepare("INSERT INTO content_data (id, published, title, content) "
                "VALUES (:id, :published, :title, :content)");
    for(int i=0; i<pages.count(); i++)
    {
        emit info(tr("Добавляем новые данные в БД"));
        sql.bindValue(":published", false);
        sql.bindValue(":title",   pages[i].title);
        sql.bindValue(":content", pages[i].content);
        if(sql.exec() == false)
        {
            emit debug(sql.executedQuery());
            emit error("Не удалось добавить запись!");
        }
    }
    emit info(tr("Закончили сбор"));
    //-----------------------------------------------
    QSqlTableModel model;
    model.setTable("content_data");
    model.setFilter("published = 'false'");
    bool result = model.select();
    if(result == false)
    {
        emit error(tr("Ошибка model.select()"));
        return;
    }
    //-----------------------------------------------
    emit info(tr("Сохраняем данные"));
    emit info(QString(tr("rowCount %1")).arg(model.rowCount()));
    while(model.rowCount()>0)
    {
        load_url("http://www.home.net/svow/admin/page_new");

        emit info(QString(tr("Осталось обработать %1 записей")).arg(model.rowCount()));
        emit info(QString(tr("Создаем страницу [%1]")).arg(model.record(0).value("title").toString()));
        find_radioelement_and_click("div[class='block_page page_post_type']", "2");
        find_checkboxelement_and_click("div[class='block_page page_all_cat']", "1");
        find_radioelement_and_click("div[class=page_status]", "publish");

        find_and_evaluateJavaScript("input[name=f_comment_allow]", "this.checked=false");
        find_and_evaluateJavaScript("input[name=f_feed_allow]", "this.checked=false");

        JavaScript::setByName(main_frame, "f_header", model.record(0).value("title").toString());

        simple_setByName("f_content", model.record(0).value("content").toString());
        //js_setByName("f_content", pages[n].content);

        JavaScript::clickByName(main_frame, "f_submit");

        is_loaded = false;
        Waiting::is_loaded(&is_loaded);

        QSqlRecord record = model.record(0);
        record.setValue("published", true);
        model.setRecord(0, record);
        model.submitAll();
    }

    load_url("http://www.home.net/svow");

    db->view("SELECT * FROM content_data");
    db->close();
}
//--------------------------------------------------------------------------------
void MainBox::find_and_evaluateJavaScript(const QString &selector_element,
                                          const QString &java_script)
{
    QWebElement element = main_frame->findFirstElement(selector_element);
    if(element.isNull())
    {
        emit error("element not found");
        return;
    }
    element.evaluateJavaScript(java_script);
}
//--------------------------------------------------------------------------------
void MainBox::find_checkboxelement_and_click(const QString &selector_block,
                                             const QString &value)
{
    QWebElement radio_element = main_frame->findFirstElement(selector_block);
    if(radio_element.isNull())
    {
        emit error("block not found");
        return;
    }
    QWebElementCollection radio_collection = radio_element.findAll("input[type=checkbox]");
    foreach (QWebElement element, radio_collection)
    {
        if(element.attribute("value") == value)
        {
            element.evaluateJavaScript("this.checked=true");
            return;
        }
    }
    emit error("value not found");
}
//--------------------------------------------------------------------------------
void MainBox::find_radioelement_and_click(const QString &selector_block,
                                          const QString &value)
{
    QWebElement radio_element = main_frame->findFirstElement(selector_block);
    if(radio_element.isNull())
    {
        emit error("block not found");
        return;
    }
    QWebElementCollection radio_collection = radio_element.findAll("input[type=radio]");
    foreach (QWebElement element, radio_collection)
    {
        if(element.attribute("value") == value)
        {
            element.evaluateJavaScript("this.checked=true");
            return;
        }
    }
    emit error("value not found");
}
//--------------------------------------------------------------------------------
bool MainBox::simple_setByName(const QString &name,
                               const QVariant &value,
                               bool isInnerXml)
{
    QWebElement content = main_frame->findFirstElement(QString("textarea[name=%1]").arg(name));
    if(content.isNull())
    {
        emit error("content not found");
        return false;
    }
    if(isInnerXml)
        content.setInnerXml(value.toString());
    else
        content.setOuterXml(value.toString());
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::test_login(void)
{
    QString address("http://www.home.net/svow/login");

    load_url(address);

    is_loaded = false;
    if(JavaScript::setByName(main_frame, "flogin_user", "admin") == false) return;
    if(JavaScript::setByName(main_frame, "flogin_password", "q12345") == false) return;
    JavaScript::clickByName(main_frame, "flogin_submit");

    Waiting::is_loaded(&is_loaded);
}
//--------------------------------------------------------------------------------
void MainBox::test_securitylab(void)
{
    emit info(tr("Собираем коллекцию ссылок"));

    Database *db = new Database("QSQLITE", "customdb.db", this);
    db->open();

    QStringList tables = db->tables();
    if(tables.isEmpty())
    {
        create_content_data();
    }
    QList<QUrl> urls_collection;
    QSqlQuery temp_sql;

    pages.clear();
    //-----------------------------------------------
    urls_collection.clear();
    load_url("http://www.securitylab.ru");
    QUrl baseUrl = main_frame->baseUrl();
    QWebElement main_content = main_frame->findFirstElement("div[class=main_content]");
    if(main_content.isNull())
    {
        emit error("main_content not found!");
        return;
    }
    QWebElementCollection news_h1_collection = main_content.findAll("h1");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_h1_collection.count()));
    foreach (QWebElement element, news_h1_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl h1_absoluteUrl = baseUrl.resolved(relativeUrl);

            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'").arg(link.toPlainText().trimmed()));
            bool result = temp_sql.first();
            if(result == false)
            {
                emit debug(link.toPlainText());
                urls_collection.append(h1_absoluteUrl);
            }
        }
    }
    QWebElementCollection news_h2_collection = main_content.findAll("h2");
    emit info(QString(tr("Нашли %1 ссылок")).arg(news_h2_collection.count()));
    foreach (QWebElement element, news_h2_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl h2_absoluteUrl = baseUrl.resolved(relativeUrl);

            temp_sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'").arg(link.toPlainText().trimmed()));
            bool result = temp_sql.first();
            if(result == false)
            {
                emit debug(link.toPlainText());
                urls_collection.append(h2_absoluteUrl);
            }
        }
    }

    foreach (QUrl url, urls_collection)
    {
        load_url(url.toString());

        QString selectorQuery = "h1[itemprop=name]";
        QWebElement title = main_frame->findFirstElement(selectorQuery);
        if(title.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        selectorQuery = "div[itemprop=articleBody]";
        QWebElement content = main_frame->findFirstElement(selectorQuery);
        if(content.isNull())
        {
            emit error(QString(tr("селектор %1 не найден")).arg(selectorQuery));
            return;
        }

        page pg;
        pg.url     = main_frame->url().toString();
        pg.title   = title.toPlainText().trimmed();
        pg.content = QString("%1<br><a href=\"%2\">Источник</a>")
                .arg(content.toInnerXml())
                .arg(pg.url);
        pages.append(pg);
    }
    if(pages.count() == 0)
    {
        emit info(tr("Новых данных нет"));
        return;
    }
    emit info(QString(tr("Собрали коллекцию из %1 ссылок")).arg(pages.count()));
    db->close();
}
//--------------------------------------------------------------------------------
void MainBox::find_and_append(void)
{
    QTime time;
    time.start();

    Database *db = new Database("QSQLITE", "customdb.db", this);
    db->open();

    QSqlTableModel model;
    model.setTable("content_data");
    bool result = model.select();
    if(result == false)
    {
        emit error(tr("Ошибка model.select()"));
        return;
    }
    //-----------------------------------------------
    emit info(QString(tr("В базе %1 записей")).arg(model.rowCount()));
    for(int n=0; n<model.rowCount(); n++)
    {
        load_url("http://www.home.net/svow/", false);
        is_loaded=false;
        QString search_str=model.record(n).value("title").toString().trimmed().simplified();
        //emit debug(search_str);
        //js_setByName("s", search_str.replace("/", "&frasl;"));
        //js_setByName("s", search_str.replace("%", "&#37"));
        JavaScript::setByName(main_frame, "s", search_str);

        JavaScript::clickByName(main_frame, "Submit");

        Waiting::is_loaded(&is_loaded);

        QWebElementCollection main_collection = main_frame->findAllElements("div[class=main]");
        bool found = false;
        foreach (QWebElement element, main_collection)
        {
            QWebElement link = element.findFirst(":link");
            if(!link.isNull())
            {
                if(link.toPlainText() == search_str)
                {
                    found = true;
                }
            }
        }
        if(found == false)
        {
            emit error(QString(tr("[%1] нет на сайте")).arg(search_str));
            return;
#if 0
            load_url("http://www.home.net/svow/admin/page_new");

            emit info(QString(tr("Создаем страницу [%1]")).arg(model.record(n).value("title").toString()));
            find_radioelement_and_click("div[class='block_page page_post_type']", "2");
            find_checkboxelement_and_click("div[class='block_page page_all_cat']", "1");
            find_radioelement_and_click("div[class=page_status]", "publish");

            find_and_evaluateJavaScript("input[name=f_comment_allow]", "this.checked=false");
            find_and_evaluateJavaScript("input[name=f_feed_allow]", "this.checked=false");

            js_setByName("f_header", model.record(n).value("title").toString().trimmed());

            simple_setByName("f_content", model.record(n).value("content").toString());
            //js_setByName("f_content", pages[n].content);

            js_clickByName("f_submit");

            is_loaded = false;

            Waiting::is_loaded(&is_loaded);
#endif
            Waiting::sec(5);
        }
    }
    emit debug(QString("time.elapsed() = %1 second").arg(time.elapsed() / 1000.f));
    emit info("End!");
}
//--------------------------------------------------------------------------------
void MainBox::wait_element(const QString &selector_element,
                           int timeout_msec)
{
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        QWebElement form = main_frame->findFirstElement(selector_element);
        if(form.isNull() == false)
            break;
        if(time.elapsed() > timeout_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::add_news(void)
{
    QString url = "http://linux.ucoz.com";

    int button = QMessageBox::question(this,
                                       tr("Добавление данных"),
                                       QString(tr("Добавить данные на %1?").arg(url)),
                                       QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
        return;

    bool r = find_exit(url,
                       "Выход");
    if(!r)
    {
        emit error("Exit is false");
        return;
    }

    int timeout = 25;

    Database *db = new Database("QSQLITE", "customdb.db", this);
    db->open();

    QSqlTableModel model;
    model.setTable("content_data");
    //model.setFilter("published = 'false'");
    bool result = model.select();
    if(result == false)
    {
        emit error(tr("Ошибка model.select()"));
        return;
    }
    //-----------------------------------------------
    int full_time = timeout * model.rowCount();

    QTime time;
    QTime ntime = time.addSecs(full_time);

    QString waiting_time = QString(tr("На добавление %1 постов ориентировочно потребуется времени %2:%3:%4. Продолжить?"))
            .arg(model.rowCount())
            .arg(ntime.hour())
            .arg(ntime.minute())
            .arg(ntime.second());

    int btn = QMessageBox::question(this, tr("Добавление постов"), waiting_time, QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes)
        return;

    //-----------------------------------------------
    emit info(QString(tr("rowCount %1")).arg(model.rowCount()));
    for(int n=0; n<model.rowCount(); n++)
    {
        emit info(QString(tr("Обрабатываем %1 пост из %2 постов"))
                  .arg(n)
                  .arg(model.rowCount()));

        QString title = model.record(n).value("title").toString();
        QString content = model.record(n).value("content").toString();
        if(title.isEmpty())
            continue;
        if(content.isEmpty())
            continue;

        load_url(QString("%1/news/0-0-0-0-1").arg(url));
        JavaScript::setByName(main_frame, "title", title);
        content = html_to_bbcode(content);
        simple_setByName("message", content);
        JavaScript::clickByName(main_frame, "sbm");

        wait_element("div[class=myWinSuccess]");
        Waiting::sec(timeout + (rand() % 5));
    }
    load_url(url);

    db->close();
    QMessageBox::information(this, tr("Information"), tr("The end!"));
}
//--------------------------------------------------------------------------------
void MainBox::del_news(void)
{
    QString url = "http://linux.ucoz.com";

    int button = QMessageBox::question(this,
                                       tr("Удаление данных"),
                                       QString(tr("Удалить данные из %1?").arg(url)),
                                       QMessageBox::Yes, QMessageBox::No);
    if(button != QMessageBox::Yes)
        return;

    bool r = find_exit(url,
                       "Выход");
    if(!r)
    {
        emit error("Exit is false");
        return;
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(always_yes()));
    timer->start(1500);

    while(true)
    {
        load_url(url);
        QWebElement submit = main_frame->findFirstElement("img[title=Удалить]");
        if(submit.isNull())
        {
            timer->stop();
            delete timer;
            QMessageBox::critical(this, tr("Error"), tr("submit not found"));
            return;
        }

        JavaScript::click_element(submit);
        Waiting::sec(5);
    }
    QMessageBox::information(this, tr("Information"), tr("The end!"));
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
    QNetworkProxy::setApplicationProxy(proxy);

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::always_yes(void)
{
    QList<QMessageBox *> all_dlg = findChildren<QMessageBox *>();
    foreach(QMessageBox *dlg, all_dlg)
    {
        emit debug(dlg->windowTitle());

        QList<QPushButton *> all_btn = dlg->findChildren<QPushButton *>();
        foreach(QPushButton *btn, all_btn)
        {
            if(btn->text() == "&Да")
            {
                QTest::keyClick(btn, Qt::Key_Space);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(QUrl url)
{
    browser->load(url);
}
//--------------------------------------------------------------------------------
void MainBox::replyFinished(QNetworkReply *reply)
{
    emit info(reply->header(QNetworkRequest::ContentTypeHeader).toString());
    emit info(reply->header(QNetworkRequest::ContentLengthHeader).toString());
    emit info(reply->header(QNetworkRequest::LocationHeader).toString());
    emit info(reply->header(QNetworkRequest::LastModifiedHeader).toString());
    emit info(reply->header(QNetworkRequest::CookieHeader).toString());
    emit info(reply->header(QNetworkRequest::SetCookieHeader).toString());
    emit debug("replyFinished");
    QXmlStreamReader xml(reply->readAll());
    while (!xml.atEnd())
    {
        emit debug(xml.name().toString());
        xml.readNext();
    }
    is_loaded = true;
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    QString test_string = "<table border='0' cellspacing='0' cellpadding='7'>"
            "<tbody><tr>"
            "<td class='job_content-field_label job_content-field_title'>Вакансия:</td>"
            "<td class='job_content-field_value'>Администратор салона красоты</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Организация:</td>"
            "<td class='job_content-field_value'>Английский салон красоты 'TONI&amp;GUY'</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Адрес работы:</td>"
            "<td class='job_content-field_value'>"
            "<div class='address'>г. Краснодар</div>"
            "</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label job_content-field_title'>Функции:</td>"
            "<td class='job_content-field_value job_content-field_html'><ul>"
            "<li>Встреча гостей салона;</li>"
            "<li>приём заявок на обслуживание в салоне;</li>"
            "<li>ведение денежной отчётности;</li>"
            "<li>работа с коллективом.</li>"
            "</ul></td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Непосредственный руководитель:</td>"
            "<td class='job_content-field_value'>Директор</td>"
            "</tr>"
            "<tr valign='top'>"
            "<td class='job_content-field_label job_content-field_title'>Требования</td>"
            "<td>&nbsp;</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Пол:</td>"
            "<td class='job_content-field_value'>Женщина</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Возраст:</td>"
            "<td class='job_content-field_value'>От 25 до 35 лет</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Образование:</td>"
            "<td class='job_content-field_value'>Высшее</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Знание ин. языка:</td>"
            "<td class='job_content-field_value'>Желательно знание английского языка (базовый уровень).</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Владение ПК:</td>"
            "<td class='job_content-field_value'>Уверенный пользователь. Знание 1С.</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Место жительства:</td>"
            "<td class='job_content-field_value'>г. Краснодар (наличие местной прописки)</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Подчиненные:</td>"
            "<td class='job_content-field_value'>От 25 человек</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Требуемый опыт:</td>"
            "<td class='job_content-field_value job_content-field_html'><ul>"
            "<li>Опыт работы в салонах красоты, СПА-центрах;</li>"
            "<li>опыт продаж услуг.</li>"
            "</ul></td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Дополнительные требования:</td>"
            "<td class='job_content-field_value job_content-field_html'><ul>"
            "<li>Умение владеть ситуацией;</li>"
            "<li>коммуникабельность;</li>"
            "<li>вежливость;</li>"
            "<li>умение конструктивно воспринимать критику;</li>"
            "<li>хорошо поставленная речь.</li>"
            "</ul></td>"
            "</tr>"
            "<tr valign='top'>"
            "<td class='job_content-field_label job_content-field_title'>Условия найма</td>"
            "<td>&nbsp;</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Зарплата:</td>"
            "<td class='job_content-field_value'>От 22 000 руб.</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Форма занятости:</td>"
            "<td class='job_content-field_value'>Полная</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>График работы:</td>"
            "<td class='job_content-field_value'>Сменный график</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Расписание рабочего дня:</td>"
            "<td class='job_content-field_value'>С 09.00 до 21.00, 2 дня рабочих, 2 дня выходных</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Командировки:</td>"
            "<td class='job_content-field_value'>Возможны</td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Спец. условия найма, льготы и выплаты:</td>"
            "<td class='job_content-field_value job_content-field_html'><p>Трудоустройство согласно ТК РФ.</p></td>"
            "</tr>"
            "<tr>"
            "<td class='job_content-field_label'>Особые пожелания и дополнения:</td>"
            "<td class='job_content-field_value job_content-field_html'><p><strong>Обязательно присылать резюме с фото (в настоящем времени).</strong></p></td>"
            "</tr>"
            "</tbody></table>";
}
//--------------------------------------------------------------------------------
void MainBox::generate_random_name(void)
{
    QFile name_rus_men_data(":/name_rus_men.txt");
    QFile name_rus_women_data(":/name_rus_women.txt");
    QFile family_rus_data(":/family_rus.txt");

    QStringList name_rus_men_list;
    QStringList name_rus_women_list;
    QStringList family_list;
    //------
    if(name_rus_men_data.open(QFile::ReadOnly | QFile::Truncate))
    {
        QTextStream in(&name_rus_men_data);
        while(1)
        {
            QString name = in.readLine();
            if(name.isEmpty()) break;
            name_rus_men_list << name;
        }
    }
    emit info(QString(tr("Нашли %1 мужских имен")).arg(name_rus_men_list.count()));
    name_rus_men_list.sort();
    //------
    if(name_rus_women_data.open(QFile::ReadOnly | QFile::Truncate))
    {
        QTextStream in(&name_rus_women_data);
        while(1)
        {
            QString name = in.readLine();
            if(name.isEmpty()) break;
            name_rus_women_list << name;
        }
    }
    emit info(QString(tr("Нашли %1 женских имен")).arg(name_rus_women_list.count()));
    name_rus_women_list.sort();
    //------
    if(family_rus_data.open(QFile::ReadOnly | QFile::Truncate))
    {
        QTextStream in(&family_rus_data);
        while(1)
        {
            QString name = in.readLine();
            if(name.isEmpty()) break;
            family_list << name;
        }
    }
    emit info(QString(tr("Нашли %1 фамилий")).arg(family_list.count()));
    family_list.sort();
    //------
    QStringList final_data;
    for(int n=0; n<family_list.count(); n++)
    {
        QString last_simvol = family_list.at(n).right(1);
        QString temp = "";
        if((last_simvol == "а") || (last_simvol == "е") || (last_simvol == "я"))
        {
            temp = QString("%1 %2")
                    .arg(family_list.at(n))
                    .arg(name_rus_women_list.at(qrand() % name_rus_women_list.count()));
        }

        if(temp.isEmpty())
        {
            temp = QString("%1 %2")
                    .arg(family_list.at(n))
                    .arg(name_rus_men_list.at(qrand() % name_rus_men_list.count()));
        }
        final_data << temp;
    }

    QStringListModel *model = new QStringListModel;
    model->setStringList(final_data);

    QListView *view = new QListView;
    view->setWindowIcon(QIcon(ICON_PROGRAMM));
    view->setWindowTitle(tr("view"));
    view->setModel(model);
    view->show();
}
//--------------------------------------------------------------------------------
void MainBox::func(int paramCount, ...)
{
    va_list vlist;
    va_start(vlist, paramCount);

    for(int i = 0; i < paramCount; i++)
    {
        emit debug(QString("%1").arg(va_arg(vlist, int)));
    }
}
//--------------------------------------------------------------------------------
QString MainBox::get_link(QWebElement element)
{
    QUrl baseUrl = main_frame->baseUrl();
    QWebElement temp = element.findFirst(":link");
    QString href = temp.attribute("href");
    QUrl relativeUrl(href);
    QUrl absoluteUrl = baseUrl.resolved(relativeUrl);

    return absoluteUrl.toString();
}
//--------------------------------------------------------------------------------
QStringList MainBox::get_links(QWebElementCollection collection)
{
    QStringList sl;
    sl.clear();
    foreach (QWebElement element, collection)
    {
        QString temp = get_link(element);
        if(temp.isEmpty() == false)
            sl << temp;
    }

    return sl;
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    load_url("http://plastikovyeokna-krasnodar.ru/");
    QWebElementCollection element_collection = main_frame->findAllElements("*");
    emit info(QString(tr("found %1")).arg(element_collection.count()));
    QMap <int, QWebElement> map;
    foreach (QWebElement element, element_collection)
    {
        map.insert(element.toPlainText().length(), element);
    }

    int x = 0;
    QMapIterator<int, QWebElement> i(map);
    while (i.hasNext())
    {
        i.next();
        emit info(QString("len %1")
                  .arg(i.key()));
        if(i.key() > x) x=i.key();
    }
    emit info(map.value(x).toPlainText());

    return;

    load_url("http://bash.im/quote/422650");
    QWebElement element = main_frame->findFirstElement("a[class=down]");
    if(!element.isNull())
    {
        emit info("Found!");
        JavaScript::click_element(element);
    }
    else
        emit error("element NOT found!");
    return;

    load_url("http://lenta.ru/");
    QWebElementCollection titles_collection = main_frame->findAllElements("div[class=titles]");
    QWebElementCollection items_collection = main_frame->findAllElements("div[class=item]");
    emit info(QString("titles find %1").arg(titles_collection.count()));
    emit info(QString("items find %1").arg(items_collection.count()));
    QStringList sl;
    foreach (QWebElement title, titles_collection)
    {
        sl << title.toPlainText();
    }
    foreach (QWebElement item, items_collection)
    {
        sl << item.toPlainText();
    }

    QStringListModel *sl_model = new QStringListModel;
    sl_model->setStringList(sl);
    QListView *sl_view = new QListView;
    sl_view->setModel(sl_model);
    sl_view->show();
    return;

    proxies.clear();

    for(int n=1; n<13; n++)
    {
        load_url(QString("http://www.freeproxylists.net/ru/?c=&pt=&pr=HTTP&a[]=0&a[]=1&a[]=2&u=0&page=%1").arg(n));
        QWebElementCollection proxy_collection = main_frame->findAllElements("tr[class=Odd]");
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
                proxies.append(uri);
            }
        }
    }

    get_proxies("http://nntime.com/");
    for(int n=1; n<12; n++)
    {
        get_proxies(QString("http://nntime.com/proxy-list-%1.htm").arg(n, 2, 10, QLatin1Char('0')));
    }

    emit debug(QString("found %1 proxy").arg(proxies.count()));
    int n=0;
    QStringListModel *model = new QStringListModel();
    QStringList list;
    foreach (QString proxy, proxies)
    {
        if(proxy.isEmpty() == false)
        {
            emit debug(QString(tr("Обрабатываем %1 из %2 [%3]"))
                       .arg(n)
                       .arg(proxies.count())
                       .arg(proxy));
            bool ok = append_proxy(proxy);
            if(ok)
            {
                load_url("http://linux.ucoz.com");
                QWebElement title = main_frame->findFirstElement("title");
                if(title.isNull() == false)
                {
                    if(title.toPlainText() == "Персональный сайт - Главная страница")
                    {
                        emit info(QString("proxy %1 is valid!").arg(proxy));
                        list << proxy;
                    }
                    else
                    {
                        emit error(QString("proxy %1 is NOT valid!").arg(proxy));
                    }
                }
            }
        }
        n++;
    }

    QFile proxyfile("proxies.txt");
    if (proxyfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        foreach (QString proxy, list)
        {
            proxyfile.write(QString("%1\n").arg(proxy).toLocal8Bit());
        }
        proxyfile.close();
    }


    model->setStringList(list);
    QTableView *view = new QTableView;
    view->setWindowTitle("valid proxy");
    view->setWindowIcon(QIcon(ICON_PROGRAMM));

    view->setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    header->setResizeMode(QHeaderView::ResizeToContents);
#endif
    view->setHorizontalHeader(header);

    view->show();

    emit info("The end!");
}
//--------------------------------------------------------------------------------
void MainBox::get_proxies(const QString &address)
{
    load_url(address);
    QWebElement select = main_frame->findFirstElement("a[class=select]");
    if(select.isNull())
    {
        emit error("select not found");
        return;
    }
    JavaScript::click_element(select);
    Waiting::sec(1);

    QWebElement textarea = main_frame->findFirstElement("textarea[name=t1]");
    if(textarea.isNull())
    {
        emit error("textarea not found");
        return;
    }

    QVariant result = main_frame->evaluateJavaScript("document.getElementsByName('t1')[0].value");
    QStringList sl = result.toString().split("\n");
    foreach (QString uri, sl)
    {
        if(uri.isEmpty() == false)
        {
            proxies.append(uri);
        }
    }
}
//--------------------------------------------------------------------------------
#if 0
QStringList sl;
sl << "46.19.102.104:80"
   << "186.227.61.38:8080"
   << "120.85.132.234:80"
   << "80.188.127.134:3128"
   << "213.209.107.182:80"
   << "180.248.50.172:8080"
   << "110.138.216.157:3128"
   << "180.247.4.197:3128"
   << "85.43.254.203:80"
   << "77.93.75.241:3128"
   << "183.89.115.33:3128"
   << "183.89.44.86:3128"
   << "183.89.44.164:3128"
   << "202.183.252.151:8080"
   << "74.208.125.160:3128"
   << "205.213.195.180:8080"
   << "205.213.195.80:8080"
   << "207.127.19.127:80"
   << "190.38.154.7:8080"
   << "198.245.70.92:3128"
   << "192.227.139.106:8089"
   << "61.6.254.74:8080"
   << "192.95.35.201:7808"
   << "221.10.40.232:83"
   << "123.157.208.8:9090";

for(int n=0; n<sl.count(); n++)
{
    emit info(QString(tr("Обрабатывается %1 из %2"))
              .arg(n)
              .arg(sl.count()));
    bool ok = append_proxy(sl.at(n));
    if(ok)
        load_url("http://linux.ucoz.com");
}

QMessageBox::information(this, "proxy", "the end!");
#endif

#if 0
bool ok = false;

for(int n=0; n<100; n++)
{
    emit info(QString(tr("Попытка %1")).arg(n));
    load_url("https://www.nic.ru/whois/?domain=atas.ru");
    QWebElement busy = main_frame->findFirstElement("font[class=warning2]");
    if(busy.isNull() == false)
    {
        emit error(busy.toPlainText());
    }
    QWebElement free = main_frame->findFirstElement("div[id=ok-block]");
    if(free.isNull() == false)
    {
        emit debug(free.toPlainText());
        ok=true;
    }
    if(ok == true)
        break;
}
emit info("The end!");
#endif

#if 0
//load_url("http://wordstat.yandex.ru/");
//load_url("http://adstat.rambler.ru/wrds/");
//load_url("https://adwords.google.com/select/KeywordTool");

QFile file(":/aaa/bbb/ccc.ddd");
QFileInfo fileInfo(file.fileName());
QString filename(fileInfo.fileName());
emit debug(filename);
#endif

#if 0
QIcon icon = qApp->style()->standardIcon(QStyle::SP_MediaPlay);
QPixmap pixmap = icon.pixmap(QSize(32, 32));
QString filename = QString("%1.png").arg(icon.name());
pixmap.save(filename);
emit debug(QString("save %1").arg(filename));
#endif

#if 0
func(3, 1, 2, 3);
#endif

#if 0
//load_url("http://www.opennet.ru");
load_url("http://www.opennet.ru/#mini");
QWebElementCollection news_collection = main_frame->findAllElements("td[class=tnews]");
emit info(QString(tr("Нашли %1 ссылок")).arg(news_collection.count()));
foreach (QWebElement link, news_collection)
{
    emit debug(link.toPlainText());
    emit info(get_link(link));
}
#endif

#if 0
load_url("http://www.securitylab.ru");
QWebElementCollection main_content = main_frame->findAllElements("div[class=main_content]");
emit info(QString(tr("Нашли %1 ссылок")).arg(main_content.count()));
foreach (QWebElement el, main_content)
{
    QWebElementCollection h1 = el.findAll("h1");
    foreach (QWebElement link, h1)
    {
        emit debug(link.toPlainText());
        emit info(get_link(link));
    }
    QWebElementCollection h2 = el.findAll("h2");
    foreach (QWebElement link, h2)
    {
        emit debug(link.toPlainText());
        emit info(get_link(link));
    }
}
#endif

#if 0
load_url("http://www.xakep.ru");
QWebElementCollection tags = main_frame->findAllElements("div[class=zagolovok]");
emit info(QString(tr("Нашли %1")).arg(tags.count()));
foreach (QWebElement element, tags)
{
    QWebElementCollection tags = element.findAll(":link");
    emit debug(tags.at(1).toPlainText());
}
#endif

#if 0
generate_random_name();
#endif

#if 0
load_url("http://hotjob.ru/jobsearch/139804");

QWebElement element = main_frame->findFirstElement("div[class=job_content]");
//QWebElement element = main_frame->findFirstElement("tr[valign=top]");
emit debug(QString("element %1")
           .arg(element.isNull() ? "not found" : "found"));
#endif

#if 0
load_url("http://www.home.net/svow/");
QWebElementCollection tags = main_frame->findAllElements("*");
emit info(QString(tr("Нашли %1")).arg(tags.count()));
QStringListModel *model = new QStringListModel;
QStringList list;
foreach (QWebElement tag, tags)
{
    QStringList data;
    data << tag.tagName();
    QStringList attributes = tag.attributeNames();
    foreach (QString attrib, attributes)
    {
        data << QString("%1=%2")
                .arg(attrib)
                .arg(tag.attribute(attrib));
    }
    data << tag.toPlainText();

    QWebElementCollection adv_tag = tag.findAll("*");
    emit info(QString(tr("Нашли %1 субтегов")).arg(adv_tag.count()));

    QString temp;
    temp.clear();
    foreach (QString data_string, data)
    {
        temp.append(data_string);
        temp.append(" ");
    }
    emit debug(temp);

    list << temp;
}
model->setStringList(list);

QListView *view = new QListView;
view->setModel(model);
view->show();
#endif

#if 0
load_url("http://hotjob.ru/jobsearch/156198");
QWebElement content = main_frame->findFirstElement("div[class=job_content]");
if(content.isNull())
{
    emit error("content not found!");
    return;
}
QWebElementCollection tr_collection = content.findAll("tr");
emit info(QString(tr("Нашли %1")).arg(tr_collection.count()));
foreach (QWebElement td, tr_collection)
{
    QWebElement title = td.findFirst("td[class='job_content-field_label job_content-field_title']");
    QWebElement html  = td.findFirst("td[class='job_content-field_value job_content-field_html']");
    QWebElement label = td.findFirst("td[class=job_content-field_label]");
    QWebElement value = td.findFirst("td[class=job_content-field_value]");
    emit info(title.toPlainText());
    emit info(html.toPlainText());
    emit info(label.toPlainText());
    emit info(value.toPlainText());
}
#endif

#if 0
load_url("http://hotjob.ru/jobsearch/156198");
QWebElement content = main_frame->findFirstElement("div[class=job_content]");
QWebElement contact = main_frame->findFirstElement("table[class=contactinfo]");
emit debug(content.toOuterXml());
emit debug(contact.toOuterXml());

QTextBrowser *browser = new QTextBrowser;
browser->setSearchPaths(QStringList() << ".");
browser->append(content.toOuterXml());
browser->append(contact.toOuterXml());
browser->show();
#endif

#if 0
load_url("http://www.hotjob.ru/");
is_loaded = false;
js_setByName("searchstring", "менеджер");
QWebElement submit = main_frame->findFirstElement("input[class='input input_button']");
if(submit.isNull())
{
    emit error("submit not found");
    return;
}
submit.evaluateJavaScript("this.click();");

Waiting::is_loaded(&is_loaded);

QWebElementCollection search_collection = main_frame->findAllElements("div[class=job]");
emit info(QString(tr("Нашли %1 ссылок")).arg(search_collection.count()));
QTabWidget *tab = new QTabWidget;
foreach (QWebElement element, search_collection)
{
    QWebElement h3 = element.findFirst("h3[class=title]");
    if(h3.isNull() == false)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            load_url(href);
            QWebElement content = main_frame->findFirstElement("div[class=job_content]");
            QWebElement contact = main_frame->findFirstElement("table[class=contactinfo]");
            QTextBrowser *browser = new QTextBrowser;
            browser->setSearchPaths(QStringList() << ".");
            browser->append(content.toOuterXml());
            browser->append(contact.toOuterXml());
            tab->addTab(browser, href);

            //                QWebElementCollection table = main_frame->findAllElements("div[class=job_content]");
            //                emit info(QString(tr("Нашли %1 ссылок")).arg(table.count()));
            //                QWebElementCollection title = main_frame->findAllElements("td[class='job_content-field_label job_content-field_title']");
            //                QWebElementCollection data  = main_frame->findAllElements("td[class=job_content-field_value]");
            //                for(int n=0; n<title.count(); n++)
            //                {
            //                    emit debug(QString("%1 %2")
            //                               .arg(title.at(n).toPlainText())
            //                               .arg(data.at(n).toPlainText()));
            //                }
            //                foreach (QWebElement title, data)
            //                {
            //                    emit debug(title.toPlainText());
            //                }

            Waiting::msec(1000);
        }
    }
}
tab->show();
emit info("The end!");
#endif

#if 0
QNetworkRequest request;
//request.setUrl(QUrl("http://whoer.net/"));
request.setUrl(QUrl("http://leader.ru/secure/who.html"));
request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
ui->webView->load(request);
#endif

#if 0
QNetworkAccessManager *manager = new QNetworkAccessManager(this);
connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

is_loaded = false;
manager->get(QNetworkRequest(QUrl("http://www.home.net/svow/")));

Waiting::is_loaded(&is_loaded);
//manager->
#endif

#if 0
QUrl url("http://www.goggle.com");
url.addQueryItem("a", "1");
url.addQueryItem("b", "2");
url.addQueryItem("c", "3");
emit debug(url.toString());
emit debug(url.host());
#endif

#if 0
load_url("https://www.google.ru/");
load_url("https://www.google.ru/");
is_loaded = false;
//js_setByName("q", tr("пластиковые окна"));
js_setByName("q", tr("linux"));
js_clickByName("btnG");

Waiting::is_loaded(&is_loaded);

QWebElement main_content = main_frame->findFirstElement("div[id=search]");
if(main_content.isNull())
{
    emit error("main_content not found!");
    return;
}

QUrl baseUrl = main_frame->baseUrl();
QWebElementCollection search_collection = main_content.findAll("h3");
emit info(QString(tr("Нашли %1 ссылок")).arg(search_collection.count()));
foreach (QWebElement element, search_collection)
{
    QWebElement link = element.findFirst(":link");
    QString href = link.attribute("href");
    if (!href.isEmpty())
    {
        QUrl relativeUrl(href);
        QUrl absoluteUrl = baseUrl.resolved(relativeUrl);
        QString data = absoluteUrl.toString().remove("https://www.google.ru/url?q=");
        QString host = QUrl(data).host();
        //emit debug(host);
        if((host != "www.google.ru") && (host != "maps.google.ru"))
        {
            load_url("https://www.google.ru/", false);
            is_loaded = false;
            js_setByName("q", QString(tr("site:%1")).arg(host));
            js_clickByName("btnG");
            Waiting::is_loaded(&is_loaded);
            QWebElement element = main_frame->findFirstElement("div[id=resultStats]");
            if(element.isNull() == false)
            {
                emit debug(QString("%1 %2")
                           .arg(host)
                           .arg(element.toPlainText()));
            }
        }
    }
}
emit info("The end!");

#endif

#if 0
QString str = "   a b  c    dcccc fgdfg     dfhdfhh  ";
emit debug(QString("[%1]").arg(str));
emit debug(QString("[%1]").arg(str.simplified()));
#endif

#if 0
load_url("http://www.smartmoney.com/quote/ibm/?story=snapshot&symbol=ibm");
QWebElementCollection collection = main_frame->findAllElements("span[smfield=price]");
emit info(QString(tr("Нашли %1 элементов")).arg(collection.count()));
QWebElement element = collection.first();
emit info(element.toPlainText());
#endif

#if 0
//load_url("http://plastikovyeokna-krasnodar.ru/");
load_url("http://tigral.ru/");
QWebElementCollection collection = main_frame->findAllElements("*");
emit info(QString(tr("Нашли %1 элементов [*]")).arg(collection.count()));
QStringListModel *model = new QStringListModel;
QStringList sl;
QListView *view = new QListView;
int cnt = 0;
foreach (QWebElement element, collection)
{
    QString tag = element.tagName();
    sl.append(tag);
    QString text = element.toPlainText().simplified();
    bool ok = text.contains("пластиковые окна", Qt::CaseInsensitive);
    if(ok)
    {
        emit info(tag);
        cnt++;
    }
    //        QStringList sl = element.attributeNames();
    //        foreach (QString name, sl)
    //        {
    //            if(!name.isEmpty())
    //                emit debug(name);
    //        }
}
emit debug(QString(tr("count %1")).arg(cnt));
sl.sort();
model->setStringList(sl);
view->setModel(model);
view->show();
#endif

#if 0
QTime time;
int timeout = 60;

emit debug(QString(tr("start test %1 sec")).arg(timeout));
time.start();
for(int n=0; n<timeout; n++)
{
    //emit debug(QString(tr("Прошло %1 секунд")).arg(n));
    wait_sec(1);
}
emit debug(QString(tr("Прошло %1 милисекунд")).arg(time.elapsed()));
#endif

#if 0
load_url("https://www.google.ru/");
load_url("https://www.google.ru/");
is_loaded = false;
//js_setByName("q", tr("пластиковые окна"));
//js_setByName("q", tr("site:plastikovyeokna-krasnodar.ru"));
//js_setByName("q", tr("site:tigral.ru"));
js_setByName("q", tr("site:okoshkino.ru"));
js_clickByName("btnG");

Waiting::is_loaded(&is_loaded);
QWebElement element = main_frame->findFirstElement("div[id=resultStats]");
if(element.isNull() == false)
emit debug(element.toPlainText());
#endif

#if 0
FindLinks *find = new FindLinks(main_frame, this);
find->exec();
#endif
//--------------------------------------------------------------------------------
void MainBox::get_rank(void)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("rank site"), "site:", QLineEdit::Normal, "platanov.ru", &ok);

    if (ok && !text.isEmpty())
    {
        if(text.toLower().startsWith("http"))
        {
            text.remove("http://");
            text.remove("/");
        }

        load_url("http://digsitevalue.org/");
        is_loaded = false;

        QString site = QString("this.value='%1';").arg(text);
        find_and_evaluateJavaScript("input[id=frm_domain_text]", site);
        find_and_evaluateJavaScript("input[id=frm_search_btn]", "this.click();");

        Waiting::is_loaded(&is_loaded);
        emit info("The End");
    }
}
//--------------------------------------------------------------------------------
QString MainBox::html_to_bbcode(const QString &data)
{
    QString content = data;
    content = content.replace("<",      "[");
    content = content.replace(">",      "]");
    content = content.replace("a href", "url");
    content = content.replace("/a",     "/url");
    content = content.replace("\"",     "");
    content = content.replace("[p]",    "");
    content = content.replace("[/p]",   "");
    content = content.replace("[center]",   "[c]");
    content = content.replace("[/center]",  "[/c]");
    content = content.replace("[left]",     "[l]");
    content = content.replace("[/left]",    "[/l]");
    content = content.replace("[right]",    "[r]");
    content = content.replace("[/right]",   "[/r]");
    content = content.replace("'",      "");
    return content;
}
//--------------------------------------------------------------------------------
//prependOutside
//--------------------------------------------------------------------------------
bool MainBox::find_exit(const QString &address,
                        const QString &exit_string)
{
    load_url(address);
    QWebElementCollection links_collection = main_frame->findAllElements(":link");
    emit debug(QString("count %1").arg(links_collection.count()));
    foreach (QWebElement element, links_collection)
    {
        if(element.toPlainText().toLower() == exit_string.toLower())
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
