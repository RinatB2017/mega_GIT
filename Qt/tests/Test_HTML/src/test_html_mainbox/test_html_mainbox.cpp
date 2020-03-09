/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
    if(ui->widget_html->isModified())
    {
        messagebox_warning("error", "HTML not saved");
        //save_html(filename_html);
    }
    if(ui->widget_js->isModified())
    {
        messagebox_warning("error", "JS not saved");
        //save_js(filename_js);
    }
#endif

    save_widgets();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    ui->widget_html->set_extension("html");
    ui->widget_js->set_extension("js");

    QWebEngineProfile *profile = new QWebEngineProfile(this);
    // изменяем необходимые http-заголовки на свои значения
    //profile->setHttpUserAgent("Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    //profile->setHttpUserAgent("Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    profile->setHttpUserAgent("iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");

    new_page = new CustomPage(profile);
    connect(new_page,   SIGNAL(err_output(QString)),  this,   SIGNAL(error(QString)));

    connect(ui->widget_html,    &FileManager::set_data, ui->webEngineView,  &QWebEngineView::setHtml);
    connect(ui->widget_js,      &FileManager::run,      this,   &MainBox::s_run_js);

    ui->webEngineView->setPage(new_page);

    connect(new_page,   SIGNAL(loadFinished(bool)),         this,   SLOT(s_autorun_js()));

    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("HTML", "frame_html",  Qt::LeftDockWidgetArea, ui->frame_html);
        mw->add_dock_widget("JS",   "frame_js",    Qt::LeftDockWidgetArea, ui->frame_js);
    }

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::s_autorun_js(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->cb_auto->isChecked() == false)
    {
        return;
    }

    const QString javascript = ui->widget_js->get_data();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    emit debug(javascript);
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

    const QString javascript = ui->widget_js->get_data();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }

    emit debug(javascript);
    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
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
QString MainBox::get_full_objectName(QWidget *widget)
{
    QStringList sl;
    QWidget *temp = static_cast<QWidget *>(widget);
    do {
        if(temp)
        {
            sl.append(temp->objectName());
        }
        temp = static_cast<QWidget *>(temp->parent());
    } while(temp);

    QString temp_string;
    for(int n=sl.count(); n>0; n--)
    {
        QString str = sl.at(n-1);
        if(str.isEmpty() == false)
        {
            temp_string.append(QString("%1/").arg(str));
        }
    }
    temp_string = temp_string.remove(temp_string.length()-1, 1);
    if(temp_string.isEmpty()) temp_string = "xxx";
    return temp_string;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
    //emit info(QString("%1").arg(ui->te_text_js->document()->isModified()));

#if 1
    QWidgetList wl = qApp->allWidgets();
    foreach (QWidget *widget, wl)
    {
        if(strncmp(widget->metaObject()->className(), QString("QCheckBox").toLocal8Bit(), 9) == 0)
        {
            emit info(get_full_objectName(widget));
        }
    }
#endif

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
