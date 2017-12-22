#include "webview.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
    setPage(new WebPage(this));
    setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    settings()->setAttribute(QWebSettings::JavaEnabled,true);
    settings()->setAttribute(QWebSettings::JavascriptEnabled,true);

    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings()->setAttribute(QWebSettings::DnsPrefetchEnabled, true);
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setIconDatabasePath(QString(QCoreApplication::applicationDirPath() + QDir::separator() + "icondatabase"));

    pageAction(QWebPage::Back)->setIcon(QIcon(":/img/back.png"));
    pageAction(QWebPage::Forward)->setIcon(QIcon(":/img/forward.png"));
    pageAction(QWebPage::Reload)->setIcon(QIcon(":/img/reload.png"));
    pageAction(QWebPage::Stop)->setIcon(QIcon(":/img/stop.png"));

    // page()->mainFrame()->evaluateJavaScript("alert('test')"); // Exec JS

    // connect(this->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*, QList<QSslError>)), Qt::UniqueConnection);
}

void WebView::sslErrors(QNetworkReply *reply, const QList<QSslError> &errorList)
{
    /*
    QStringList errorStrings;
    foreach(QSslError error, errorList)
    {
        errorStrings << error.errorString();
    }
    QString message = tr("Risque potentiel de sécurité :\n\n %1 \n Voulez-vous quand même continuer ? ").arg(errorStrings.join("\n"));
    int msgBoxReply = QMessageBox::warning(this, tr("Erreurs SSL"), message, QMessageBox::Ok, QMessageBox::Abort);
    if(msgBoxReply == QMessageBox::Ok)
        reply->ignoreSslErrors();
    */
    Q_UNUSED(reply)
    Q_UNUSED(errorList)
}

// WEBWIDGET

WebWidget::WebWidget(QWidget *parent) :
    QFrame(parent)
{
    m_view = new WebView;
    onLoad = false;

    inspect = new QWebInspector;
    inspect->setPage(m_view->page());
    inspect->hide();

    QSplitter *divisor = new QSplitter(Qt::Horizontal); // Regarder si problème vvient pas de currentvievv()
    divisor->addWidget(m_view);
    divisor->addWidget(inspect);

    QVBoxLayout *center = new QVBoxLayout;
    center->setContentsMargins(0,0,0,0);
    center->addWidget(divisor);
    setLayout(center);

    connect(m_view,SIGNAL(loadStarted()),this,SLOT(loadStarted()));
    connect(m_view,SIGNAL(loadProgress(int)),this,SLOT(loadProgress()));
    connect(m_view,SIGNAL(loadFinished(bool)),this,SLOT(loadFinished(bool)));
}

WebView *WebWidget::view()
{
    return m_view;
}

QWebInspector *WebWidget::inspector()
{
    return inspect;
}

QPixmap WebWidget::screenShoot()
{
    QPixmap pixmap(view()->width(), view()->height());
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view()->render(&painter);
    return pixmap;
}

void WebWidget::loadStarted()
{
    onLoad = true;
}

void WebWidget::loadProgress()
{
    onLoad = true;
}

void WebWidget::loadFinished(bool finished)
{
    if(!finished)
    {
        // Chargement interromppu
    }
    onLoad = false;
}

bool WebWidget::isLoading() const
{
    bool isOk = onLoad;

    return isOk;
}

// WEBPAGE

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
    setForwardUnsupportedContent(true);

    //networkAccessManager()->setCookieJar(new CookieJar(this));
}

bool WebPage::extension(Extension extension, const ExtensionOption *option,
                        ExtensionReturn *output)
{
    if (extension != QWebPage::ErrorPageExtension)
        return QWebPage::extension(extension, option, output);
    const ErrorPageExtensionOption *option2 =
            static_cast<const ErrorPageExtensionOption*>(option);
    // Traitement de l'erreur
    // ...

    // Si tu veux juste recevoir l'erreur, sans l'afficher en tant que page
    // return false;

    // Sinon
    ErrorPageExtensionReturn *output2 = static_cast<ErrorPageExtensionReturn*>(output);
    // Change l'url qui sera affichée au dessus de l'erreur
    output2->baseUrl = option2->url;
    // Renvoie le html à afficher pour indiquer l'erreur
    output2->content = QString(tr(
                                   "<html>"
                                   "  <head>"
                                   "    <title>%1 - %2</title>"
                                   "    <link rel=\"stylesheet\" href=\"qrc:/res/html.css\" />"
                                   "  </head>"
                                   "  <body>"
                                   "    <h1>Error</h1>"
                                   "    <p>%1</p>"
                                   "    <p>Url: %2</p>"
                                   "  </body>"
                                   "</html>"))
            .arg(option2->errorString)
            .arg(option2->url.toString()).toUtf8();
    return true;
}

bool WebPage::supportsExtension(Extension extension) const
{
    if(extension != QWebPage::ErrorPageExtension)
        return QWebPage::supportsExtension(extension);
    return true;
}

void WebPage::triggerAction(WebAction action, bool checked)
{
    QWebPage::triggerAction(action, checked);
    if(action == QWebPage::Stop) {
        // Traitement de l'interruption par l'utilisateur
        // ...
    }
}
