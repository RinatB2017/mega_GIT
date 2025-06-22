/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "template_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MainBox_GUI(parent, splash)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{

}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
#ifdef QT_DEBUG
    create_test_bar();
    show_objectNames();
#endif
    create_programm_bar();

    connect(this,   &MainBox_GUI::s_test,   this,   &MainBox::test);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
                test_commands.begin(),
                test_commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != test_commands.end())
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
void MainBox::choice_programm(void)
{
    bool ok = false;
    int cmd = cb_programm->itemData(cb_programm->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
                programm_commands.begin(),
                programm_commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != programm_commands.end())
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
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        test_commands.clear(); int id = 0;
        test_commands.append({ id++, "test", &MainBox::test });

        test_bar = new QToolBar("testbar");
        Q_ASSERT(test_bar);

        if(mw && test_bar)
        {
            test_bar->setObjectName("testbar");
            mw->addToolBar(Qt::TopToolBarArea, test_bar);

            cb_test = new QComboBox(this);
            Q_ASSERT(cb_test);

            cb_test->setObjectName("cb_test");
            cb_test->setProperty(NO_SAVE, true);
            foreach (CMD command, test_commands)
            {
                cb_test->addItem(command.cmd_text, QVariant(command.cmd));
            }

            test_bar->addWidget(cb_test);
        }

        QToolButton *btn_choice_test = add_button(test_bar,
                                                  new QToolButton(this),
                                                  qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                  "choice_test",
                                                  "choice_test");
        Q_ASSERT(btn_choice_test);

        btn_choice_test->setObjectName("btn_choice_test");

        connect(btn_choice_test,    &QPushButton::clicked,  this,   &MainBox::choice_test);
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_programm_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        programm_commands.clear(); int id = 0;
        programm_commands.append({ id++, "start",   &MainBox::start });
        programm_commands.append({ id++, "stop",    &MainBox::stop });

        programm_bar = new QToolBar("programm_bar");
        programm_bar->setObjectName("programm_bar");
        mw->addToolBar(Qt::TopToolBarArea, programm_bar);

        cb_programm = new QComboBox(this);
        cb_programm->setObjectName("cb_programm");
        cb_programm->setProperty(NO_SAVE, true);
        foreach (CMD command, programm_commands)
        {
            cb_programm->addItem(command.cmd_text, QVariant(command.cmd));
        }

        programm_bar->addWidget(cb_programm);
        QToolButton *btn_choice_programm = add_button(programm_bar,
                                                      new QToolButton(this),
                                                      qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                      "choice_programm",
                                                      "choice_programm");
        btn_choice_programm->setObjectName("btn_choice_programm");

        connect(btn_choice_programm,    &QPushButton::clicked,  this,   &MainBox::choice_programm);
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::start(void)
{
    fail();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::stop(void)
{
    fail();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    int index;
    bool ok;
#ifdef QT_DEBUG
    ok = load_int("test_bar", &index);
    if(ok)
    {
        cb_test->setCurrentIndex(index);
    }
#endif

    ok = load_int("programm_bar", &index);
    if(ok)
    {
        cb_programm->setCurrentIndex(index);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
#ifdef QT_DEBUG
    save_int("test_bar", cb_test->currentIndex());
#endif

    save_int("programm_bar", cb_programm->currentIndex());
}
//--------------------------------------------------------------------------------
