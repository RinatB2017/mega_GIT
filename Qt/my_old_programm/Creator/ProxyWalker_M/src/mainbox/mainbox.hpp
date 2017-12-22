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
#include <QWidget>
#include <QTime>
#include <QUrl>
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
class QLCDNumber;
class QWebFrame;
class QToolBar;
class WebView;
class QTimer;
//--------------------------------------------------------------------------------
struct web_struct
{
    bool is_block;
    bool is_loaded;     // страница загружена?
    int count;          // сколько страниц смотрим, до 0
    WebView *webview;   // текущий WebView
    QWebFrame *main_frame;
    QProgressBar *progressBar;
    QTime time;
};
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = 0);
    ~MainBox();

protected:
    void changeEvent(QEvent *e);

private slots:
    void get_proxies_list(void);
    void check_proxies_list(void);
    void run_walker(void);

    void test(void);
    void stop(void);

    void web_started(void);
    void web_finished(bool state);

    void save_setting(void);

private:
    Ui::MainBox *ui = 0;
    QList <web_struct> webviews;
    bool global_stop_flag;
    QProgressBar *result_progressBar = 0;
    QListWidget *proxy_list = 0;
    QAction *save_action = 0;
    int current_index = 0;

    void init(void);

    void create_menu(void);

    void createTestBar(void);

    void load_setting(void);

    void load_url(int index,
                  const QString &address,
                  bool show_address = false,
                  bool waiting_load = true);
    void get_proxies(QStringList *proxies,
                     const QString &address);
    bool append_proxy(int index,
                      const QString &ip_and_port);

    QString get_random_url(int index);
    bool is_not_working(void);
    int check_url(int index,
                  const QString &check_string);
    int next_proxy(int index);
    void init_webview(int index,
                      const QString &proxy,
                      QUrl url);

protected:

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
