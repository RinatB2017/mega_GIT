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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "google_walker.hpp"
#include "google_js.hpp"
//--------------------------------------------------------------------------------
Google_walker::Google_walker(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Google_walker::~Google_walker()
{
    if(viewer)
    {
        viewer->disconnect();
        viewer->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void Google_walker::init(void)
{
    viewer = new QWebEngineView(this);
    viewer->page()->setAudioMuted(true);

    //---
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    Google_JS *gjs = new Google_JS(this);
    mw->add_dock_widget("JS", "JS", Qt::RightDockWidgetArea, gjs);
    //---

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(viewer);
    setLayout(vbox);

    //connect(viewer->page(), SIGNAL(loadFinished(bool)), this,   SLOT(test_JS(bool)));
    //connect(this,           SIGNAL(send(QString)),      this,   SLOT(find_url(QString)));
}
//--------------------------------------------------------------------------------
void Google_walker::test_JS(bool)
{
    emit info("test_JS");

    QString javascript;
    javascript.append("function myFunction()");
    javascript.append("{");
    javascript.append("   var links = document.getElementsByTagName('a');");
    javascript.append("   var temp = \"\";");
    javascript.append("   for (var i = 0; i < links.length; i++)");
    javascript.append("   {");
    javascript.append("      temp += links[i].href;");
    javascript.append("      temp += \";\";");
    javascript.append("   }");
    javascript.append("   return temp;");
    javascript.append("}");
    javascript.append("myFunction();");

    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }
    //emit trace(javascript);

    viewer->page()->runJavaScript(javascript, [=](const QVariant &v)
    {
        //emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void Google_walker::find_url(const QString &text)
{
    QStringList sl = text.split(";");

    int cnt = 0;
    urls.clear();
    foreach (QString url, sl)
    {
        if(url.contains("watch?"))
        {
            cnt++;
            //emit info(url);
            urls.append(url);
        }
    }
    emit info(QString("find %1 urls").arg(cnt));

    if(cnt <= 0)
    {
        return;
    }

    bool ok = false;
    while(!ok)
    {
        int rand_url = rand() % cnt;
        if(urls.isEmpty())
        {
            return;
        }
        QString new_url = urls.at(rand_url);
        if(current_url != new_url)
        {
            current_url = new_url;
            ok = true;
        }
    }

    int next_time = 15000 + rand() % 20000;

    emit info(QString("next_time %1 ms").arg(next_time));
    emit info(QString("next_url %1").arg(current_url));

#if 0
    QTime time;

    time.start();
    while(time.elapsed() < next_time)
    {
        QCoreApplication::processEvents();
    }
    load_url();
#endif

#if 0
    QTimer::singleShot(next_time, this, SLOT(load_url()));
#endif
}
//--------------------------------------------------------------------------------
void Google_walker::load_url(void)
{
    if(current_url.isEmpty())
    {
        return;
    }
    setUrl(QUrl(current_url));
}
//--------------------------------------------------------------------------------
void Google_walker::setUrl(QUrl url)
{
    //viewer->stop();
    viewer->setUrl(url);
}
//--------------------------------------------------------------------------------
