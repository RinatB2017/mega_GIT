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
#ifndef HELPBROWSER_HPP
#define HELPBROWSER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QUrl>
//--------------------------------------------------------------------------------
class QPushButton; 
class QTextBrowser; 
//--------------------------------------------------------------------------------
class HelpBrowser : public QWidget 
{ 
    Q_OBJECT
public: 
    HelpBrowser(QString &page,
                QWidget *parent = nullptr,
                const char *name = nullptr);

    static void showPage(QString page,
                         bool is_maximized = true);

private slots: 
    void updateCaption(QUrl);

private:
    QTextBrowser *textBrowser;
    QPushButton *homeButton;
    QPushButton *backButton;
    QPushButton *closeButton;

    void setCaption(const QString &caption);
};
//--------------------------------------------------------------------------------
#endif //HELPBROWSER_HPP
