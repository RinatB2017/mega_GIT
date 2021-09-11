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
//--------------------------------------------------------------------------------
#include "persistentcookiejar.hpp"
#include "websettingbox.hpp"
#include "webview.hpp"
//--------------------------------------------------------------------------------
#include "javascript.hpp"
#include "qxmlputget.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
WebView::WebView(QWidget *parent)
    : QWebView(parent)
{
    init();;
}
//--------------------------------------------------------------------------------
void WebView::init(void)
{
    ATTR temp;

    attributes.clear();

    temp.attribute = QWebSettings::AutoLoadImages;
    temp.name = "AutoLoadImages";
    temp.tooltip = "Specifies whether images are automatically loaded in web pages.\n"
                   "This is enabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptEnabled;
    temp.name = "JavascriptEnabled";
    temp.tooltip = "Enables or disables the running of JavaScript programs.\n"
                   "This is enabled by default";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::JavaEnabled;
    temp.name = "JavaEnabled";
    temp.tooltip = "Enables or disables Java applets. Currently Java applets are not supported.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::PluginsEnabled;
    temp.name = "PluginsEnabled";
    temp.tooltip = "Enables or disables plugins in Web pages (e.g. using NPAPI).\n"
                   "Qt plugins with a mimetype such as \"application/x-qt-plugin\" are not affected by this setting. This is disabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::PrivateBrowsingEnabled;
    temp.name = "PrivateBrowsingEnabled";
    temp.tooltip = "Private browsing prevents WebKit from recording visited pages in the history and storing web page icons.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanOpenWindows;
    temp.name = "JavascriptCanOpenWindows";
    temp.tooltip = "Specifies whether JavaScript programs can open new windows.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanAccessClipboard;
    temp.name = "JavascriptCanAccessClipboard";
    temp.tooltip = "Specifies whether JavaScript programs can read or write to the clipboard.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::DeveloperExtrasEnabled;
    temp.name = "DeveloperExtrasEnabled";
    temp.tooltip = "Enables extra tools for Web developers. Currently this enables the \"Inspect\" element in the context menu as well as the use of QWebInspector which controls the web inspector for web site debugging.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::LinksIncludedInFocusChain;
    temp.name = "LinksIncludedInFocusChain";
    temp.tooltip = "Specifies whether hyperlinks should be included in the keyboard focus chain.\n"
                   "This is enabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::ZoomTextOnly;
    temp.name = "ZoomTextOnly";
    temp.tooltip = "Specifies whether the zoom factor on a frame applies only to the text or to all content.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::PrintElementBackgrounds;
    temp.name = "PrintElementBackgrounds";
    temp.tooltip = "Specifies whether the background color and images are also drawn when the page is printed.\n"
                   "This is enabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::OfflineStorageDatabaseEnabled;
    temp.name = "OfflineStorageDatabaseEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 offline storage feature is enabled or not.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::OfflineWebApplicationCacheEnabled;
    temp.name = "OfflineWebApplicationCacheEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 web application cache feature is enabled or not.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalStorageEnabled;
    temp.name = "LocalStorageEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 local storage feature is enabled or not.\n"
                   "This is disabled by default. (This value was introduced in 4.6.)";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalStorageDatabaseEnabled;
    temp.name = "LocalStorageDatabaseEnabled";
    temp.tooltip = "This enum value is deprecated. Use QWebSettings::LocalStorageEnabled instead.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalContentCanAccessRemoteUrls;
    temp.name = "LocalContentCanAccessRemoteUrls";
    temp.tooltip = "Specifies whether locally loaded documents are allowed to access remote urls.\n"
                   "This is disabled by default. For more information about security origins and local vs. remote content see QWebSecurityOrigin. (This value was introduced in 4.6.)";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::DnsPrefetchEnabled;
    temp.name = "DnsPrefetchEnabled";
    temp.tooltip = "Specifies whether QtWebkit will try to pre-fetch DNS entries to speed up browsing.\n"
                   "This only works as a global attribute. Only for Qt 4.6 and later.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::XSSAuditingEnabled;
    temp.name = "XSSAuditingEnabled";
    temp.tooltip = "Specifies whether load requests should be monitored for cross-site scripting attempts.\n"
                   "Suspicious scripts will be blocked and reported in the inspector's JavaScript console.\n"
                   "Enabling this feature might have an impact on performance and it is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::AcceleratedCompositingEnabled;
    temp.name = "AcceleratedCompositingEnabled";
    temp.tooltip = "This feature, when used in conjunction with QGraphicsWebView, accelerates animations of web content. CSS animations of the transform and opacity properties will be rendered by composing the cached content of the animated elements.\n"
                   "This is enabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::SpatialNavigationEnabled;
    temp.name = "SpatialNavigationEnabled";
    temp.tooltip = "Enables or disables the Spatial Navigation feature, which consists in the ability to navigate between focusable elements in a Web page, such as hyperlinks and form controls, by using Left, Right, Up and Down arrow keys.\n"
                   "For example, if a user presses the Right key, heuristics determine whether there is an element he might be trying to reach towards the right and which element he probably wants.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalContentCanAccessFileUrls;
    temp.name = "LocalContentCanAccessFileUrls";
    temp.tooltip = "Specifies whether locally loaded documents are allowed to access other local urls.\n"
                   "This is enabled by default. For more information about security origins and local vs. remote content see QWebSecurityOrigin.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::TiledBackingStoreEnabled;
    temp.name = "TiledBackingStoreEnabled";
    temp.tooltip = "This setting enables the tiled backing store feature for a QGraphicsWebView.\n"
                   "With the tiled backing store enabled, the web page contents in and around the current visible area is speculatively cached to bitmap tiles. The tiles are automatically kept in sync with the web page as it changes. Enabling tiling can significantly speed up painting heavy operations like scrolling. Enabling the feature increases memory consumption. It does not work well with contents using CSS fixed positioning (see also resizesToContents property). tiledBackingStoreFrozen property allows application to temporarily freeze the contents of the backing store. This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::FrameFlatteningEnabled;
    temp.name = "FrameFlatteningEnabled";
    temp.tooltip = "With this setting each subframe is expanded to its contents.\n"
                   "On touch devices, it is desired to not have any scrollable sub parts of the page as it results in a confusing user experience, with scrolling sometimes scrolling sub parts and at other times scrolling the page itself.\n"
                   "For this reason iframes and framesets are barely usable on touch devices.\n"
                   "This will flatten all the frames to become one scrollable page.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::SiteSpecificQuirksEnabled;
    temp.name = "SiteSpecificQuirksEnabled";
    temp.tooltip = "This setting enables WebKit's workaround for broken sites.\n"
                   "It is enabled by default.";
    temp.value = true;
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanCloseWindows;
    temp.name = "JavascriptCanCloseWindows";
    temp.tooltip = "Specifies whether JavaScript programs can close windows.\n"
                   "This is disabled by default.";
    temp.value = false;
    attributes.append(temp);

    temp.attribute = QWebSettings::WebGLEnabled;
    temp.name = "WebGLEnabled";
    temp.tooltip = "no tooltip";
    attributes.append(temp);

    temp.attribute = QWebSettings::HyperlinkAuditingEnabled;
    temp.name = "HyperlinkAuditingEnabled";
    temp.tooltip = "no tooltip";
    attributes.append(temp);

    load_setting();

    for(int n=0; n<attributes.count(); n++)
    {
        QWebSettings::globalSettings()->setAttribute(attributes[n].attribute, attributes[n].value);
    }

    cookies = new PersistentCookieJar(this, "cookies");
    cookies->load();

    QBotNetworkAccessManager *m_NetManage = new QBotNetworkAccessManager;
    Q_ASSERT(m_NetManage);

    page()->setNetworkAccessManager(m_NetManage);
    page()->networkAccessManager()->setCookieJar(cookies);

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(slot_finished(bool)));
}
//--------------------------------------------------------------------------------
void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = this->page()->createStandardContextMenu();
    Q_ASSERT(menu);
    if(!menu)
    {
        emit info("menu not created!");
        return;
    }
    menu->addSeparator();

    QAction *settings = new QAction(QObject::tr("Settings"), this);
    Q_ASSERT(settings);

    settings->setIcon(QIcon::fromTheme("applications-system"));

    menu->addSeparator();
    QAction *home = new QAction(QObject::tr("Home"), this);
    home->setIcon(QIcon::fromTheme("go-home"));

    menu->addSeparator();
    QAction *goto_in = new QAction(QObject::tr("Go to"), this);
    goto_in->setIcon(QIcon::fromTheme("konqueror"));

    connect(settings, SIGNAL(triggered()), this, SLOT(run_websettingbox()));
    connect(home, SIGNAL(triggered()), this, SLOT(go_home()));
    connect(goto_in, SIGNAL(triggered()), this, SLOT(goto_in()));

    menu->addAction(home);
    menu->addAction(settings);
    menu->addAction(goto_in);
    menu->exec(event->globalPos());
    delete menu;
}
//--------------------------------------------------------------------------------
void WebView::goto_in(void)
{
    bool ok = false;
    QString text = QInputDialog::getText(this,
                                         tr("goto"),
                                         tr("URL:"),
                                         QLineEdit::Normal,
                                         url().toString(),
                                         &ok);
    if(ok && !text.isEmpty())
    {
        load(QUrl(text));
    }
}
//--------------------------------------------------------------------------------
void WebView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->angleDelta().y() > 0)
            setZoomFactor(zoomFactor()+0.1);
        else if (zoomFactor() > 0.1)
            setZoomFactor(zoomFactor()-0.1);
        return;
    }
    QWebView::wheelEvent(event);
}
//--------------------------------------------------------------------------------
void WebView::run_websettingbox(void)
{
    WebSettingBox *vBox = new WebSettingBox(attributes, this);
    Q_ASSERT(vBox);

    int res = vBox->exec();
    if(res != QDialog::Accepted)
    {
        vBox->deleteLater();
        return;
    }

    attributes = vBox->get_attributes();
    for(int n=0; n<attributes.count(); n++)
    {
        QWebSettings::globalSettings()->setAttribute(attributes[n].attribute, attributes[n].value);
    }

    save_setting();
    vBox->deleteLater();
}
//--------------------------------------------------------------------------------
void WebView::load_setting(void)
{
    bool ok = false;

    QXmlGet xmlGet;
    if(!QFile(WEBSETTINGS_XML).exists())
    {
        emit error(QString(tr("file %1 not exists")).arg(WEBSETTINGS_XML));
        return;
    }

    QString error_message;
    int error_line = 0;
    int error_column = 0;
    ok = xmlGet.load(WEBSETTINGS_XML, &error_message, &error_line, &error_column);
    if(ok == false)
    {
        emit error(QString(tr("file %1 not load ERROR: %2 line %3 column %4"))
                   .arg(WEBSETTINGS_XML)
                   .arg(error_message)
                   .arg(error_line)
                   .arg(error_column));
        return;
    }

    while(xmlGet.findNext("attribute"))
    {
        xmlGet.descend();
        QString name;
        bool value = false;
        if(xmlGet.find("name"))
        {
            name = xmlGet.getString();
        }
        if(xmlGet.find("value"))
        {
            value = xmlGet.getBool();
        }
        if(name.isEmpty()) continue;
        for(int n=0; n<attributes.count(); n++)
        {
            if(attributes[n].name == name)
            {
                attributes[n].value = value;
                break;
            }
        }

        xmlGet.rise();
    }
}
//--------------------------------------------------------------------------------
void WebView::save_setting(void)
{
    QXmlPut *xmlPut = new QXmlPut("WebSettings");
    Q_ASSERT(xmlPut);

    for(int n=0; n<attributes.count(); n++)
    {
        xmlPut->descend("attribute");
        xmlPut->putString("name", attributes[n].name);
        xmlPut->putBool("value", attributes[n].value);
        xmlPut->rise();
    }
    xmlPut->save(WEBSETTINGS_XML);
}
//--------------------------------------------------------------------------------
void WebView::load(const QUrl &url)
{
    if(url.isEmpty()) return;

    QString validLink = url.toString();

    // Добавляем информацию о протоколе, если она совсем отсутствует
    if(!validLink.toLower().startsWith("http"))
    {
        validLink = "http://" + validLink;
    }
    home_page = validLink;

    emit info(QString("validLink = %1").arg(validLink));
    QWebView::load(validLink);
}
//--------------------------------------------------------------------------------
QWebView* WebView::createWindow(QWebPage::WebWindowType type)
{
    WebView* page = new WebView;
    Q_ASSERT(page);

    switch(type)
    {
    case QWebPage::WebBrowserWindow:
        emit info("type = QWebPage::WebBrowserWindow");
        break;

    case QWebPage::WebModalDialog:
        emit info("type = QWebPage::WebModalDialog");
        break;

    default:
        emit info(QString("error type %1").arg(type));
        break;
    }

    return page;
}
//--------------------------------------------------------------------------------
void WebView::slot_finished(bool state)
{
    if(state)
    {
        cookies->save();
    }
}
//--------------------------------------------------------------------------------
PersistentCookieJar *WebView::get_cookies(void)
{
    return cookies;
}
//--------------------------------------------------------------------------------
QWebFrame *WebView::get_mainframe(void)
{
    Q_ASSERT(page());
    if(page() == nullptr)
    {
        return 0;
    }
    return page()->mainFrame();
}
//--------------------------------------------------------------------------------
void WebView::go_home(void)
{
    load(home_page);
}
//--------------------------------------------------------------------------------
