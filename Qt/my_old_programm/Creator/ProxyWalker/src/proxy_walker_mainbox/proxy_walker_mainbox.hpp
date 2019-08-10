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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

#include <QNetworkReply>
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QProgressBar;
class QListWidget;
class QToolButton;
class QWebFrame;
class QToolBar;
class WebView;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    ~MainBox();

private slots:
    void get_proxies_list(void);
    void check_proxies_list(void);
    void run_walker(void);

    void test(void);
    void stop(void);

    void web_started(void);
    void web_finished(bool state);


private:
    Ui::MainBox *ui;
    bool is_loaded = false;
    WebView *webview;
    bool global_stop_flag = false;
    QWebFrame *main_frame;

    QProgressBar *web_progressBar;
    QProgressBar *result_progressBar;

    QListWidget *proxy_list;

    int current_index;

    void init(void);

    void createTestBar(void);

    void load_url(const QString &address,
                  bool show_address = false);
    void get_proxies(QStringList *proxies,
                     const QString &address);
    bool append_proxy(const QString &ip_and_port);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
