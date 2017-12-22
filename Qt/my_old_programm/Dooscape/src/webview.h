#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QtWebKitWidgets>
#include <QtWidgets>
#include <QtNetwork>

#include "webinfo.h"

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

class WebView : public QWebView
{
    Q_OBJECT

    public:
        explicit WebView(QWidget *parent = 0);

    signals:

    public slots:

    private slots:
        void sslErrors(QNetworkReply *reply, const QList<QSslError> &errorList);

};

class WebWidget : public QFrame
{
    Q_OBJECT

    public:
        explicit WebWidget(QWidget *parent = 0);

        WebView *view();
        QWebInspector *inspector();
        bool isLoading() const;

    signals:

    public slots:
        void loadStarted();
        void loadProgress();
        void loadFinished(bool finished);
        QPixmap screenShoot();

    private slots:

    private:
        WebView *m_view;
        QWebInspector *inspect;
        bool onLoad;
};

class WebPage : public QWebPage
{
    Q_OBJECT

    public:
        explicit WebPage(QObject *parent = 0);

        bool extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output);
        void triggerAction(WebAction action, bool checked);
        bool supportsExtension(Extension extension) const;

};

#endif // WEBVIEW_H
