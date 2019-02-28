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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QNetworkProxy>

#include <QNetworkCookie>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class QWebEngineCookieStore;
class QWebEngineProfile;
class QWebEnginePage;

class Highlighter;
class CustomPage;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = nullptr);
    ~MainBox();

signals:
    void send(const QString &);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void run_JS(bool);
    void test_JS(bool);

    void s_run(void);
    void s_default(void);

    void js_load(void);
    void js_save(void);

    void analize(const QString data);

    void handleCookieAdded(const QNetworkCookie &cookie);

    void get_document_title(void);

private:
    Ui::MainBox *ui;
    QWidget *parent;
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    QComboBox *cb_test;
    QList<CMD> commands;

#ifdef USE_CUSTOMPAGE
    CustomPage *new_page;
#else
    QWebEnginePage *new_page;
#endif

    QWebEngineProfile *profile;

    QWebEngineCookieStore *m_store;
    QVector<QNetworkCookie> m_cookies;

    Highlighter *highlighter_js;

    void init(void);
    void createTestBar(void);

    void load_proxies(void);
    void load_js_default(void);

    void load_js(const QString &filename);
    void save_js(const QString &filename);

    bool containsCookie(const QNetworkCookie &cookie);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
