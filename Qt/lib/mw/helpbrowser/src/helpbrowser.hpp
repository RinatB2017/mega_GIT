/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef HELPBROWSER_HPP
#define HELPBROWSER_HPP
//--------------------------------------------------------------------------------
#include <QGuiApplication>
#include <QTextBrowser>
#include <QPushButton>
#include <QScreen>
#include <QWidget>
#include <QEvent>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class HelpBrowser : public QWidget
{ 
    Q_OBJECT
public: 
    explicit HelpBrowser(const QString &page,
                         QWidget *parent = nullptr);
    virtual ~HelpBrowser();

    static void showPage(const QString &page,
                         bool is_maximized = true);

private slots: 
    void updateCaption(QUrl);

private:
    QTextBrowser *textBrowser = nullptr;
    QPushButton  *homeButton = nullptr;
    QPushButton  *backButton = nullptr;
    QPushButton  *closeButton = nullptr;

    void setCaption(const QString &caption);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif //HELPBROWSER_HPP
