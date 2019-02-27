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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_triangle_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_triangle_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
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
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *testbar = new QToolBar("testbar", this);
    testbar->setObjectName("testbar");

    testbar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
bool MainBox::check_triangle(double A, double B, double C)
{
    if((A+B)<C)
    {
        //emit error(QString("A=%1 B=%2 C=%3").arg(A).arg(B).arg(C));
        return false;
    }

    if((B+C)<A)
    {
        //emit error(QString("A=%1 B=%2 C=%3").arg(A).arg(B).arg(C));
        return false;
    }

    if((A+C)<B)
    {
        //emit error(QString("A=%1 B=%2 C=%3").arg(A).arg(B).arg(C));
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_angle(const QString &angle_name, double angle_grad)
{
    if(angle_grad < 0.0f)
    {
        //emit error(QString("%1 %2 too small").arg(angle_name).arg(angle_grad));
        return false;
    }
    if(angle_grad >= 90.0f)
    {
        //emit error(QString("%1 %2 too large").arg(angle_name).arg(angle_grad));
        return false;
    }
    Q_UNUSED(angle_name);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_trianle_120(double A, double B, double C)
{
    double angle_A, angle_B, angle_C, delta;

    angle_A = rad_to_grad(qAcos((A*A + B*B - C*C) / (2*A*B)));
    angle_B = rad_to_grad(qAcos((B*B + C*C - A*A) / (2*B*C)));
    angle_C = rad_to_grad(qAcos((A*A + C*C - B*B) / (2*A*C)));
    //emit debug(QString("check_trianle_120: angle_A=%1 angle_B=%2 angle_C=%3").arg(angle_A).arg(angle_B).arg(angle_C));

    delta = 0.01f;
    if(qAbs(angle_A - 120.0f) < delta) return true;
    if(qAbs(angle_B - 120.0f) < delta) return true;
    if(qAbs(angle_C - 120.0f) < delta) return true;
    return false;
}
//--------------------------------------------------------------------------------
double MainBox::grad_to_rad(double angle_grad)
{
    return (angle_grad *(double)M_PI)/(double)180.0f;
}
//--------------------------------------------------------------------------------
double MainBox::rad_to_grad(double angle_rad)
{
    return (angle_rad * (double)180.0f)/(double)M_PI;
}
//--------------------------------------------------------------------------------
void MainBox::test_triangle(double a, double b, double c)
{
    double A, B, C;
    double x;
    bool temp;

    A = qSqrt(a*a + b*b + a*b);
    B = qSqrt(a*a + c*c + a*c);
    C = qSqrt(b*b + c*c + b*c);
    emit debug(QString("A=%1 B=%2 C=%3").arg(A).arg(B).arg(C));
    for(x=0.0f; x<90.0f; x+=0.00001)
    {
        temp = calc(A, B, C, x);
        if(temp)
        {
            emit info(QString("found! x = %1").arg(x));
            return;
        }
    }
    emit error("not found!");
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{    
    test_triangle(220.0f, 220.0f, 220.0f);
    //emit debug(QString("%1").arg(calc(381.051f, 381.051f, 381.051f, 30.0f) ? "true" : "false"));
}
//--------------------------------------------------------------------------------
bool MainBox::calc(double A, double B, double C, double angle_grad)
{
    double a, b, c;
    double angle_A, angle_B, angle_C;
    double angle1_b, angle1_a;
    double angle2_b, angle2_c;
    double angle3_a, angle3_c;
    double temp;
    double temp_value;
    double delta;

    delta = 0.0001f;
    if(!check_triangle(A, B, C)) return false;

    angle_A = rad_to_grad(qAcos((A*A + B*B - C*C) / (2*A*B)));
    angle_B = rad_to_grad(qAcos((B*B + C*C - A*A) / (2*B*C)));
    angle_C = rad_to_grad(qAcos((A*A + C*C - B*B) / (2*A*C)));
    //emit debug(QString("angle_A=%1 angle_B=%2 angle_C=%3").arg(angle_A).arg(angle_B).arg(angle_C));
    if(!check_angle("angle_A", angle_A)) return false;
    if(!check_angle("angle_B", angle_B)) return false;
    if(!check_angle("angle_C", angle_C)) return false;

    angle1_b = angle_grad;
    angle3_c = angle_B - angle1_b;
    angle3_a = 60.0f - angle3_c;
    angle2_b = angle_A - angle3_a;
    angle2_c = 60.0f - angle2_b;
    angle1_a = angle_C - angle2_c;

    if(!check_angle("angle1_a", angle1_a)) return false;
    if(!check_angle("angle1_b", angle1_b)) return false;
    if(!check_angle("angle2_b", angle2_b)) return false;
    if(!check_angle("angle2_c", angle2_c)) return false;
    if(!check_angle("angle3_a", angle3_a)) return false;
    if(!check_angle("angle3_c", angle3_c)) return false;

    // calc triangle 1
    temp = A / qSin(grad_to_rad(angle_A));
    a = temp * qSin(grad_to_rad(angle1_a));
    b = temp * qSin(grad_to_rad(angle1_b));
    if(!check_triangle(a, b, A)) return false;
    temp_value = b;

    // calc triangle 2
    temp = B / qSin(grad_to_rad(angle_B));
    b = temp * qSin(grad_to_rad(angle2_b));
    c = temp * qSin(grad_to_rad(angle2_c));
    if(qAbs(b - temp_value) > delta) return false;
    if(!check_triangle(b, c, B)) return false;
    temp_value = c;

    // calc triangle 3
    temp = C / qSin(grad_to_rad(angle_C));
    a = temp * qSin(grad_to_rad(angle3_a));
    c = temp * qSin(grad_to_rad(angle3_c));
    if(qAbs(c - temp_value) > delta) return false;
    if(!check_triangle(a, c, C)) return false;

    //emit debug(QString("angle1_a=%1 angle1_b=%2 angle_A=%3").arg(angle1_a).arg(angle1_b).arg(120.0f));
    //emit debug(QString("angle2_b=%1 angle2_c=%2 angle_B=%3").arg(angle2_b).arg(angle2_c).arg(120.0f));
    //emit debug(QString("angle3_a=%1 angle3_c=%2 angle_C=%3").arg(angle3_a).arg(angle3_c).arg(120.0f));

    emit info(QString("a = %1").arg(a));
    emit info(QString("b = %1").arg(b));
    emit info(QString("c = %1").arg(c));

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
