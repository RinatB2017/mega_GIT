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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
#include "adc_label.hpp"
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
#   include "grapherbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_title("ADC");
    ui->grapher_widget->set_title_axis_X("time");
    ui->grapher_widget->set_title_axis_Y("voltage");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 5);

    ui->grapher_widget->set_visible_btn_Options(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);

    ui->grapher_widget->push_btn_Horizontal(true);
    ui->grapher_widget->push_btn_Vertical(true);
#else
    ui->grapher_widget->setVisible(false);
#endif

    curves.clear();
    //---
    //TODO криво, надо переделать
    ui->lcd_layout->setMargin(0);
    ui->lcd_layout->setSpacing(0);
    curves.clear();
    CURVES cur;
    for(int n=0; n<6; n++)
    {
        QString curve_name = QString("A%1:").arg(n);
        ADC_label *adc_label = new ADC_label(curve_name, this);

        cur.name = QString("A%1:").arg(n);
        cur.obj = adc_label;
#ifndef NO_GRAPHER
        cur.curve_index = ui->grapher_widget->add_curve(curve_name);
#else
        cur.curve_index = 0;
#endif

        curves.append(cur);
        ui->lcd_layout->addWidget(curves.at(n).obj);
    }
    //---

    ui->serial_widget->set_fix_baudrate(57600);

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_ADC(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "create curves", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "resize curves", &MainBox::test_1 });

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
QString MainBox::convert(qreal value)
{
    return QString("%1").arg(value, 0, 'f', 2);
}
//--------------------------------------------------------------------------------
qreal MainBox::convert_adc(int value)
{
    // 5V
    // 10 bit

    // 1024 - 5V
    // x    - y

    qreal res = static_cast<qreal>(value) * 5.0 / 1024.0;
    return res;
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
                show_data_ADC(data_str.split("|"));
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
void MainBox::show_data_ADC(QStringList sl)
{
    if(sl.count() != curves.count())
    {
        emit error(QString("Bad cnt %1").arg(sl.count()));
        emit error(QString("data_str [%1]").arg(data_str));
        return;
    }
    int max_index = sl.count();
    for(int index=0; index<max_index; index++)
    {
        int value = sl.at(index).toInt();
#ifndef NO_GRAPHER
        ui->grapher_widget->add_curve_data(curves.at(index).curve_index,    convert_adc(value));
#endif
        curves.at(index).obj->display(convert(convert_adc(value)));
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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (MainBox::*my_mega_function)(void);
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
void MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);

    ui->grapher_widget->remove_all_curve();
    for(int n=0; n<curves.count(); n++)
    {
        curves[n].obj->deleteLater();
    }
    curves.clear();

    CURVES cur;
    for(int n=0; n<5; n++)
    {
        QString curve_name = QString("A%1:").arg(n);
        ADC_label *adc_label = new ADC_label(curve_name, this);

        cur.name = QString("A%1:").arg(n);
        cur.obj = adc_label;
#ifndef NO_GRAPHER
        cur.curve_index = ui->grapher_widget->add_curve(curve_name);
#else
        cur.curve_index = 0;
#endif

        curves.append(cur);
        ui->lcd_layout->addWidget(curves.at(n).obj);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
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
