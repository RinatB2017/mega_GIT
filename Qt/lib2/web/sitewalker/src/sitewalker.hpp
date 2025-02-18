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
#ifndef SITEWALKER_HPP
#define SITEWALKER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QUrl>
//--------------------------------------------------------------------------------
class QProgressBar;
class QCheckBox;
class QWebFrame;
class WebView;
class LogBox;
//--------------------------------------------------------------------------------
class SiteWalker : public QWidget
{
    Q_OBJECT
public:
    explicit SiteWalker(QUrl url,
                        QWidget *parent = nullptr);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);
    void run(void);
    void started(void);
    void finished(bool state);

private:
    QUrl url;

    bool is_loaded;
    WebView *web;
    QWebFrame *main_frame;

    QCheckBox *stop_checkbox;
    QProgressBar *progressBar;
    LogBox *logBox;

    void connect_log(void);
    void init(void);
    void load_url(const QString &address,
                  bool show_address = false);

};
//--------------------------------------------------------------------------------
#endif // SITEWALKER_HPP
