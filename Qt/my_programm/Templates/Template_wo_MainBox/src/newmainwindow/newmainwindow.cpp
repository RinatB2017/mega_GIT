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
    createTestBar();

    add_dock_widget("RS232",  "serial",   Qt::LeftDockWidgetArea,  new SerialBox5(this));
    add_dock_widget("График", "grapher",  Qt::RightDockWidgetArea, new GrapherBox(this));
    add_dock_widget("SpinBox", "spinbox", Qt::RightDockWidgetArea, new QSpinBox(this));

    QPushButton *btn = new QPushButton(this);
    btn->setText("PUSH");
    connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(push()));
    add_dock_widget("PushButton", "pushbutton", Qt::RightDockWidgetArea,  btn);

    //FIXME явный костыль
    // без centralwidget нельзя нормально расставить доки позже мышкой
    // cw->setVisible(false) не работает
    // cw->hide() не работает
    QWidget *cw = new QWidget(this);
    cw->setFixedSize(0, 0);
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
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void NewMainWindow::createTestBar(void)
{
    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &NewMainWindow::test_0 });
    commands.append({ ID_TEST_1, "test 1", &NewMainWindow::test_1 });
    commands.append({ ID_TEST_2, "test 2", &NewMainWindow::test_2 });
    commands.append({ ID_TEST_3, "test 3", &NewMainWindow::test_3 });
    commands.append({ ID_TEST_4, "test 4", &NewMainWindow::test_4 });
    commands.append({ ID_TEST_5, "test 5", &NewMainWindow::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (NewMainWindow::*my_mega_function)(void);
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
bool NewMainWindow::test_0(void)
{
    emit info("Test_0()");

#if 0
    QList<QDockWidget *> dockWidgets = findChildren<QDockWidget *>();
    foreach (auto dw, dockWidgets)
    {
        if(dw->objectName() == "spinbox")
        {
            removeDockWidget(dw);
            addDockWidget(Qt::LeftDockWidgetArea, dw);
            dw->show();
        }
    }
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool NewMainWindow::test_1(void)
{
    emit info("Test_1()");

#if 0
    QList<QDockWidget *> dockWidgets = findChildren<QDockWidget *>();
    foreach (auto dw, dockWidgets)
    {
        if(dw->objectName() == "spinbox")
        {
            removeDockWidget(dw);
            addDockWidget(Qt::RightDockWidgetArea, dw);
            dw->show();
        }
    }
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool NewMainWindow::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool NewMainWindow::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool NewMainWindow::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool NewMainWindow::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void NewMainWindow::push(void)
{
    emit info("push");
}
//--------------------------------------------------------------------------------
