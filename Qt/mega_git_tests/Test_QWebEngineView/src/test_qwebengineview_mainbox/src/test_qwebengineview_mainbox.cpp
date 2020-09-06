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
#include "ui_test_qwebengineview_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_qwebengineview_mainbox.hpp"
#include "highlighter.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef USE_CUSTOMPAGE
#   include "custompage.h"
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
#if 0
    if(ui->me_js->document()->isModified())
    {
        qDebug() << "JS not saved";
        messagebox_critical("ERROR", "JS not saved");
    }
#endif

    save_widgets();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    // ширина TAB в символах
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    int fontWidth = QFontMetrics(ui->me_js->currentCharFormat().font()).averageCharWidth();
    ui->me_js->setTabStopWidth(3 * fontWidth);
#else
    ui->me_js->setTabStopDistance(QFontMetricsF(ui->me_js->font()).horizontalAdvance(' ') * 3);
#endif

    ui->me_js->setProperty(NO_SAVE, true);

    QFont font("Courier", 10);
    ui->me_js->setFont(font);

    connect(ui->btn_default_js, &QToolButton::clicked,
            this,               &MainBox::s_default);
    connect(ui->btn_run_js,     &QToolButton::clicked,
            this,               &MainBox::test_JS);
    connect(ui->browser_widget, &MyBrowser::send,
            this,               &MainBox::analize);

    connect(ui->btn_load_js,    &QToolButton::clicked,
            this,               &MainBox::js_load);
    connect(ui->btn_save_js,    &QToolButton::clicked,
            this,               &MainBox::js_save);

    connect(ui->btn_get_document_title, &QToolButton::clicked,
            ui->browser_widget,         &MyBrowser::get_document_title);

    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    //---
#if 1
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("JS", "js_dock", Qt::RightDockWidgetArea, reinterpret_cast<QWidget *>(ui->groupBox_js));
    }
#else
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->setChildrenCollapsible(false);

    ui->groupBox_js->setParent(splitter);
    ui->frame_browser->setParent(splitter);
    splitter->addWidget(ui->frame_browser);
    splitter->addWidget(ui->groupBox_js);
    layout()->addWidget(splitter);
#endif
    //---
    
    highlighter_js  = new Highlighter(ui->me_js->document());

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::js_load(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory("/home/boss/Programming/_GitHub/Private/JS/home");
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
void MainBox::js_save(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("JS files (*.js)"));
    dlg->setDefaultSuffix("js");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);

#ifdef Q_OS_LINUX
    dlg->setDirectory("/home/boss/Programming/_GitHub/Private/JS/home");
#endif

#ifdef Q_OS_WIN
    dlg->setDirectory("c:\\Users\\User\\Programming\\GitHub\\Private\\JS\\work");
#endif

    dlg->selectFile("noname");
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_js(files.at(0));
    }
    dlg->deleteLater();
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
    ui->me_js->setPlainText(file.readAll());

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
    file.write(ui->me_js->toPlainText().replace("\r\n", "\n").toLocal8Bit());
    //file.write(ui->me_js->toPlainText().replace('\n', "\r\n").toLocal8Bit());

    file.close();
}
//--------------------------------------------------------------------------------
void MainBox::load_js_default(void)
{
    QString temp;

#if 1
    temp.append("function find_link_and_click(text)\n");
    temp.append("{\n");
    temp.append("   var links = document.getElementsByTagName('a');\n");
    temp.append("   for (var i = 0; i < links.length; i++)\n");
    temp.append("   {\n");
    temp.append("       if(links[i].innerHTML == text)\n");
    temp.append("       {\n");
    temp.append("           links[i].click();\n");
    temp.append("           return;\n");
    temp.append("       }\n");
    temp.append("   }\n");
    temp.append("   alert('not found!');\n");
    temp.append("}\n");
    temp.append("\n");

    temp.append(ui->me_js->toPlainText());
#endif

#if 0
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("        var username = document.getElementsByName('LoginForm[username]');\n");
    temp.append("        var password = document.getElementsByName('LoginForm[password]');\n");
    temp.append("        var buttons = document.getElementsByClassName('btn login-btn');\n");

    temp.append("        username[0].value = 'username';\n");
    temp.append("        password[0].value = 'password';\n");
    temp.append("        buttons[0].click();\n");

    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

#if 0
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   document.getElementById('rf_district_547').checked = true;\n");
    temp.append("   document.getElementById('rf_district_359').checked = true;\n");
    temp.append("   document.getElementById('rf_district_360').checked = true;\n");
    temp.append("   document.getElementById('rf_district_361').checked = true;\n");
    temp.append("   document.getElementById('rf_district_362').checked = true;\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

    ui->me_js->setPlainText(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_default(void)
{
    emit trace(Q_FUNC_INFO);

    load_js_default();
}
//--------------------------------------------------------------------------------
void MainBox::run_JS(bool)
{
    emit trace(Q_FUNC_INFO);

#if 0
    s_run();
#else
    if(ui->cb_auto_js->isChecked() == false)
    {
        emit trace("no autorun JS");
        return;
    }

    const QString javascript = ui->me_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    //FIXME надо сделать
    ui->browser_widget->run_javascript(javascript);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::test_JS(bool)
{
    emit trace(Q_FUNC_INFO);

    const QString javascript = ui->me_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    emit debug(javascript);
    //FIXME надо сделать
    ui->browser_widget->run_javascript(javascript);
}
//--------------------------------------------------------------------------------
void MainBox::analize(const QString data)
{
    QStringList sl = data.split(';');
    if(sl.length() > 0)
    {
        emit info(QString("len = %1").arg(sl.length()));
        foreach (QString text, sl)
        {
            emit info(QString("text [%1]").arg(text));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

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

    connect(btn_choice_test,    &QToolButton::clicked,
            this,               &MainBox::choice_test);

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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
#include "mybrowser.hpp"
bool MainBox::test(void)
{
    emit info("Test_0()");

#if 1
    MyBrowser *browser = new MyBrowser();
    browser->show();
#endif

#if 0
    emit info(ui->multiedit_widget->getCurrentText());
#endif

#if 0
    click(ui->webEngineView, QPoint(500, 500), Qt::LeftButton);
#endif

#if 0
    QStringList *sl = new QStringList();
    new_page->toHtml([sl](QString const &s)
    {
        sl->append(s);
    });
    emit info(QString("ken = %1").arg(sl->count()));
#endif

#if 0
    QPlainTextEdit *te = new QPlainTextEdit();
    new_page->toHtml([te](QString const &s)
    {
        te->appendPlainText(s);
    });
    te->setMinimumSize(1280, 600);
    te->show();
#endif

#if 0
    QByteArray ba;
    ba.append(te->toPlainText());

    QString filename = "html.sav";
    QFile *file = new QFile(filename);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    qint64 bytes = file->write(ba);
    file->close();
    emit info(QString("file %1 is saved [%2 bytes]")
              .arg(filename)
              .arg(bytes));
#endif
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
