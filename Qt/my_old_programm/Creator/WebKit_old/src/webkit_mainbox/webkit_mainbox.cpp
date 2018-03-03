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
#include <QWebElementCollection>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QWebSettings>
#include <QMessageBox>
#include <QWebHistory>
#include <QWebElement>
#include <QHeaderView>
#include <QByteArray>
#include <QTableView>
#include <QLineEdit>
#include <QWebFrame>
#include <QTextEdit>
#include <QWidget>
#include <QtSql>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "ui_webkit_mainbox.h"
//--------------------------------------------------------------------------------
#include "webkit_mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::load_jquery(void)
{
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    ui->setupUi(this);

    frame = ui->webView->page()->mainFrame();

    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, false); //
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false); //
    QWebSettings::globalSettings()->setAttribute(QWebSettings::FrameFlatteningEnabled, true);

    urls_collection.clear();
    urls_index = 0;

    ui->tabWidget->clear();

    connect(ui->webView, SIGNAL(loadStarted()), this, SLOT(started()));
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(finished(bool)));

    connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(test()));

    connect(ui->webView, SIGNAL(loadProgress(int)), ui->progressBar, SLOT(setValue(int)));
}
//--------------------------------------------------------------------------------
void MainBox::js_clickByName(const QString &name)
{
    frame->evaluateJavaScript(QString("document.getElementsByName('%1')[0].click();")
                              .arg(name));
}
//--------------------------------------------------------------------------------
bool MainBox::js_setByName(const QString& name,
                           const QVariant& value)
{
    QVariant result = frame->evaluateJavaScript(QString("document.getElementsByName('%1')[0].value='%2';")
                                                .arg(name)
                                                .arg(value.toString()));
    if(result.isNull())
    {
        emit error(QString("Error in js_setByName name=[%1] value=[%2]")
                   .arg(name)
                   .arg(value.toString()));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::simple_setByName(const QString &name,
                               const QVariant &value)
{
    QWebElement content = frame->findFirstElement(QString("textarea[name=%1]").arg(name));
    if(content.isNull())
    {
        emit error("content not found");
        return false;
    }
    content.setInnerXml(value.toString());
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::find_links(void)
{
    QUrl baseUrl = frame->baseUrl();

    QWebElementCollection links_collection = frame->findAllElements(":link");
    emit debug(QString("count %1").arg(links_collection.count()));
    foreach (QWebElement element, links_collection)
    {
        QString href = element.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl absoluteUrl = baseUrl.resolved(relativeUrl);
            emit debug(QString("%1 %2").arg(element.toPlainText()).arg(absoluteUrl.toString()));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_links(const QString &selectorQuery)
{
    QUrl baseUrl = frame->baseUrl();

    QWebElementCollection news_collection = frame->findAllElements(selectorQuery);
    emit debug(QString("count %1").arg(news_collection.count()));
    urls_collection.clear();
    foreach (QWebElement element, news_collection)
    {
        QWebElement link = element.findFirst(":link");
        QString href = link.attribute("href");
        if (!href.isEmpty())
        {
            QUrl relativeUrl(href);
            QUrl absoluteUrl = baseUrl.resolved(relativeUrl);
            urls_collection.append(absoluteUrl);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_url(const QString &address)
{
    QString validLink = address;

    //Добавляем информацию о протоколе, если она совсем отсутствует
    if(!validLink.toLower().startsWith("http"))
    {
        validLink = "http://" + validLink;
    }

    QUrl url(validLink);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

    ui->webView->load(request);
}
//--------------------------------------------------------------------------------
void MainBox::save_content(const QString &filename,
                           const QWebElement &content)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        file.write(content.toInnerXml().toAscii());
#else
        file.write(content.toInnerXml().toLocal8Bit());
#endif
        file.close();
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_and_save(const QString &selector_title, const QString &selector_content)
{
    emit info("selectors:");
    emit info(QString("title %1").arg(selector_title));
    emit info(QString("content %1").arg(selector_content));

    emit debug("find title");
    QWebElement title = frame->findFirstElement(selector_title);
    if(title.isNull())
    {
        emit error("title not found");
        return;
    }

    emit debug("find content");
    QWebElement content = frame->findFirstElement(selector_content);
    if(content.isNull())
    {
        emit error("content not found");
        return;
    }

    page pg;
    pg.url   = frame->url().toString();
    pg.title = title.toPlainText();
    pg.content  = content.toInnerXml();
    pages.append(pg);
}
//--------------------------------------------------------------------------------
void MainBox::print_page_only(void)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    emit info(ui->webView->url().encodedHost());
#endif
    //find_and_save("div[class=page_only]");
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    is_loaded = false;
    emit debug("started");
}
//--------------------------------------------------------------------------------
void MainBox::finished(bool state)
{
    if(state == false) return;
    emit debug(QString("bytes received = %1")
               .arg(ui->webView->page()->bytesReceived()));
    ui->progressBar->reset();
    is_loaded = true;
    emit debug("finished");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::wait(void)
{
    while(!is_loaded)
    {
        QCoreApplication::processEvents();
    }
}
//--------------------------------------------------------------------------------
void MainBox::wait_element(const QString &selector_element)
{
    while(1)
    {
        QCoreApplication::processEvents();
        QWebElement form = frame->findFirstElement(selector_element);
        if(!form.isNull()) break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read(const QString &address,
                   const QString &selector_link,
                   const QString &selector_title,
                   const QString &selector_content)
{
    load_url(address);
    wait();
    //------
    find_links(selector_link);
    //------
    int n=0;
    foreach (QUrl url, urls_collection)
    {
        load_url(url.toString());
        wait();
        find_and_save(selector_title, selector_content);
        if(n>2) break;
        n++;
    }
    //------
    for(int n=0; n<pages.count(); n++)
    {
        QWidget *frame = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout;
        QLineEdit *le_title = new QLineEdit;
        QTextEdit *te_content = new QTextEdit;

        le_title->setText(pages[n].title);
        te_content->setText(QString("%1<br><a href='%2'>Источник</a>")
                            .arg(pages[n].content)
                            .arg(pages[n].url));

        vbox->addWidget(le_title);
        vbox->addWidget(te_content);
        frame->setLayout(vbox);

        ui->tabWidget->addTab(frame, pages[n].url);
    }
    //------
    emit info("the end!");
}
//--------------------------------------------------------------------------------
void MainBox::login(const QString &address,
                    const QString &login_name,
                    const QString &login,
                    const QString &login_password,
                    const QString &password,
                    const QString &submit_name)
{
    load_url(address);
    wait();

    is_loaded = false;

    if(js_setByName(login_name, login) == false) return;
    if(js_setByName(login_password, password) == false) return;
    js_clickByName(submit_name);

    wait();
}
//--------------------------------------------------------------------------------
void MainBox::write(const QString &address,
                    const QString &title_name,
                    const QString &content_name,
                    const QString &submit_name,
                    bool is_simple)
{
#if 0
    page pg;
    pg.url   = "url";
    pg.title = "title";
    pg.content  = "что-то написано полезное";
    pages.append(pg);
#endif

    if(pages.count() == 0)
    {
        emit error("no data!");
        return;
    }

    QSqlTableModel model;
    model.setTable("content_data");
    bool result = model.select();
    if(result == false) return;

    for(int n=0; n<pages.count(); n++)
    {
        is_loaded = false;
        load_url(address);
        wait();

        find_radioelement_and_click("div[class='block_page page_post_type']", "1");
        find_radioelement_and_click("div[class=page_status]", "publish");

        find_and_evaluateJavaScript("input[name=f_comment_allow]", "this.checked=false");
        find_and_evaluateJavaScript("input[name=f_feed_allow]", "this.checked=false");

        js_setByName(title_name, pages[n].title);

        if(is_simple)
            simple_setByName(content_name, pages[n].content);
        else
            js_setByName(content_name, pages[n].content);

        js_clickByName(submit_name);

        is_loaded = false;
        wait();

        QSqlRecord record = model.record(n);
        // bool published = record.value("published").toBool();
        record.setValue("published", true);
        model.setRecord(n, record);
    }
    model.submitAll();
}
//--------------------------------------------------------------------------------
void MainBox::find_and_evaluateJavaScript(const QString &selector_element,
                                          const QString &java_script)
{
    QWebElement element = frame->findFirstElement(selector_element);
    if(element.isNull())
    {
        emit error("element not found");
        return;
    }
    element.evaluateJavaScript(java_script);
}
//--------------------------------------------------------------------------------
void MainBox::find_radioelement_and_click(const QString &selector_block,
                                          const QString &value)
{
    QWebElement radio_element = frame->findFirstElement(selector_block);
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
int MainBox::open_database(const QString &driver_name, const QString &database_name)
{
    if(db.databaseName() != database_name)
    {
        if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            db = QSqlDatabase::database();
        }
        else
        {
            db = QSqlDatabase::addDatabase(driver_name);
            db.setDatabaseName(database_name);
        }
    }
    bool ok = db.open();
    if(ok == false)
    {
        emit error(QString("db %1 not open!").arg(database_name));
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------------------
void MainBox::view_table(const QString &query)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString(query));

    QTableView *view = new QTableView;
    view->setWindowTitle("view table");
    view->setMinimumSize(1024, 600);

    view->setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    header->setResizeMode(QHeaderView::ResizeToContents);
#endif
    view->setHorizontalHeader(header);

    view->show();
}
//--------------------------------------------------------------------------------
void MainBox::drop_table(const QString &table_name)
{
    QSqlQuery sql;
    bool result = sql.exec(QString("DROP TABLE %1").arg(table_name));
    if(result == false)
    {
        emit error(QString("DROP TABLE %1 failed").arg(table_name));
    }
}
//--------------------------------------------------------------------------------
void MainBox::close_database(const QString &database_name)
{
    if(!db.isValid()) return;
    db.close();
    db.removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
bool MainBox::create_content_data(void)
{
    bool ok;
    QSqlQuery sql;

    sql.prepare("create table content_data ("
                "id integer primary key, "
                "published boolean, "
                "title varchar(200), "
                "content memo)");
    ok = sql.exec();
    if(!ok)
    {
        emit error(QString("query [%1] not exec!").arg(sql.lastQuery()));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::insert_content_data(void)
{
    QSqlQuery sql;

    if(pages.count() == 0)
    {
        emit error("no data");
        return;
    }
    sql.prepare("INSERT INTO content_data (id, published, title, content) "
                "VALUES (:id, :published, :title, :content)");
    for(int i=0; i<pages.count(); i++)
    {
        sql.bindValue(":published", false);
        sql.bindValue(":id",      i);
        sql.bindValue(":title",   pages[i].title);
        sql.bindValue(":content", pages[i].content);
        sql.exec();
    }
}
//--------------------------------------------------------------------------------
void MainBox::temp(void)
{
    pages.clear();

#if 0
    read("http://www.securitylab.ru",
         "h2",
         "h1[itemprop=name]",
         "div[itemprop=articleBody]");
#endif
#if 1
    read("http://www.opennet.ru",
         "td[class=tnews]",
         "span[id=r_title]",
         "td[class=chtext]");
#endif
#if 0
    login("http://www.home.net/svow/login",
          "flogin_user",
          "admin",
          "flogin_password",
          "q12345",
          "flogin_submit");
#endif
#if 0
    is_loaded = false;
    load_url("http://www.home.net/svow/admin");
    wait();
#endif
#if 0
    write("http://www.home.net/svow/admin/page_new",
          "f_header",
          "f_content",
          "f_submit");
#endif
#if 1
    open_database("QSQLITE", "customdb");
#endif
#if 1
    create_content_data();
    insert_content_data();
#endif
#if 0
    emit debug("+++++++++++++++++++++++++++++++++++++++++");
    for(int n=0; n<pages.count(); n++)
    {
        sql.exec(QString("SELECT * FROM content_data WHERE title LIKE '%1'").arg(pages[n].title));
        bool result = sql.first();
        if(result == false)
        {
            emit debug(pages[n].title);
        }
    }
    emit debug("+++++++++++++++++++++++++++++++++++++++++");

    view_table("SELECT * FROM content_data");
    //drop_table("content_data");
    close_database("customdb");
#endif
    view_table("SELECT * FROM content_data");
    close_database("customdb");
#if 0
    is_loaded = false;
    load_url("http://linux.moy.su");
    wait();

    QWebElementCollection links_collection = frame->findAllElements(":link");
    emit debug(QString("count %1").arg(links_collection.count()));
    foreach (QWebElement element, links_collection)
    {
        if(element.toPlainText() == "Вход")
        {
            QString js =
                    "var actuate = function(obj) {"
                    "   if (obj.click) {"
                    "       obj.click();"
                    "       return;"
                    "   } else {"
                    "       var e = document.createEvent('MouseEvents');"
                    "       e.initEvent('click', true, true);"
                    "       obj.dispatchEvent(e);"
                    "   }"
                    "};"
                    "actuate(this);";
            element.evaluateJavaScript(js);
        }
    }

    is_loaded = false;
    wait_element("form[id=frmLg555]");
    js_setByName("user", "tux");
    js_setByName("password", "q71103008");
    js_clickByName("submfrmLg555");
    wait();

    for(int n=0; n<5; n++)
    {
        is_loaded = false;
        load_url("http://linux.moy.su/news/0-0-0-0-1");
        wait();

        js_setByName("title", QString("тест #%1").arg(n));
        js_setByName("message", QString("мессага #%1").arg(n));
        js_clickByName("sbm");
        wait_element("div[class=myWinSuccess]");
    }

    is_loaded = false;
    load_url("http://linux.moy.su");
    wait();
#endif
    emit debug("OK!");
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    pages.clear();

#if 0
    read("http://www.securitylab.ru",
         "h2",
         "h1[itemprop=name]",
         "div[itemprop=articleBody]");
#endif
#if 1
    read("http://www.opennet.ru",
         "td[class=tnews]",
         "span[id=r_title]",
         "td[class=chtext]");
#endif
#if 0
    QMessageBox::information(this, "info", QString("found %1").arg(pages.count()), QMessageBox::Ok);
#endif

    login("http://www.home.net/svow/login",
          "flogin_user",
          "admin",
          "flogin_password",
          "q12345",
          "flogin_submit");

    open_database("QSQLITE", "customdb.db");
    QStringList tables = db.tables();
    if(tables.isEmpty())
    {
        emit error("not tables in database");
        create_content_data();
    }
    insert_content_data();

    write("http://www.home.net/svow/admin/page_new",
          "f_header",
          "f_content",
          "f_submit",
          true);

    view_table("SELECT * FROM content_data");
    close_database("customdb");
}
//--------------------------------------------------------------------------------
