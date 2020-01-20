/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "mainwindow.hpp"
#include "test_widget.hpp"
#include "serialbox5.hpp"
//--------------------------------------------------------------------------------
TestWidget::TestWidget(QWidget *parent)
    : QWidget(parent)
{
    createTestBar();
}
//--------------------------------------------------------------------------------
void TestWidget::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &TestWidget::test_0 });
    commands.append({ ID_TEST_1, "test 1", &TestWidget::test_1 });
    commands.append({ ID_TEST_2, "test 2", &TestWidget::test_2 });
    commands.append({ ID_TEST_3, "test 3", &TestWidget::test_3 });
    commands.append({ ID_TEST_4, "test 4", &TestWidget::test_4 });
    commands.append({ ID_TEST_5, "test 5", &TestWidget::test_5 });
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
QToolButton *TestWidget::add_button(QToolBar *tool_bar,
                                    QToolButton *tool_button,
                                    QIcon icon,
                                    const QString &text,
                                    const QString &tool_tip)
{
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);

    tool_button->setIcon(icon);
    tool_button->setText(QObject::tr(text.toLocal8Bit()));
    tool_button->setToolTip(QObject::tr(tool_tip.toLocal8Bit()));
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void TestWidget::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (TestWidget::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool TestWidget::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");
    return true;
}
//--------------------------------------------------------------------------------
bool TestWidget::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool TestWidget::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool TestWidget::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool TestWidget::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool TestWidget::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
