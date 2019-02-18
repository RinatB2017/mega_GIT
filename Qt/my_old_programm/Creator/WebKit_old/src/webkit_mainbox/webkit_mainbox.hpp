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
#include <QWidget>
#include <QtSql>
#include <QUrl>
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
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QWebFrame;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    ~MainBox();

private slots:
    void temp(void);
    void test(void);

    void started(void);
    void finished(bool);

    void find_links(void);
    void find_links(const QString &selectorQuery);

private:
    Ui::MainBox *ui;

    bool is_loaded = false;
    QList<QUrl> urls_collection;
    int urls_index;
    QVector<page> pages;
    QWebFrame *frame;
    QString jQuery;

    QSqlDatabase db;
    int  open_database(const QString &driver_name, const QString &database_name);
    void view_table(const QString &query);
    void drop_table(const QString &table_name);
    void close_database(const QString &database_name);

    bool create_content_data(void);
    void insert_content_data(void);

    void load_jquery(void);
    void save_content(const QString &filename,
                      const QWebElement &content);

    void init(void);
    void print_page_only(void);
    void load_url(const QString &address);

    bool js_setByName(const QString& name,
                      const QVariant& value);

    void js_clickByName(const QString& name);

    bool simple_setByName(const QString& name,
                          const QVariant& value);

    void find_and_save(const QString &selector_title,
                       const QString &selector_content);

    void login(const QString &address,
               const QString &login_name,
               const QString &login,
               const QString &login_password,
               const QString &password,
               const QString &submit_name);

    void read(const QString &address,
              const QString &selector_link,
              const QString &selector_title,
              const QString &selector_content);

    void write(const QString &address,
               const QString &title_name,
               const QString &content_name,
               const QString &submit_name,
               bool is_simple = false);

    void find_radioelement_and_click(const QString &selector_block,
                                     const QString &value);

    void find_and_evaluateJavaScript(const QString &selector_element,
                                     const QString &java_script);

    void wait(void);
    void wait_element(const QString &selector_element);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
