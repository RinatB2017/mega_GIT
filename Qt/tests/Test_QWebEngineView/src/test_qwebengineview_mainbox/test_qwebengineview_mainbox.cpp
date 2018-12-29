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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QNetworkProxy>
//--------------------------------------------------------------------------------
#include "ui_test_qwebengineview_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_qwebengineview_mainbox.hpp"
#include "custompage.h"
#include "qxmlputget.h"
#include "highlighter.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets("Test_QWebEngineView");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
	
	//https://support.google.com/webmasters/answer/1061943?hl=ru

    // ширина TAB в символах
    int fontWidth = QFontMetrics(ui->te_js->currentCharFormat().font()).averageCharWidth();
    ui->te_js->setTabStopWidth(3 * fontWidth);

    ui->cb_user_agent->addItem("Linux", "Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
    ui->cb_user_agent->addItem("Windows XP", "Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    ui->cb_user_agent->addItem("Windows 7", "Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
    ui->cb_user_agent->addItem("Windows 10", "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
    ui->cb_user_agent->addItem("Windows 10", "Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    ui->cb_user_agent->addItem("IOS 6.0", "iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");

    ui->cb_user_agent->addItem("Googlebot", "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
    ui->cb_user_agent->addItem("Googlebot", "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Safari/537.36");
    ui->cb_user_agent->addItem("Googlebot", "Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
    ui->cb_user_agent->addItem("Googlebot", "Googlebot/2.1 (+http://www.google.com/bot.html)");

    ui->cb_user_agent->setFixedWidth(200);

    QWebEngineProfile *profile = new QWebEngineProfile();
    // изменяем необходимые http-заголовки на свои значения
    //profile->setHttpUserAgent("Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    //profile->setHttpUserAgent("Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");
	
    //profile->setHttpUserAgent("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
	//profile->setHttpUserAgent("Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko; compatible; Googlebot/2.1; +http://www.google.com/bot.html) Safari/537.36");
    //profile->setHttpUserAgent("Mozilla/5.0 (Linux; Android 6.0.1; Nexus 5X Build/MMB29P) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.96 Mobile Safari/537.36 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
	//profile->setHttpUserAgent("Googlebot/2.1 (+http://www.google.com/bot.html)");

    //TODO печеньки
    emit info(QCoreApplication::applicationDirPath() + QLatin1String("/storage"));
    //profile->setPersistentStoragePath(QCoreApplication::applicationDirPath() + QLatin1String("/storage"));
    m_store = profile->cookieStore();
    connect(m_store, &QWebEngineCookieStore::cookieAdded, this, &MainBox::handleCookieAdded);
    m_store->loadAllCookies();
    //---

    new_page = new CustomPage(profile);
    //new_page = new QWebEnginePage(profile);

    connect(new_page,   SIGNAL(err_output(QString)),  this,   SIGNAL(error(QString)));

    ui->webEngineView->setPage(new_page);

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(new_page,           SIGNAL(loadProgress(int)),
            ui->progressBar,    SLOT(setValue(int)));
    connect(new_page,           SIGNAL(loadFinished(bool)),
            this,               SLOT(run_JS(bool)));
    connect(ui->btn_run,        SIGNAL(clicked(bool)),
            this,               SLOT(s_run()));
    connect(ui->btn_default_js, SIGNAL(clicked(bool)),
            this,               SLOT(s_default()));
    connect(ui->btn_run_js,     SIGNAL(clicked(bool)),
            this,               SLOT(test_JS(bool)));
    connect(this,               SIGNAL(send(QString)),
            this,               SLOT(analize(QString)));

    connect(ui->le_address,     SIGNAL(returnPressed()),
            this,               SLOT(s_run()));
    connect(ui->btn_load_js,    SIGNAL(clicked(bool)),
            this,               SLOT(js_load()));
    connect(ui->btn_save_js,    SIGNAL(clicked(bool)),
            this,               SLOT(js_save()));

    connect(ui->btn_get_document_title, SIGNAL(clicked(bool)),
            this,               SLOT(get_document_title()));

    //ui->le_address->setText("https://2ip.ru/");
    ui->le_address->setText("https://cashgo.ru/play/levels/#103");
    //ui->le_address->setText("https://www.youtube.com/");
    //ui->le_address->setText("http://localhost/mso/");
    //ui->le_address->setText("http://localhost/mso/home/next/12");
    //ui->le_address->setText("https://www.avito.ru/krasnodar");
    //ui->le_address->setText("file:///C:/Users/User/Dropbox/HTML/test.html");
    //ui->le_address->setText("file:///home/boss/HDD/Dropbox/HTML/test.html");
    //ui->le_address->setText("http://fgramota.org/game/");

    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    //---
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->setChildrenCollapsible(false);

    ui->groupBox_js->setParent(splitter);
    ui->frame_browser->setParent(splitter);
    splitter->addWidget(ui->frame_browser);
    splitter->addWidget(ui->groupBox_js);
    layout()->addWidget(splitter);
    //---
    
    highlighter_js  = new Highlighter(ui->te_js->document());

    load_proxies();

    ui->progressBar->setValue(0);

    load_widgets("Test_QWebEngineView");
}
//--------------------------------------------------------------------------------
bool MainBox::containsCookie(const QNetworkCookie &cookie)
{
    for (auto c: m_cookies)
    {
        if (c.hasSameIdentifier(cookie))
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::handleCookieAdded(const QNetworkCookie &cookie)
{
    emit info("handleCookieAdded");

    // only new cookies
    if (containsCookie(cookie))
        return;

    m_cookies.append(cookie);
}
//--------------------------------------------------------------------------------
void MainBox::get_document_title(void)
{
    emit trace(Q_FUNC_INFO);

   QString javascript;
   javascript.append("function myFunction()\n");
   javascript.append("{\n");
   javascript.append("  var title = document.title;\n");
   javascript.append("  alert(title);\n");
   javascript.append("  return title;\n");
   javascript.append("}\n");
   javascript.append("myFunction();\n");

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MainBox::js_load(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        load_js(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::js_save(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_js(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::load_js(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    ui->te_js->setText(file.readAll());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::save_js(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    file.write(ui->te_js->toPlainText().replace('\n', "\r\n").toLocal8Bit()); //.toAscii());
    //file.write(ui->te_js->toPlainText().toLocal8Bit()); //.toAscii());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::load_proxies(void)
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

    while(xmlGet.findNext("proxy"))
    {
        xmlGet.descend();
        if (xmlGet.find("uri"))
        {
            QString uri = xmlGet.getString();
            if(uri.isEmpty() == false)
            {
                ui->cb_proxy->addItem(uri);
            }
        }
        xmlGet.rise();
    }
    emit info(QString(tr("load %1 proxy")).arg(ui->cb_proxy->count()));
}
//--------------------------------------------------------------------------------
void MainBox::load_js_default(void)
{
    QString temp;

#if 1
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("        var username = document.getElementsByName('LoginForm[username]');\n");
    temp.append("        username[0].value = 'username';\n");
    temp.append("        var password = document.getElementsByName('LoginForm[password]');\n");
    temp.append("        password[0].value = 'password';\n");
    temp.append("        var buttons = document.getElementsByClassName('btn login-btn');\n");
    temp.append("        buttons[0].click();\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

#if 0
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   var links = document.getElementsByTagName('a');\n");
    temp.append("   var temp = 'false';\n");
    temp.append("   for (var i = 0; i < links.length; i++)\n");
    temp.append("   {\n");

    temp.append("      if(links[i].innerHTML == 'Транспорт')\n");
    temp.append("      {\n");
    temp.append("          temp = 'true';\n");
    temp.append("          links[i].click();\n");
    temp.append("      }\n");

    temp.append("   }\n");
    temp.append("   return temp;\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

#if 0
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   document.getElementById('rf_district_547').checked = true;\n");
    temp.append("   document.getElementById('rf_district_359').checked = true;\n");
    temp.append("   document.getElementById('rf_district_360').checked = true;\n");
    temp.append("   document.getElementById('rf_district_361').checked = true;\n");
    temp.append("   document.getElementById('rf_district_362').checked = true;\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

    ui->te_js->setPlainText(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_run(void)
{
    emit trace(Q_FUNC_INFO);

    QString address = ui->le_address->text();
    if(address.isEmpty())
    {
        emit error("address is empty!");
        return;
    }

    if(ui->cb_use_proxy->isChecked())
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        QString host = ui->cb_proxy->currentText();
        QStringList sl = host.split(':');
        if(sl.count() == 2)
        {
            proxy.setHostName(sl.at(0));
            proxy.setPort(sl.at(1).toInt());

            emit info(QString("proxy %1:%2")
                      .arg(sl.at(0))
                      .arg(sl.at(1)));
        }
        else
        {
            emit error("bad address of proxy");
        }
        //webviews[index].webview->page()->networkAccessManager()->setProxy(proxy);
        QNetworkProxy::setApplicationProxy(proxy);
    }
    else
    {
        QNetworkProxy::setApplicationProxy(QNetworkProxy());
    }

    QWebEngineProfile *profile = new QWebEngineProfile();
    profile->setHttpUserAgent(ui->cb_user_agent->itemData(ui->cb_user_agent->currentIndex()).toString());

    new_page->deleteLater();

    new_page = new CustomPage(profile);
    connect(new_page,   SIGNAL(loadProgress(int)),      ui->progressBar,    SLOT(setValue(int)));
    connect(new_page,   SIGNAL(loadFinished(bool)),     this,               SLOT(run_JS(bool)));
    connect(new_page,   SIGNAL(err_output(QString)),    this,               SIGNAL(error(QString)));

    //TODO печеньки
    emit info(QCoreApplication::applicationDirPath() + QLatin1String("/storage"));
    //profile->setPersistentStoragePath(QCoreApplication::applicationDirPath() + QLatin1String("/storage"));
    m_store = profile->cookieStore();
    connect(m_store, &QWebEngineCookieStore::cookieAdded, this, &MainBox::handleCookieAdded);
    m_store->loadAllCookies();
    //---

    ui->webEngineView->setPage(new_page);
    ui->webEngineView->setUrl(QUrl(address));
}
//--------------------------------------------------------------------------------
void MainBox::s_default(void)
{
    emit trace(Q_FUNC_INFO);

    load_js_default();
}
//--------------------------------------------------------------------------------
void MainBox::run_JS(bool)
{
    emit trace(Q_FUNC_INFO);

#if 0
    s_run();
#else
    if(ui->cb_auto_js->isChecked() == false)
    {
        emit trace("no autorun JS");
        return;
    }

    const QString javascript = ui->te_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
#endif
}
//--------------------------------------------------------------------------------
void MainBox::test_JS(bool)
{
    emit trace(Q_FUNC_INFO);

    const QString javascript = ui->te_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MainBox::analize(const QString data)
{
    QStringList sl = data.split(';');
    if(sl.length() > 0)
    {
        emit info(QString("len = %1").arg(sl.length()));
        foreach (QString text, sl)
        {
            emit info(QString("text [%1]").arg(text));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    emit info(new_page->profile()->httpUserAgent());
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
