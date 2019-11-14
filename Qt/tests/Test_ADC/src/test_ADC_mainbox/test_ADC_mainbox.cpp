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

    //createTestBar();

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

    curve_A0 = ui->grapher_widget->add_curve("A0");
    curve_A1 = ui->grapher_widget->add_curve("A1");
    curve_A2 = ui->grapher_widget->add_curve("A2");
    curve_A3 = ui->grapher_widget->add_curve("A3");
    curve_A4 = ui->grapher_widget->add_curve("A4");
    curve_A5 = ui->grapher_widget->add_curve("A5");

    QList<QLCDNumber *> allobj = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *obj, allobj)
    {
        obj->setFixedSize(220, 48);
        obj->setDigitCount(6);
    }
#else
    ui->grapher_widget->setVisible(false);
#endif

    ui->serial_widget->set_fix_baudrate(57600);

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_ADC(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test,   SIGNAL(clicked()),  this,   SLOT(test()));
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
    QString temp = ba.data();
    if(temp.isEmpty())
    {
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
    if(sl.count() != 6)
    {
        emit error(QString("Bad cnt %1").arg(sl.count()));
        emit debug(QString("data_str [%1]").arg(data_str));
        return;
    }
    int A0 = sl.at(0).toInt();
    int A1 = sl.at(1).toInt();
    int A2 = sl.at(2).toInt();
    int A3 = sl.at(3).toInt();
    int A4 = sl.at(4).toInt();
    int A5 = sl.at(5).toInt();

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_A0,    convert_adc(A0));
    ui->grapher_widget->add_curve_data(curve_A1,    convert_adc(A1));
    ui->grapher_widget->add_curve_data(curve_A2,    convert_adc(A2));
    ui->grapher_widget->add_curve_data(curve_A3,    convert_adc(A3));
    ui->grapher_widget->add_curve_data(curve_A4,    convert_adc(A4));
    ui->grapher_widget->add_curve_data(curve_A5,    convert_adc(A5));
#endif

    ui->display_A0->display(convert(convert_adc(A0)));
    ui->display_A1->display(convert(convert_adc(A1)));
    ui->display_A2->display(convert(convert_adc(A2)));
    ui->display_A3->display(convert(convert_adc(A3)));
    ui->display_A4->display(convert(convert_adc(A4)));
    ui->display_A5->display(convert(convert_adc(A5)));

    //emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
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
