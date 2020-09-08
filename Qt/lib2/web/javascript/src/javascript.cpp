/*********************************************************************************
**                                                                              **
**     Copyright (C) 2013                                                       **
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
#include <QWebElement>
#include <QWebFrame>
#include <QVariant>
#include <QString>
//--------------------------------------------------------------------------------
#include "javascript.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
JavaScript::JavaScript(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
bool JavaScript::setByName(QWebFrame *main_frame,
                           const QString& name,
                           const QVariant& value)
{
    Q_CHECK_PTR(main_frame);
    QVariant result = main_frame->evaluateJavaScript(QString("document.getElementsByName('%1')[0].value='%2';")
                                                     .arg(name)
                                                     .arg(value.toString()));
    if(result.isNull())
    {
        QString temp = QString("Error in js_setByName name=[%1] value=[%2]")
                .arg(name)
                .arg(value.toString());
        qDebug() << temp;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool JavaScript::setById(QWebFrame *main_frame,
                         const QString& id,
                         const QVariant& value)
{
    Q_CHECK_PTR(main_frame);

    QVariant result = main_frame->evaluateJavaScript(QString("document.getElementsById('%1')[0].value='%2';")
                                                     .arg(id)
                                                     .arg(value.toString()));
    if(result.isNull())
    {
        QString temp = QString("Error in js_setById name=[%1] value=[%2]")
                .arg(id)
                .arg(value.toString());
        qDebug() << temp;
        return false;
    }
    return result.isNull();
}
//--------------------------------------------------------------------------------
void JavaScript::clickByName(QWebFrame *main_frame,
                             const QString& name)
{
    Q_CHECK_PTR(main_frame);
    main_frame->evaluateJavaScript(QString("document.getElementsByName('%1')[0].click();")
                                   .arg(name));
}
//--------------------------------------------------------------------------------
void JavaScript::clickById(QWebFrame *main_frame,
                           const QString& id)
{
    Q_CHECK_PTR(main_frame);
    main_frame->evaluateJavaScript(QString("document.getElementsById('%1')[0].click();")
                                   .arg(id));
}
//--------------------------------------------------------------------------------
void JavaScript::click_element(QWebElement element)
{
    if(element.isNull())
    {
        qDebug() << "element is null";
        return;
    }

    QString js =
            "var actuate = function(obj) {"
            "   if (obj.click) {"
            "       obj.click();"
            "       return true;"
            "   } else {"
            "       var e = document.createEvent('MouseEvents');"
            "       e.initEvent('click', true, true);"
            "       obj.dispatchEvent(e);"
            "   }"
            "};"
            "actuate(this); null;";
    element.evaluateJavaScript(js);
}
//--------------------------------------------------------------------------------
