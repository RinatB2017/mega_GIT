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
#include "newmainwindow.hpp"
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
NewMainWindow::NewMainWindow(MainWindow *parent)
    : MainWindow(parent)
{
#ifdef QT_DEBUG
    create_test_bar();
    show_objectNames();
#endif

    QPushButton *btn = new QPushButton(this);
    btn->setText("PUSH");
    connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(push()));

    add_dock_widget("RS232",        "serial",       Qt::LeftDockWidgetArea,     new SerialBox5(this));
    add_dock_widget("Grapher",      "grapher",      Qt::RightDockWidgetArea,    new GrapherBox(this));
    add_dock_widget("SpinBox",      "spinbox",      Qt::RightDockWidgetArea,    new QSpinBox(this), true);
    add_dock_widget("PushButton",   "pushbutton",   Qt::RightDockWidgetArea,    btn, true);

    //FIXME явный костыль
    // без centralwidget нельзя нормально расставить доки позже мышкой
    // cw->setVisible(false) не работает
    // cw->hide() не работает
    //QWidget *cw = new QWidget(this);
    //cw->setFixedSize(0, 0);
    //FIXME setCentralWidget(cw);
    //---

    load_setting();
}
//--------------------------------------------------------------------------------
QToolButton *NewMainWindow::add_button(QToolBar *tool_bar,
                                       QToolButton *tool_button,
                                       QIcon icon,
                                       const QString &text,
                                       const QString &tool_tip)
{
    Q_ASSERT(tool_bar);
    Q_ASSERT(tool_button);

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void NewMainWindow::create_test_bar(void)
{
    commands.clear(); int id = 0;
    commands.append({ id++, "test", &NewMainWindow::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    addToolBar(Qt::TopToolBarArea, testbar);

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

    //testbar->setFixedWidth(toolBar->sizeHint().width());
}
//--------------------------------------------------------------------------------
void NewMainWindow::choice_test(void)
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
        typedef bool (NewMainWindow::*function)(void);
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
bool NewMainWindow::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void NewMainWindow::push(void)
{
    emit info("push");
}
//--------------------------------------------------------------------------------
