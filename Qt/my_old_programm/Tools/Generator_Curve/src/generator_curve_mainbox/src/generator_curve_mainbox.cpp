/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include "ui_generator_curve_mainbox.h"
//--------------------------------------------------------------------------------
#include "generator_curve.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "generator_curve_mainbox.hpp"
#include "defines.hpp"
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
    save_widgets();

#ifdef QT_DEBUG
    qDebug() << "~MainBox()";
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    ui->rs232_widget->set_generator(ui->generator_widget);
    ui->sound_widget->set_generator(ui->generator_widget);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test 0", &MainBox::test_0 });
        commands.append({ id++, "test 1", &MainBox::test_1 });
        commands.append({ id++, "test 2", &MainBox::test_2 });
        commands.append({ id++, "test 3", &MainBox::test_3 });
        commands.append({ id++, "test 4", &MainBox::test_4 });
        commands.append({ id++, "test 5", &MainBox::test_5 });
        commands.append({ id++, "test 6", nullptr });

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
        typedef void (MainBox::*function)(void);
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
void MainBox::test_0(void)
{
    emit info("Test_0()");

    QByteArray ba = ui->generator_widget->get_data();
    emit info(QString("size %1").arg(ba.count()));
    if(ba.count() != MAX_SLIDER * 2)
    {
        emit error("bad size");
    }

    //---
    struct TEMP {
        uint16_t data[MAX_SLIDER];
    };

    TEMP* temp = reinterpret_cast<TEMP *>(ba.data());
    for(int n=0; n<MAX_SLIDER; n++)
    {
        emit info(QString("%1").arg(temp->data[n]));
    }
    //---
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
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
