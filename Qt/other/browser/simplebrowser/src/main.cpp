/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"
#include "webview.h"
#include <QApplication>
#include <QWebEngineSettings>

void set_light_palette(void)
{
    //Light
    QPalette palette;

#ifdef Q_OS_LINUX
    palette.setBrush(QPalette::WindowText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor((QRgb)0x000000), Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor((QRgb)0xffffff), Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor((QRgb)0xd4d4d4), Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor((QRgb)0x404040), Qt::SolidPattern));
#endif

#ifdef Q_OS_WIN
    palette.setBrush(QPalette::WindowText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(0, 0, 0),         Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(255, 255, 255),   Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(240, 240, 240),   Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(105, 105, 105),   Qt::SolidPattern));
#endif

    qApp->setPalette(palette);
}

QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1))
    {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    //return QUrl(QStringLiteral("https://www.qt.io"));
    //return QUrl(QStringLiteral("https://2ip.ru/"));
    return QUrl(QStringLiteral("https://www.google.ru/maps"));
    //return QUrl(QStringLiteral("https://translate.google.com"));
    //return QUrl();
}

int main(int argc, char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStringLiteral(":AppLogoColor.png")));

    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    QUrl url = commandLineUrlArgument();

    Browser browser;
    BrowserWindow *window = browser.createWindow();
    window->currentTab()->setUrl(url);

#ifdef Q_OS_LINUX
    set_light_palette();
#endif

    return app.exec();
}
