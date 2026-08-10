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
#ifndef JAVASCRIPT_HPP
#define JAVASCRIPT_HPP
//--------------------------------------------------------------------------------
#include <QWebElement>
#include <QObject>
//--------------------------------------------------------------------------------
class JavaScript : public QObject
{
    Q_OBJECT
public:
    JavaScript(QObject *parent = nullptr);

    static bool setByName(QWebFrame *main_frame,
                          const QString &name,
                          const QVariant &value);
    static bool setById(QWebFrame *main_frame,
                        const QString &id,
                        const QVariant &value);
    static void clickByName(QWebFrame *main_frame,
                            const QString& name);
    static void clickById(QWebFrame *main_frame,
                          const QString& id);
    static void click_element(QWebElement element);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:

};
//--------------------------------------------------------------------------------
#endif // JAVASCRIPT_HPP
