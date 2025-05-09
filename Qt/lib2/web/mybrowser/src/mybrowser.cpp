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
#include "webattributedialog.hpp"
#include "mybrowser.hpp"
#include "defines.hpp"
#include "ui_mybrowser.h"
//--------------------------------------------------------------------------------
// Поисковые роботы https://support.google.com/webmasters/answer/1061943?hl=ru
//--------------------------------------------------------------------------------
MyBrowser::MyBrowser(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyBrowser)
{
    init();
}
//--------------------------------------------------------------------------------
MyBrowser::~MyBrowser()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MyBrowser::init(void)
{
    ui->setupUi(this);

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

    profile = QWebEngineProfile::defaultProfile();

    //TODO печеньки
    QString path_cookies = QCoreApplication::applicationDirPath() + QLatin1String("/storage");
    emit info(path_cookies);
    profile->setCachePath(path_cookies);
    profile->setPersistentStoragePath(path_cookies);
    m_store = profile->cookieStore();
    connect(m_store, &QWebEngineCookieStore::cookieAdded, this, &MyBrowser::handleCookieAdded);
    m_store->loadAllCookies();

#ifdef USE_CUSTOMPAGE
    new_page = new CustomPage(profile);
    connect(new_page,   &CustomPage::err_output,    this,   &MyBrowser::error);
#else
    new_page = new QWebEnginePage(profile);
#endif

    ui->webEngineView->setPage(new_page);

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(new_page,           &QWebEnginePage::loadProgress,
            ui->progressBar,    &QProgressBar::setValue);
    connect(new_page,           &QWebEnginePage::urlChanged,
            this,               &MyBrowser::refresh_url);
    //FIXME надо доделать
    //    connect(new_page,           &QWebEnginePage::loadFinished,
    //            this,               &MyBrowser::run_JS);

    connect(ui->btn_run,        &QToolButton::clicked,
            this,               &MyBrowser::s_run);

    connect(ui->le_address,     &QLineEdit::returnPressed,
            this,               &MyBrowser::s_run);

    connect(ui->webEngineView,  &QWebEngineView::loadStarted,   this,   &MyBrowser::loadStarted);
    connect(ui->webEngineView,  &QWebEngineView::loadFinished,  this,   &MyBrowser::loadFinished);
    connect(ui->webEngineView,  &QWebEngineView::loadProgress,  this,   &MyBrowser::loadProgress);

    //ui->le_address->setText("https://2ip.ru/");
    //ui->le_address->setText("https://cashgo.ru/play/levels/#103");
    //ui->le_address->setText("https://www.youtube.com/");
    //ui->le_address->setText("http://localhost/mso/");
    //ui->le_address->setText("http://localhost/mso/home/next/12");
    //ui->le_address->setText("https://www.avito.ru/krasnodar");
    //ui->le_address->setText("file:///C:/Users/User/Dropbox/HTML/test.html");
    //ui->le_address->setText("file:///home/boss/HDD/Dropbox/HTML/test.html");
    //ui->le_address->setText("http://fgramota.org/game/");
    //ui->le_address->setText("https://free-socks.in");
    //ui->le_address->setText("https://whatismyipaddress.com/");
    //ui->le_address->setText("https://www.youtube.com/watch?v=0vZp3slDGjw");
    ui->le_address->setText("https://coinmarketcap.com/");

    //---
#ifdef SIMPLE_BROWSER
    ui->proxy_label->setVisible(false);
    ui->cb_proxy->setVisible(false);
    ui->cb_use_proxy->setVisible(false);
    ui->cb_user_agent->setVisible(false);
    ui->le_address->setVisible(false);
    ui->btn_run->setVisible(false);
    ui->progressBar->setVisible(false);
#endif
    //---

    //---
    ui->webEngineView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->webEngineView,  &QWebEngineView::customContextMenuRequested,    this,   &MyBrowser::popup);
    //---

    load_proxies();
    ui->progressBar->setValue(0);
}
//--------------------------------------------------------------------------------
void MyBrowser::popup(QPoint)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMenu *popup_menu = new_page->createStandardContextMenu();
    if(popup_menu == nullptr)
    {
        //errorLog("cannot create standard context menu");
        popup_menu = new QMenu;
    }

    popup_menu->setStyleSheet("background:white;color:black;");

    QAction *attributes_action   = new QAction(tr("attributes"),   this);

    popup_menu->addSeparator();
    popup_menu->addAction(attributes_action);

    connect(attributes_action,  &QAction::triggered,    this,   &MyBrowser::set_attributes);

    popup_menu->exec(QCursor::pos());
#endif
}
//--------------------------------------------------------------------------------
void MyBrowser::set_attributes(void)
{
    //FIXME убрать говнокод
    QWebEngineSettings *settings = new_page->settings();

    WebAttributeDialog *dlg = new WebAttributeDialog();

    dlg->set("AutoLoadImages",                  settings->testAttribute(QWebEngineSettings::AutoLoadImages));
    dlg->set("JavascriptEnabled",               settings->testAttribute(QWebEngineSettings::JavascriptEnabled));
    dlg->set("JavascriptCanOpenWindows",        settings->testAttribute(QWebEngineSettings::JavascriptCanOpenWindows));
    dlg->set("JavascriptCanAccessClipboard",    settings->testAttribute(QWebEngineSettings::JavascriptCanAccessClipboard));
    dlg->set("LinksIncludedInFocusChain",       settings->testAttribute(QWebEngineSettings::LinksIncludedInFocusChain));
    dlg->set("LocalStorageEnabled",             settings->testAttribute(QWebEngineSettings::LocalStorageEnabled));
    dlg->set("LocalContentCanAccessRemoteUrls", settings->testAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls));
    dlg->set("XSSAuditingEnabled",              settings->testAttribute(QWebEngineSettings::XSSAuditingEnabled));
    dlg->set("SpatialNavigationEnabled",        settings->testAttribute(QWebEngineSettings::SpatialNavigationEnabled));
    dlg->set("LocalContentCanAccessFileUrls",   settings->testAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls));
    dlg->set("HyperlinkAuditingEnabled",        settings->testAttribute(QWebEngineSettings::HyperlinkAuditingEnabled));
    dlg->set("ScrollAnimatorEnabled",           settings->testAttribute(QWebEngineSettings::ScrollAnimatorEnabled));
    dlg->set("ErrorPageEnabled",                settings->testAttribute(QWebEngineSettings::ErrorPageEnabled));
    dlg->set("PluginsEnabled",                  settings->testAttribute(QWebEngineSettings::PluginsEnabled));
    dlg->set("FullScreenSupportEnabled",        settings->testAttribute(QWebEngineSettings::FullScreenSupportEnabled));
    dlg->set("ScreenCaptureEnabled",            settings->testAttribute(QWebEngineSettings::ScreenCaptureEnabled));
    dlg->set("WebGLEnabled",                    settings->testAttribute(QWebEngineSettings::WebGLEnabled));
    dlg->set("Accelerated2dCanvasEnabled",      settings->testAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled));
    dlg->set("AutoLoadIconsForPage",            settings->testAttribute(QWebEngineSettings::AutoLoadIconsForPage));
    dlg->set("TouchIconsEnabled",               settings->testAttribute(QWebEngineSettings::TouchIconsEnabled));
    dlg->set("FocusOnNavigationEnabled",        settings->testAttribute(QWebEngineSettings::FocusOnNavigationEnabled));
    dlg->set("PrintElementBackgrounds",         settings->testAttribute(QWebEngineSettings::PrintElementBackgrounds));
    dlg->set("AllowRunningInsecureContent",     settings->testAttribute(QWebEngineSettings::AllowRunningInsecureContent));
    dlg->set("AllowGeolocationOnInsecureOrigins",   settings->testAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins));
    dlg->set("AllowWindowActivationFromJavaScript", settings->testAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript));
    dlg->set("ShowScrollBars",                  settings->testAttribute(QWebEngineSettings::ShowScrollBars));
    dlg->set("PlaybackRequiresUserGesture",     settings->testAttribute(QWebEngineSettings::PlaybackRequiresUserGesture));
    dlg->set("WebRTCPublicInterfacesOnly",      settings->testAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly));
    dlg->set("JavascriptCanPaste",              settings->testAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly));
    dlg->set("DnsPrefetchEnabled",              settings->testAttribute(QWebEngineSettings::DnsPrefetchEnabled));
    dlg->set("PdfViewerEnabled",                settings->testAttribute(QWebEngineSettings::PdfViewerEnabled));
    int res = dlg->exec();
    if(res == QDialog::Accepted)
    {
        emit info("OK");

        settings->setAttribute(QWebEngineSettings::AutoLoadImages,                  dlg->get("AutoLoadImages"));
        settings->setAttribute(QWebEngineSettings::JavascriptEnabled,               dlg->get("JavascriptEnabled"));
        settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,        dlg->get("JavascriptCanOpenWindows"));
        settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,    dlg->get("JavascriptCanAccessClipboard"));
        settings->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain,       dlg->get("LinksIncludedInFocusChain"));
        settings->setAttribute(QWebEngineSettings::LocalStorageEnabled,             dlg->get("LocalStorageEnabled"));
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, dlg->get("LocalContentCanAccessRemoteUrls"));
        settings->setAttribute(QWebEngineSettings::XSSAuditingEnabled,              dlg->get("XSSAuditingEnabled"));
        settings->setAttribute(QWebEngineSettings::SpatialNavigationEnabled,        dlg->get("SpatialNavigationEnabled"));
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,   dlg->get("LocalContentCanAccessFileUrls"));
        settings->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled,        dlg->get("HyperlinkAuditingEnabled"));
        settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,           dlg->get("ScrollAnimatorEnabled"));
        settings->setAttribute(QWebEngineSettings::ErrorPageEnabled,                dlg->get("ErrorPageEnabled"));
        settings->setAttribute(QWebEngineSettings::PluginsEnabled,                  dlg->get("PluginsEnabled"));
        settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,        dlg->get("FullScreenSupportEnabled"));
        settings->setAttribute(QWebEngineSettings::ScreenCaptureEnabled,            dlg->get("ScreenCaptureEnabled"));
        settings->setAttribute(QWebEngineSettings::WebGLEnabled,                    dlg->get("WebGLEnabled"));
        settings->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled,      dlg->get("Accelerated2dCanvasEnabled"));
        settings->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,            dlg->get("AutoLoadIconsForPage"));
        settings->setAttribute(QWebEngineSettings::TouchIconsEnabled,               dlg->get("TouchIconsEnabled"));
        settings->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled,        dlg->get("FocusOnNavigationEnabled"));
        settings->setAttribute(QWebEngineSettings::PrintElementBackgrounds,         dlg->get("PrintElementBackgrounds"));
        settings->setAttribute(QWebEngineSettings::AllowRunningInsecureContent,     dlg->get("AllowRunningInsecureContent"));
        settings->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins,   dlg->get("AllowGeolocationOnInsecureOrigins"));
        settings->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, dlg->get("AllowWindowActivationFromJavaScript"));
        settings->setAttribute(QWebEngineSettings::ShowScrollBars,                  dlg->get("ShowScrollBars"));
        settings->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture,     dlg->get("PlaybackRequiresUserGesture"));
        settings->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly,      dlg->get("WebRTCPublicInterfacesOnly"));
        settings->setAttribute(QWebEngineSettings::JavascriptCanPaste,              dlg->get("JavascriptCanPaste"));
        settings->setAttribute(QWebEngineSettings::DnsPrefetchEnabled,              dlg->get("DnsPrefetchEnabled"));
        settings->setAttribute(QWebEngineSettings::PdfViewerEnabled,                dlg->get("PdfViewerEnabled"));
    }
}
//--------------------------------------------------------------------------------
void MyBrowser::setUrl(const QUrl &url)
{
    ui->le_address->setText(url.toString(QUrl::FullyEncoded));
}
//--------------------------------------------------------------------------------
void MyBrowser::run(void)
{
    s_run();
}
//--------------------------------------------------------------------------------
void MyBrowser::load_proxies(void)
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
void MyBrowser::s_run(void)
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
            proxy.setPort(static_cast<quint16>(sl.at(1).toInt()));

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

    profile->setHttpUserAgent(ui->cb_user_agent->itemData(ui->cb_user_agent->currentIndex()).toString());

    ui->webEngineView->load(QUrl(address));
}
//--------------------------------------------------------------------------------
void MyBrowser::get_document_title(void)
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
void MyBrowser::run_javascript(QString javascript)
{
    emit trace(Q_FUNC_INFO);

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MyBrowser::refresh_url(const QUrl &url)
{
    ui->le_address->setText(url.toString());
}
//--------------------------------------------------------------------------------
void MyBrowser::click(QWebEngineView * webView, QPoint pos, Qt::MouseButton button)
{
    // https://www.cyberforum.ru/qt/thread1771644.html

    QWidget* eventsReciverWidget = nullptr;
    foreach(QObject* o, webView->children()) {
        QWidget* wgt = qobject_cast<QWidget*>(o);
        if (wgt) {
            eventsReciverWidget = wgt;
            break;
        }
    }
    if (eventsReciverWidget) {
        QMouseEvent * me;
        me = new QMouseEvent(QEvent::MouseButtonPress , pos, button, button, Qt::NoModifier);
        QApplication::postEvent(eventsReciverWidget, me);
        me = new QMouseEvent(QEvent::MouseButtonRelease, pos, button, button, Qt::NoModifier);
        QApplication::postEvent(eventsReciverWidget, me);
    }
}
//--------------------------------------------------------------------------------
bool MyBrowser::containsCookie(const QNetworkCookie &cookie)
{
    for (auto c: m_cookies)
    {
        if (c.hasSameIdentifier(cookie))
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MyBrowser::handleCookieAdded(const QNetworkCookie &cookie)
{
    //emit trace(Q_FUNC_INFO);

    // only new cookies
    if (containsCookie(cookie))
        return;

    //emit debug(QString("append new cookie = [%1]").arg(cookie.name().data()));
    m_cookies.append(cookie);
}
//--------------------------------------------------------------------------------
void MyBrowser::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyBrowser::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyBrowser::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyBrowser::save_setting(void)
{

}
//--------------------------------------------------------------------------------
