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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_ADC_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_ADC_mainbox.hpp"
#include "datagrapherbox.hpp"
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
    createTestBar();
#endif

    clr_curves();
    for(int n=0; n<6; n++)
    {
        QString curve_name = QString("A%1").arg(n);
        ui->data_widget->add_curve(curve_name);
    }

    ui->serial_widget->set_fix_baudrate(57600);
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_ADC(QByteArray)));

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("ADC",
                            "data_widget",
                            Qt::LeftDockWidgetArea,
                            reinterpret_cast<QWidget *>(ui->main_frame));
        setVisible(false);
    }
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::clr_curves(void)
{
    ui->data_widget->clr_curves();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::data_ADC(const QByteArray &ba)
{
    // emit trace(Q_FUNC_INFO);

    QString temp = ba.data();
    if(temp.isEmpty())
    {
        emit trace("temp.isEmpty");
        return;
    }

    // emit trace(ba);
    for(int n=0; n<ba.length(); n++)
    {
        char s = ba.at(n);
        switch(s)
        {
        case ':':
            data_str.clear();
            flag_good_data = true;
            break;

        case '\r':
        case '\n':
            if(flag_good_data)
            {
                analize_packet(data_str.split("|"));
                data_str.clear();
                flag_good_data = false;
            }
            break;

        default:
            if(flag_good_data)
            {
                data_str.append(s);
            }
            break;
        }
    }

}
//--------------------------------------------------------------------------------
void MainBox::analize_packet(QStringList sl)
{
    int cnt = sl.count();
    if(cnt < 1)
    {
        emit error("cnt < 1");
        return;
    }
    // emit trace(sl.at(0));
    if(sl.at(0) == "curves")
    {
        sl.removeFirst();
        clr_curves();
        add_curves(sl);
    }
    if(sl.at(0) == "data")
    {
        sl.removeFirst();
        show_data_ADC(sl);
    }
}
//--------------------------------------------------------------------------------
void MainBox::add_curves(QStringList sl)
{
    // emit trace(Q_FUNC_INFO);
    ui->data_widget->add_curves(sl);
}
//--------------------------------------------------------------------------------
QVariant MainBox::convert_string(QString str_value)
{
    bool ok = false;
    QVariant value = -1;

    value = str_value.toInt(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toDouble(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toFloat(&ok);
    if(ok)
    {
        return value;
    }

    emit error("unknown format");
    return value;
}
//--------------------------------------------------------------------------------
void MainBox::show_data_ADC(QStringList sl)
{
    int max_index = sl.count();
    if(ui->data_widget->get_max_index() != max_index)
    {
        emit error("curves.length() != max_index");
        return;
    }
    for(int index=0; index<max_index; index++)
    {
        QVariant value = convert_string(sl.at(index));
        ui->data_widget->add_data(index, value.toDouble());
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
void MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
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
