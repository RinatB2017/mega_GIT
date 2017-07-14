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
#include <QWebElementCollection>
#include <QNetworkRequest>
#include <QWebFrame>

#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "sitewalker.hpp"
#include "waiting.hpp"
#include "webview.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
SiteWalker::SiteWalker(QUrl url,
                       QWidget *parent) :
    QWidget(parent),
    url(url),
    main_frame(0)
{
    init();
}
//--------------------------------------------------------------------------------
void SiteWalker::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void SiteWalker::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void SiteWalker::init(void)
{
    connect_log();

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->setSpacing(0);

    QHBoxLayout *hbox = new QHBoxLayout;
    //hbox->setMargin(0);
    //hbox->setSpacing(0);

    stop_checkbox = new QCheckBox("no stop");
    stop_checkbox->setChecked(true);

    progressBar = new QProgressBar(this);

    hbox->addWidget(stop_checkbox);
    hbox->addWidget(progressBar);

    logBox = new LogBox(this);

    web = new WebView(this);
    main_frame = web->page()->mainFrame();
    connect(web, SIGNAL(loadStarted()), this, SLOT(started()));
    connect(web, SIGNAL(loadFinished(bool)), this, SLOT(finished(bool)));
    connect(web, SIGNAL(loadProgress(int)), progressBar, SLOT(setValue(int)));
    connect(web, SIGNAL(titleChanged(QString)), this, SLOT(setWindowTitle(QString)));

    vbox->addWidget(web);
    vbox->addLayout(hbox);
    vbox->addWidget(logBox);

    setLayout(vbox);

    setWindowTitle(tr("SiteWalker"));
    setWindowIcon(QIcon(QLatin1String(":/mainwindow/computer.png")));

    QTimer::singleShot(1000, this, SLOT(run()));
}
//--------------------------------------------------------------------------------
void SiteWalker::started(void)
{
    is_loaded = false;
}
//--------------------------------------------------------------------------------
void SiteWalker::finished(bool state)
{
    if(state == false) return;
    progressBar->reset();
    //emit debug("finished");
    is_loaded = true;
}
//--------------------------------------------------------------------------------
void SiteWalker::load_url(const QString &address,
                          bool show_address)
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

    if(show_address)
        emit info(QString(tr("Загружаем %1")).arg(validLink));
    is_loaded = false;
    web->load(request);

    Waiting::is_loaded(&is_loaded);
}
//--------------------------------------------------------------------------------
void SiteWalker::run(void)
{
    emit debug("run()");
    if(url.isValid() == false)
    {
        emit error(QString(tr("url %1 not valid")).arg(url.toString()));
        return;
    }

    QString current_url = url.toString();
    QUrl beginUrl = url;
    emit info(beginUrl.toString());

    while(stop_checkbox->isChecked())
    {
        emit info(current_url);
        load_url(current_url);

        QUrl baseUrl = main_frame->baseUrl();

        QWebElementCollection links_collection = main_frame->findAllElements(":link");
        //emit debug(QString(tr("found %1")).arg(links_collection.count()));
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
            int index = qrand() % sl.count();
            current_url = sl.at(index);
        }
        else
        {
            current_url = beginUrl.toString();
        }
        Waiting::sec(1);
    }
    emit info("The end!");
    close();
}
//--------------------------------------------------------------------------------
