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
#include "ui_test_HTU21D_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_HTU21D_mainbox.hpp"
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

    ui->grapher_widget->set_title("HTU21D");
    ui->grapher_widget->set_title_axis_X("time");
    ui->grapher_widget->set_title_axis_Y("value");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 100);

    ui->grapher_widget->set_visible_btn_Options(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);

    ui->grapher_widget->push_btn_Vertical(true);
    ui->grapher_widget->push_btn_Horizontal(true);

    curve_temperature   = ui->grapher_widget->add_curve("Temperature");
    curve_humidity      = ui->grapher_widget->add_curve("Humidity");
    curve_compensatedHumidity   = ui->grapher_widget->add_curve("CompensatedHumidity");

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

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_htu21d(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

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
void MainBox::data_htu21d(QByteArray ba)
{
    QString temp = ba;
    if(temp.isEmpty())
    {
        return;
    }
    QStringList sl = temp.split("|");
    if(sl.count() != 3)
    {
        return;
    }

    qreal temperature = sl.at(0).toDouble();
    qreal humidity = sl.at(1).toDouble();
    qreal compensatedHumidity = sl.at(2).toDouble();

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_temperature,           static_cast<double>(temperature));
    ui->grapher_widget->add_curve_data(curve_humidity,              static_cast<double>(humidity));
    ui->grapher_widget->add_curve_data(curve_compensatedHumidity,   static_cast<double>(compensatedHumidity));
#endif

    ui->display_temperature->display(convert(temperature));
    ui->display_humidity->display(convert(humidity));
    ui->display_compensatedHumidity->display(convert(compensatedHumidity));

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
