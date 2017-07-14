//--------------------------------------------------------------------------------
#include <QWebSettings>
#include <QCheckBox>
#include <QVariant>
//--------------------------------------------------------------------------------
#include "websetting.hpp"
//--------------------------------------------------------------------------------
struct ATTR
{
    QWebSettings::WebAttribute attribute;
    QString name;
    QString tooltip;
};
//--------------------------------------------------------------------------------
WebSetting::WebSetting(const QString &title, QWidget *parent) :
    QToolBar(title, parent)
{
    init();
}
//--------------------------------------------------------------------------------
WebSetting::WebSetting(QWidget *parent) :
    QToolBar(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void WebSetting::init(void)
{
    QVector<ATTR> attributes;
    ATTR temp;

    attributes.clear();

    temp.attribute = QWebSettings::AutoLoadImages;
    temp.name = "AutoLoadImages";
    temp.tooltip = "Specifies whether images are automatically loaded in web pages.\n"
            "This is enabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptEnabled;
    temp.name = "JavascriptEnabled";
    temp.tooltip = "Enables or disables the running of JavaScript programs.\n"
            "This is enabled by default";
    attributes.append(temp);

    temp.attribute = QWebSettings::JavaEnabled;
    temp.name = "JavaEnabled";
    temp.tooltip = "Enables or disables Java applets. Currently Java applets are not supported.";
    attributes.append(temp);

    temp.attribute = QWebSettings::PluginsEnabled;
    temp.name = "PluginsEnabled";
    temp.tooltip = "Enables or disables plugins in Web pages (e.g. using NPAPI).\n"
            "Qt plugins with a mimetype such as \"application/x-qt-plugin\" are not affected by this setting. This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::PrivateBrowsingEnabled;
    temp.name = "PrivateBrowsingEnabled";
    temp.tooltip = "Private browsing prevents WebKit from recording visited pages in the history and storing web page icons.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanOpenWindows;
    temp.name = "JavascriptCanOpenWindows";
    temp.tooltip = "Specifies whether JavaScript programs can open new windows.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanAccessClipboard;
    temp.name = "JavascriptCanAccessClipboard";
    temp.tooltip = "Specifies whether JavaScript programs can read or write to the clipboard.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::DeveloperExtrasEnabled;
    temp.name = "DeveloperExtrasEnabled";
    temp.tooltip = "Enables extra tools for Web developers. Currently this enables the \"Inspect\" element in the context menu as well as the use of QWebInspector which controls the web inspector for web site debugging.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::LinksIncludedInFocusChain;
    temp.name = "LinksIncludedInFocusChain";
    temp.tooltip = "Specifies whether hyperlinks should be included in the keyboard focus chain.\n"
            "This is enabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::ZoomTextOnly;
    temp.name = "ZoomTextOnly";
    temp.tooltip = "Specifies whether the zoom factor on a frame applies only to the text or to all content.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::PrintElementBackgrounds;
    temp.name = "PrintElementBackgrounds";
    temp.tooltip = "Specifies whether the background color and images are also drawn when the page is printed.\n"
            "This is enabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::OfflineStorageDatabaseEnabled;
    temp.name = "OfflineStorageDatabaseEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 offline storage feature is enabled or not.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::OfflineWebApplicationCacheEnabled;
    temp.name = "OfflineWebApplicationCacheEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 web application cache feature is enabled or not.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalStorageEnabled;
    temp.name = "LocalStorageEnabled";
    temp.tooltip = "Specifies whether support for the HTML 5 local storage feature is enabled or not.\n"
            "This is disabled by default. (This value was introduced in 4.6.)";
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalStorageDatabaseEnabled;
    temp.name = "LocalStorageDatabaseEnabled";
    temp.tooltip = "This enum value is deprecated. Use QWebSettings::LocalStorageEnabled instead.";
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalContentCanAccessRemoteUrls;
    temp.name = "LocalContentCanAccessRemoteUrls";
    temp.tooltip = "Specifies whether locally loaded documents are allowed to access remote urls.\n"
            "This is disabled by default. For more information about security origins and local vs. remote content see QWebSecurityOrigin. (This value was introduced in 4.6.)";
    attributes.append(temp);

    temp.attribute = QWebSettings::DnsPrefetchEnabled;
    temp.name = "DnsPrefetchEnabled";
    temp.tooltip = "Specifies whether QtWebkit will try to pre-fetch DNS entries to speed up browsing.\n"
            "This only works as a global attribute. Only for Qt 4.6 and later.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::XSSAuditingEnabled;
    temp.name = "XSSAuditingEnabled";
    temp.tooltip = "Specifies whether load requests should be monitored for cross-site scripting attempts.\n"
            "Suspicious scripts will be blocked and reported in the inspector's JavaScript console.\n"
            "Enabling this feature might have an impact on performance and it is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::AcceleratedCompositingEnabled;
    temp.name = "AcceleratedCompositingEnabled";
    temp.tooltip = "This feature, when used in conjunction with QGraphicsWebView, accelerates animations of web content. CSS animations of the transform and opacity properties will be rendered by composing the cached content of the animated elements.\n"
            "This is enabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::SpatialNavigationEnabled;
    temp.name = "SpatialNavigationEnabled";
    temp.tooltip = "Enables or disables the Spatial Navigation feature, which consists in the ability to navigate between focusable elements in a Web page, such as hyperlinks and form controls, by using Left, Right, Up and Down arrow keys.\n"
            "For example, if a user presses the Right key, heuristics determine whether there is an element he might be trying to reach towards the right and which element he probably wants.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::LocalContentCanAccessFileUrls;
    temp.name = "LocalContentCanAccessFileUrls";
    temp.tooltip = "Specifies whether locally loaded documents are allowed to access other local urls.\n"
            "This is enabled by default. For more information about security origins and local vs. remote content see QWebSecurityOrigin.";
    attributes.append(temp);

    temp.attribute = QWebSettings::TiledBackingStoreEnabled;
    temp.name = "TiledBackingStoreEnabled";
    temp.tooltip = "This setting enables the tiled backing store feature for a QGraphicsWebView.\n"
            "With the tiled backing store enabled, the web page contents in and around the current visible area is speculatively cached to bitmap tiles. The tiles are automatically kept in sync with the web page as it changes. Enabling tiling can significantly speed up painting heavy operations like scrolling. Enabling the feature increases memory consumption. It does not work well with contents using CSS fixed positioning (see also resizesToContents property). tiledBackingStoreFrozen property allows application to temporarily freeze the contents of the backing store. This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::FrameFlatteningEnabled;
    temp.name = "FrameFlatteningEnabled";
    temp.tooltip = "With this setting each subframe is expanded to its contents.\n"
            "On touch devices, it is desired to not have any scrollable sub parts of the page as it results in a confusing user experience, with scrolling sometimes scrolling sub parts and at other times scrolling the page itself.\n"
            "For this reason iframes and framesets are barely usable on touch devices.\n"
            "This will flatten all the frames to become one scrollable page.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::SiteSpecificQuirksEnabled;
    temp.name = "SiteSpecificQuirksEnabled";
    temp.tooltip = "This setting enables WebKit's workaround for broken sites.\n"
            "It is enabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::JavascriptCanCloseWindows;
    temp.name = "JavascriptCanCloseWindows";
    temp.tooltip = "Specifies whether JavaScript programs can close windows.\n"
            "This is disabled by default.";
    attributes.append(temp);

    temp.attribute = QWebSettings::WebGLEnabled;
    temp.name = "WebGLEnabled";
    temp.tooltip = "no tooltip";
    attributes.append(temp);

    temp.attribute = QWebSettings::HyperlinkAuditingEnabled;
    temp.name = "HyperlinkAuditingEnabled";
    temp.tooltip = "no tooltip";
    attributes.append(temp);

    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = new QCheckBox(attributes[n].name);
        checkBox->setProperty("Attribute", attributes[n].attribute);
        checkBox->setChecked(QWebSettings::globalSettings()->testAttribute(attributes[n].attribute));
        checkBox->setToolTip(attributes[n].tooltip);
        addWidget(checkBox);

        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(slot_checkbox(bool)));
    }
}
//--------------------------------------------------------------------------------
void WebSetting::slot_checkbox(bool state)
{
    QCheckBox *checkBox = (QCheckBox *)sender();
    int attr = checkBox->property("Attribute").toInt();
    QWebSettings::globalSettings()->setAttribute((QWebSettings::WebAttribute)attr, state);
    //emit debug(QString("%1").arg(attr));
}
//--------------------------------------------------------------------------------
