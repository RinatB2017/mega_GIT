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
#include "ui_template_rs232_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "template_rs232_mainbox.hpp"
#include "defines.hpp"
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

    init_serial();
    init_serial_lite();
    init_serial_fix();

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    QTimer::singleShot(100, [this]{
        ui->serial_widget->set_caption("RS232_5");
    });
    connect(this,               static_cast<void (MainBox::*)(const QByteArray&)>(&MainBox::send),
            ui->serial_widget,  static_cast<int (SerialBox5::*)(const QByteArray&)>(&SerialBox5::input));
    connect(ui->serial_widget,  &SerialBox5::output,
            this,               &MainBox::read_data);
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_lite(void)
{
    QTimer::singleShot(100, [this]{
        ui->serial_lite_widget->set_caption("RS232_5 (lite)");
    });
    connect(this,                   static_cast<void (MainBox::*)(const QByteArray&)>(&MainBox::send),
            ui->serial_lite_widget, static_cast<int (SerialBox5_lite::*)(const QByteArray&)>(&SerialBox5_lite::input));
    connect(ui->serial_lite_widget, &SerialBox5_lite::output,
            this,                   &MainBox::read_data);
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_fix(void)
{
    QTimer::singleShot(100, [this]{
        ui->serial_fix_widget->set_caption("RS232_5 (fix)");
    });
    connect(this,                   static_cast<void (MainBox::*)(const QByteArray&)>(&MainBox::send),
            ui->serial_fix_widget,  static_cast<int (SerialBox5_fix_baudrate::*)(const QByteArray&)>(&SerialBox5_fix_baudrate::input));
    connect(ui->serial_fix_widget,  &SerialBox5_fix_baudrate::output,
            this,                   &MainBox::read_data);
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.data());
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
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

        connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
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
bool MainBox::test(void)
{
#ifdef QT_DEBUG
    qDebug() << "Test";
#endif
    return true;
}
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
bool MainBox::d_test(void)
{
    return test();
}
#endif
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    if(ui->serial_widget->isOpen() ||
        ui->serial_lite_widget->isOpen() ||
        ui->serial_fix_widget->isOpen())
    {
        messagebox_warning("Предупреждение!", "Выключите порты перед выходом.");
        return false;
    }
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
