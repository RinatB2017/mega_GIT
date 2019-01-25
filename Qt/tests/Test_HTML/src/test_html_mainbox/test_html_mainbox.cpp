/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QWebEngineProfile>
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_test_html_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_html_mainbox.hpp"
#include "custompage.h"
#include "defines.hpp"

#include "highlighter.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
#if 0
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setIcon(QMessageBox::Warning);
    msgBox->setWindowTitle("title");
    msgBox->setText("text");
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
#endif

#if 1
    if(ui->te_text_html->document()->isModified())
    {
        messagebox_warning("error", "HTML not saved");
    }
    if(ui->te_text_js->document()->isModified())
    {
        messagebox_warning("error", "JS not saved");
    }
#endif

    save_widgets("test_html");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    // ширина TAB в символах
    int fontWidth_html = QFontMetrics(ui->te_text_html->currentCharFormat().font()).averageCharWidth();
    ui->te_text_html->setTabStopWidth(3 * fontWidth_html);

    int fontWidth_js = QFontMetrics(ui->te_text_js->currentCharFormat().font()).averageCharWidth();
    ui->te_text_js->setTabStopWidth(3 * fontWidth_js);

    QFont font("Courier", 10);
    ui->te_text_html->setFont(font);
    ui->te_text_js->setFont(font);

    QWebEngineProfile *profile = new QWebEngineProfile();
    // изменяем необходимые http-заголовки на свои значения
    //profile->setHttpUserAgent("Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    //profile->setHttpUserAgent("Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    profile->setHttpUserAgent("iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");

    new_page = new CustomPage(profile);
    connect(new_page,   SIGNAL(err_output(QString)),  this,   SIGNAL(error(QString)));

    ui->webEngineView->setPage(new_page);

    connect(new_page,               SIGNAL(loadFinished(bool)),     this,   SLOT(s_autorun_js()));

    //TODO
    connect(new_page,               SIGNAL(urlChanged(const QUrl &)),  this,   SLOT(test_0()));

    connect(ui->btn_run_html,       SIGNAL(clicked(bool)),  this,   SLOT(s_run_html()));
    connect(ui->btn_default_html,   SIGNAL(clicked(bool)),  this,   SLOT(s_default_html()));
    connect(ui->btn_load_html,      SIGNAL(clicked(bool)),  this,   SLOT(s_load_html()));
    connect(ui->btn_save_html,      SIGNAL(clicked(bool)),  this,   SLOT(s_save_html()));

    connect(ui->btn_run_js,         SIGNAL(clicked(bool)),  this,   SLOT(s_run_js()));
    connect(ui->btn_default_js,     SIGNAL(clicked(bool)),  this,   SLOT(s_default_js()));
    connect(ui->btn_load_js,        SIGNAL(clicked(bool)),  this,   SLOT(s_load_js()));
    connect(ui->btn_save_js,        SIGNAL(clicked(bool)),  this,   SLOT(s_save_js()));

#if 1
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("HTML", "html_widget",  Qt::LeftDockWidgetArea, ui->groupBox_html);
        mw->add_dock_widget("JS",   "js_widget",    Qt::LeftDockWidgetArea, ui->groupBox_js);
    }
#else
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->setChildrenCollapsible(false);

    ui->groupBox_html->setParent(splitter);
    ui->groupBox_view->setParent(splitter);
    ui->groupBox_js->setParent(splitter);
    splitter->addWidget(ui->groupBox_html);
    splitter->addWidget(ui->groupBox_view);
    splitter->addWidget(ui->groupBox_js);

    layout()->addWidget(splitter);
#endif

    highlighter_cpp = new Highlighter(ui->te_text_html->document());
    highlighter_js  = new Highlighter(ui->te_text_js->document());

    load_widgets("test_html");
}
//--------------------------------------------------------------------------------
void MainBox::s_run_html(void)
{
    emit trace(Q_FUNC_INFO);

    const QString temp = ui->te_text_html->toPlainText();
    if(temp.isEmpty())
    {
        emit error("no data");
        return;
    }
    ui->webEngineView->setHtml(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_default_html(void)
{
    emit trace(Q_FUNC_INFO);

    QString temp;
    temp.append("<!DOCTYPE html>\n");
    temp.append("<html>\n");
    temp.append("<head>\n");
    temp.append("  <meta charset=\"utf-8\">\n");
    temp.append("  <title>Название документа</title>\n");
    temp.append("</head>\n");
    temp.append("<body>\n");

    temp.append("   <button name='btn_test' onclick=btn_test()>Кнопка с текстом</button>\n");

    temp.append("   <p>Нажмите кнопку, чтобы изменить текст в этом абзаце.</p>\n");

    temp.append("  <a href=\"#\" onclick=foo()>тест</a><br>\n");
    temp.append("  <a href=\"#\" onclick=foo1()>1</a><br>\n");
    temp.append("  <a href=\"#\" onclick=foo2()>2</a><br>\n");

    temp.append("  <hr>\n");
    temp.append("  <form action=\"#\">\n");
    temp.append("    cars<select name=\"cars\">\n");
    temp.append("      <option value=\"car1\">car1</option>\n");
    temp.append("      <option value=\"car2\">car2</option>\n");
    temp.append("      <option value=\"car3\">car3</option>\n");
    temp.append("      <option value=\"car4\">car4</option>\n");
    temp.append("    </select>\n");
    temp.append("    <br>\n");
    temp.append("    text <input type=\"text\">\n");
    temp.append("    <br>\n");
    temp.append("    <input type=\"submit\">\n");
    temp.append("  </form>\n");
    temp.append("  <hr>\n");

    temp.append("  <input type=\"radio\" name=\"drink\" value=\"rad1\"> Пиво<Br>\n");
    temp.append("  <input type=\"radio\" name=\"drink\" value=\"rad2\"> Чай<Br>\n");
    temp.append("  <input type=\"radio\" name=\"drink\" value=\"rad3\"> Кофе\n");
    temp.append("  <hr>\n");

    temp.append("  <button onclick=\"foo()\">Попробовать</button>\n");
    temp.append("  <script>\n");
    temp.append("    function foo() {\n");
    temp.append("      document.getElementsByTagName(\"p\")[0].innerHTML=\"Hello World\";\n");
    temp.append("    }\n");
    temp.append("    function foo1() {\n");
    temp.append("      document.getElementsByTagName(\"p\")[0].innerHTML=\"1\";\n");
    temp.append("    }\n");
    temp.append("    function foo2() {\n");
    temp.append("      document.getElementsByTagName(\"p\")[0].innerHTML=\"2\";\n");
    temp.append("    }\n");
    temp.append("    function btn_test() {\n");
    temp.append("      alert('YES');\n");
    temp.append("    }\n");
    temp.append("  </script>\n");
    temp.append("</body>\n");
    temp.append("</html>\n");

    ui->te_text_html->setPlainText(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_autorun_js(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->cb_auto->isChecked() == false)
    {
        return;
    }

    const QString javascript = ui->te_text_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }
    //emit trace(javascript);

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MainBox::s_run_js(void)
{
    emit trace(Q_FUNC_INFO);

    const QString javascript = ui->te_text_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }
    //emit trace(javascript);

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MainBox::s_default_js(void)
{
    emit trace(Q_FUNC_INFO);

    QString temp;
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   var links = document.getElementsByTagName('a');\n");
    temp.append("   var l_str = '';\n");
    temp.append("   for (var i = 0; i < links.length; i++)\n");
    temp.append("   {\n");
    temp.append("      l_str += links[i].href + \";\" + links[i].innerHTML + \";\";\n");

    temp.append("      if(links[i].innerHTML == 'Транспорт')\n");
    temp.append("      {\n");
    temp.append("          links[i].click();\n");
    temp.append("      }\n");

    temp.append("   }\n");
    temp.append("   return l_str;\n");
    temp.append("}\n");
    temp.append("myFunction();\n");

    ui->te_text_js->setPlainText(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_load_html(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("HTML files (*.html)"));
    dlg->setDefaultSuffix("html");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory(".");
#endif

#ifdef Q_OS_WIN
    dlg->setDirectory("c:\\Users\\User\\Programming\\GitHub\\Private\\JS\\work");
#endif

    dlg->selectFile("noname");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        load_html(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::s_save_html(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("HTML files (*.html)"));
    dlg->setDefaultSuffix("html");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory(".");
#endif

#ifdef Q_OS_WIN
    dlg->setDirectory("c:\\Users\\User\\Programming\\GitHub\\Private\\JS\\work");
#endif

    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_html(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::s_load_js(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory(".");
#endif

#ifdef Q_OS_WIN
    dlg->setDirectory("c:\\Users\\User\\Programming\\GitHub\\Private\\JS\\work");
#endif

    dlg->selectFile("noname");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        load_js(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::s_save_js(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory(".");
#endif

#ifdef Q_OS_WIN
    dlg->setDirectory("c:\\Users\\User\\Programming\\GitHub\\Private\\JS\\work");
#endif

    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_js(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::load_html(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    ui->te_text_html->setPlainText(file.readAll());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::save_html(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    file.write(ui->te_text_html->toPlainText().replace('\n', "\r\n").toLocal8Bit()); //.toAscii());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::load_js(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    ui->te_text_js->setPlainText(file.readAll());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::save_js(const QString &filename)
{
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    file.write(ui->te_text_js->toPlainText().replace('\n', "\r\n").toLocal8Bit()); //.toAscii());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    emit info(QString("%1").arg(ui->te_text_js->document()->isModified()));
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
