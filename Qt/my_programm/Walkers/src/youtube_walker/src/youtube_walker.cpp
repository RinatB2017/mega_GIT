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
#include "youtube_walker.hpp"
//--------------------------------------------------------------------------------
Youtube_walker::Youtube_walker(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Youtube_walker::~Youtube_walker()
{

}
//--------------------------------------------------------------------------------
void Youtube_walker::init(void)
{
    viewer = new QWebEngineView(this);
    viewer->page()->setAudioMuted(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(viewer);
    setLayout(vbox);

    connect(viewer->page(), SIGNAL(loadFinished(bool)), this,   SLOT(test_JS(bool)));
    connect(this,           SIGNAL(send(QString)),      this,   SLOT(find_url(QString)));
}
//--------------------------------------------------------------------------------
void Youtube_walker::test_JS(bool)
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
void Youtube_walker::find_url(const QString &text)
{
    QStringList sl = text.split(";");

    int cnt = 0;
    urls.clear();
    foreach (QString url, sl)
    {
        if(url.contains("watch?"))
        {
            cnt++;
            emit info(url);
            urls.append(url);
        }
    }
    emit info(QString("cnt %1").arg(cnt));

    foreach (QString url, urls)
    {
        if(!(rand() % 3))
        {
            emit show_url(url);
        }
    }
}
//--------------------------------------------------------------------------------
void Youtube_walker::setUrl(QUrl url)
{
    viewer->setUrl(url);
}
//--------------------------------------------------------------------------------
