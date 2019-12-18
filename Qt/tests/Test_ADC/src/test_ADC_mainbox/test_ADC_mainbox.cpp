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

#ifdef QT_DEBUG
    createTestBar();
#endif

#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_title("Sensors");
    ui->grapher_widget->set_title_axis_X("time");
    ui->grapher_widget->set_title_axis_Y("value");
    ui->grapher_widget->set_axis_scale_x(0, 200);
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

    ui->lcd_layout->setMargin(0);
    ui->lcd_layout->setSpacing(0);

    clr_curves();
    for(int n=0; n<6; n++)
    {
        QString curve_name = QString("A%1").arg(n);
        add_curve(curve_name);
    }

    ui->serial_widget->set_fix_baudrate(57600);

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_ADC(QByteArray)));
    //---
    layout()->setMargin(0);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

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
qreal MainBox::convert_adc(QVariant value)
{
    // 5V
    // 10 bit

    // 1024 - 5V
    // x    - y

    qreal res = -1;

    bool ok = false;

    int i_value = value.toInt(&ok);
    if(ok) res = i_value;

    double d_value = value.toDouble(&ok);
    if(ok) res = d_value;

    float f_value = value.toFloat(&ok);
    if(ok) res = static_cast<qreal>(f_value);

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
    foreach(QString curve_name, sl)
    {
        add_curve(curve_name);
    }
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
    if(curves.length() != max_index)
    {
        emit error("curves.length() != max_index");
        return;
    }
    for(int index=0; index<max_index; index++)
    {
        QVariant value = convert_string(sl.at(index));
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
void MainBox::add_curve(QString curve_name)
{
    // emit trace(Q_FUNC_INFO);

    ADC_label *adc_label = new ADC_label(curve_name, this);
    CURVES cur;

    cur.name = curve_name;
    cur.obj = adc_label;
#ifndef NO_GRAPHER
    cur.curve_index = ui->grapher_widget->add_curve(curve_name);
#else
    cur.curve_index = 0;
#endif

    curves.append(cur);
    ui->lcd_layout->addWidget(adc_label);
}
//--------------------------------------------------------------------------------
void MainBox::clr_curves(void)
{
    // emit trace(Q_FUNC_INFO);
    ui->grapher_widget->remove_all_curve();
    for(int n=0; n<curves.count(); n++)
    {
        curves[n].obj->deleteLater();
    }
    curves.clear();
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);

    ui->grapher_widget->test();
    //clr_curves();
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);

    ui->grapher_widget->test2();
    //add_curve("ADC1");
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
