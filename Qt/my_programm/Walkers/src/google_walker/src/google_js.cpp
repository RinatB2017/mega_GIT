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
#include "google_js.hpp"
//--------------------------------------------------------------------------------
Google_JS::Google_JS(QWidget *parent) :
    QWidget(parent)
{
    te = new QTextEdit(this);
    te->setObjectName("te_javascript");

#if 0
    function myFunction()
    {
       var x=document.forms["f"]["q"].value;
       return x;
    }
    myFunction();
#endif

#if 0
    te->append("function myFunction()");
    te->append("{");
    te->append("   var x=document.forms[\"f\"][\"q\"].value;");
    te->append("   return x;");
    te->append("}");
    te->append("myFunction();");
#endif

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(te);

    QPushButton *btn_run = new QPushButton(this);
    btn_run->setText("RUN");
    connect(btn_run,    SIGNAL(clicked(bool)),  this,   SLOT(send_javascript()));

    vbox->addWidget(btn_run);

    setLayout(vbox);
}
//--------------------------------------------------------------------------------
Google_JS::~Google_JS()
{
    if(te)
    {
        te->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void Google_JS::send_javascript(void)
{
    QString text = te->toPlainText();
    if(text.isEmpty())
    {
        return;
    }
    emit send(text);
}
//--------------------------------------------------------------------------------
