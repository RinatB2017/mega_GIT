/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QPushButton>
#include <QToolButton>
//--------------------------------------------------------------------------------
#include "testbox_2.hpp"
#include "ui_testbox_2.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
TestBox_2::TestBox_2(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TestBox_2)
{
    init();
    load_widgets("TestBox_2");    //TODO
}
//--------------------------------------------------------------------------------
TestBox_2::~TestBox_2()
{
    save_widgets("TestBox_2");    //TODO

    delete ui;
}
//--------------------------------------------------------------------------------
void TestBox_2::init(void)
{
    ui->setupUi(this);

    ui->sb_N->setRange(1, 100);
    ui->sb_N->setValue(10);

    curve_data = ui->grapher_widget->add_curve("data");
    curve_ideal = ui->grapher_widget->add_curve("ideal");
    curve_A_max = ui->grapher_widget->add_curve("A max");
    curve_A_min = ui->grapher_widget->add_curve("A min");

    ui->grapher_widget->set_silense(true);

    ui->grapher_widget->set_visible_btn_all_OFF(false);
    ui->grapher_widget->set_visible_btn_all_ON(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);
    ui->grapher_widget->set_visible_btn_Options(false);

    ui->grapher_widget->push_btn_Horizontal(true);
    ui->grapher_widget->push_btn_Vertical(true);

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
// Задание начального набора значений
double **TestBox_2::getData(int n)
{
    double **f;
    f = new double*[2];
    f[0] = new double[n];
    f[1] = new double[n];
    for (int i = 0; i<n; i++)
    {
        f[0][i] = (double)i;
        f[1][i] = 8 * (double)i - 3;
        // Добавление случайной составляющей
        f[1][i] = 8*(double)i - 3 + ((rand()%100)-50)*0.2;
    }
    return f;
}
//--------------------------------------------------------------------------------
// Вычисление коэффициентов аппроксимирующей прямой
void TestBox_2::getApprox(double **x, double *a, double *b, int n)
{
    double sumx = 0;
    double sumy = 0;
    double sumx2 = 0;
    double sumxy = 0;
    for (int i = 0; i<n; i++)
    {
        sumx += x[0][i];
        sumy += x[1][i];
        sumx2 += x[0][i] * x[0][i];
        sumxy += x[0][i] * x[1][i];
    }
    *a = (n*sumxy - (sumx*sumy)) / (n*sumx2 - sumx*sumx);
    *b = (sumy - *a*sumx) / n;
    return;
}
//--------------------------------------------------------------------------------
void TestBox_2::test(void)
{
    double **x, k, b;
    int n = ui->sb_N->value();

    ui->grapher_widget->clear();

    x = getData(n);
    for (int i = 0; i<n; i++)
    {
        ui->grapher_widget->add_curve_data(curve_data, x[0][i], x[1][i]);
    }
    getApprox(x, &k, &b, n);

    //---
    for(int i=0; i<n; i++)
    {
        ui->grapher_widget->add_curve_data(curve_ideal, i, k*i + b);
        ui->grapher_widget->add_curve_data(curve_A_max, i, k*i + b + ui->dsb_A_max->value());
        ui->grapher_widget->add_curve_data(curve_A_min, i, k*i + b - ui->dsb_A_max->value());
    }
    //---

    emit info(QString("k = %1").arg(k));
    emit info(QString("b = %1").arg(b));
}
//--------------------------------------------------------------------------------
