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

#include "ui_testwidget.h"
//--------------------------------------------------------------------------------
TestWidget::TestWidget(QWidget *parent)
    : MyWidget(parent),
      ui(new Ui::TestWidget)
{
    init();
}
//--------------------------------------------------------------------------------
TestWidget::~TestWidget()
{
    ui->serial_widget->save_widgets();
    ui->serial_widget_2->save_widgets();

    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void TestWidget::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        ui->serial_widget->load_widgets();
        ui->serial_widget_2->load_widgets();

        mw->add_dock_widget("RS232_1", "rs232_1", Qt::LeftDockWidgetArea,  reinterpret_cast<QWidget *>(ui->serial_widget));
        mw->add_dock_widget("RS232_2", "rs232_2", Qt::RightDockWidgetArea, reinterpret_cast<QWidget *>(ui->serial_widget_2));

        setVisible(false);
    }
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void TestWidget::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &TestWidget::test });

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
    Q_ASSERT(tool_bar);
    Q_ASSERT(tool_button);

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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (TestWidget::*function)(void);
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
bool TestWidget::test(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void TestWidget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool TestWidget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void TestWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void TestWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
