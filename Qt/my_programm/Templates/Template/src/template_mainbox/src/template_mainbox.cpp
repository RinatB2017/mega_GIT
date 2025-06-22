/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    create_test_bar();
    show_objectNames();
#endif
    create_programm_bar();

    connect(ui->btn_test,   &QPushButton::clicked,  this,   &MainBox::test);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        test_commands.clear(); int id = 0;
        test_commands.append({ id++,    "test", &MainBox::test });

        QToolBar *testbar = new QToolBar("testbar");
        Q_ASSERT(testbar);
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        Q_ASSERT(cb_test);
        cb_test->setProperty(NO_SAVE, true);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, test_commands)
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
        programm_commands.append({ id++,    "test", &MainBox::test });

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
bool MainBox::test(void)
{
    fail();
    return false;
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
