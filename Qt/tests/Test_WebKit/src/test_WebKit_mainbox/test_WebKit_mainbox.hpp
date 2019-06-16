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
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QWebElement>
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
#include <QtSql>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
struct page
{
    QString url;
    QString title;
    QString content;
};
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
class QListWidgetItem;
class QNetworkReply;
class QProgressBar;
class QWebFrame;
class TableView;
class WebView;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    ~MainBox();

private slots:
    void open_url(void);

    void run(void);
    void test(void);
    void test2(void);
    void test(QUrl url);
    void find_and_append(void);
    void test_securitylab(void);
    void test_login(void);

    void get_proxies(const QString &address);

    void show_pages(void);
    void show_content_data(void);
    void show_cookies(void);

    void started(void);
    void finished(bool);

    void replyFinished(QNetworkReply *reply);

    void get_rank(void);

    void generate_random_name(void);

    void itemChanged(QListWidgetItem *item);

    void add_news(void);
    void del_news(void);

    void always_yes(void);

    bool append_proxy(const QString &ip_and_port);

private:
    Ui::MainBox *ui;

    WebView *browser;
    QWebFrame *main_frame;
    QLineEdit *leAddress;
    TableView *table_view;
    QProgressBar *progressBar;
    bool is_loaded = false;
    QVector<page> pages;
    QStringList proxies;

    bool create_content_data(void);

    void find_and_evaluateJavaScript(const QString &selector_element,
                                     const QString &java_script);
    void find_checkboxelement_and_click(const QString &selector_block,
                                        const QString &value);
    void find_radioelement_and_click(const QString &selector_block,
                                     const QString &value);
    bool simple_setByName(const QString &name,
                          const QVariant &value,
                          bool isInnerXml = true);

    void init(void);
    void createUrlBar(void);
    void load_url(const QString &address,
                  bool show_address = true);

    bool find_exit(const QString &address,
                   const QString &exit_string);

    void wait_element(const QString &selector_element,
                      int timeout_msec = 30000);

    QString html_to_bbcode(const QString &data);

    void func(int paramCount, ...);

    QString get_link(QWebElement element);
    QStringList get_links(QWebElementCollection collection);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
