/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QColorDialog>
#include <QMouseEvent>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "oscilloscope_options.hpp"
#include "ui_oscilloscope_options.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
Oscilloscope_options::Oscilloscope_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Oscilloscope_options)
{
    init();
}
//--------------------------------------------------------------------------------
Oscilloscope_options::~Oscilloscope_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::init(void)
{
    ui->setupUi(this);

    //---
    QVariant cb_sync = 0;
    QVariant cb_auto = 0;
    QVariant cb_type_curve = 0;
    QVariant cb_channel = 0;
    QVariant sb_time = 0;
    QVariant sb_vert_scale_min = 0;
    QVariant sb_vert_scale_max = 0;

    QVariant clr_x_plus = 0;
    QVariant clr_x_minus = 0;
    QVariant clr_y_plus = 0;
    QVariant clr_y_minus = 0;
    QVariant clr_sum = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_sync,              &cb_sync);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_auto,              &cb_auto);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_type_curve,        &cb_type_curve);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_channel,           &cb_channel);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_time,              &sb_time);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_min,    &sb_vert_scale_min);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_max,    &sb_vert_scale_max);

    o->get_oscilloscope_option(PARAM_oscilloscope_clr_x_plus,   &clr_x_plus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_x_minus,  &clr_x_minus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_y_plus,   &clr_y_plus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_y_minus,  &clr_y_minus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_sum,      &clr_sum);
    o->deleteLater();
    //---
    ui->cb_sync->clear();
    ui->cb_sync->addItem("Непрерывно",      PET_WAVE_MODE_SYNC_NONE);
    ui->cb_sync->addItem("По фотоимпульсу", PET_WAVE_MODE_SYNC_PULSE);

    ui->cb_sync->setCurrentIndex(cb_sync.toInt());
    //---
    ui->cb_auto->clear();
    ui->cb_auto->addItem("Ручной",          PET_WAVE_MODE_SYNC_NONE);
    ui->cb_auto->addItem("Автоматический",  PET_WAVE_MODE_SYNC_PULSE);

    ui->cb_auto->setCurrentIndex(cb_auto.toInt());
    //---
    ui->cb_type_curve->clear();
    ui->cb_type_curve->addItem("Точечный", DOTS);
    ui->cb_type_curve->addItem("Линейный", LINES);
    ui->cb_type_curve->addItem("Сглаженная кривая", SPLINE_LINES);

    ui->cb_type_curve->setCurrentIndex(cb_type_curve.toInt());
    //---
    ui->cb_channel->clear();
    ui->cb_channel->addItem("1", 0);
    ui->cb_channel->addItem("2", 1);
    ui->cb_channel->addItem("3", 2);
    ui->cb_channel->addItem("4", 3);

    ui->cb_channel->setCurrentIndex(cb_channel.toInt());
    //---
    ui->sb_time->setRange(SB_TIME_MIN, SB_TIME_MAX);
    ui->sb_time->setSingleStep(SB_TIME_STEP);

    ui->sb_time->setValue(sb_time.toInt());
    //---
    ui->sb_vert_scale_min->setRange(-(4 * 0xFFFF), 0);

    ui->sb_vert_scale_min->setValue(sb_vert_scale_min.toInt());

    ui->sb_vert_scale_max->setRange(0, (4 * 0xFFFF));

    ui->sb_vert_scale_max->setValue(sb_vert_scale_max.toInt());
    //---

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));

    connect(ui->btn_x_plus,     SIGNAL(clicked(bool)),  this,   SLOT(color_X_plus()));
    connect(ui->btn_x_minus,    SIGNAL(clicked(bool)),  this,   SLOT(color_X_minus()));
    connect(ui->btn_y_plus,     SIGNAL(clicked(bool)),  this,   SLOT(color_Y_plus()));
    connect(ui->btn_y_minus,    SIGNAL(clicked(bool)),  this,   SLOT(color_Y_minus()));
    connect(ui->btn_sum,        SIGNAL(clicked(bool)),  this,   SLOT(color_SUM()));

    QColor color_x_plus = QColor(clr_x_plus.toInt());
    QColor color_x_minus = QColor(clr_x_minus.toInt());
    QColor color_y_plus = QColor(clr_y_plus.toInt());
    QColor color_y_minus = QColor(clr_y_minus.toInt());
    QColor color_sum = QColor(clr_sum.toInt());

    ui->btn_x_plus->setStyleSheet(QString("background:#%1%2%3")
                                  .arg(color_x_plus.red(),      2, 16,QChar('0'))
                                  .arg(color_x_plus.green(),    2, 16,QChar('0'))
                                  .arg(color_x_plus.blue(),     2, 16,QChar('0')));
    ui->btn_x_minus->setStyleSheet(QString("background:#%1%2%3")
                                   .arg(color_x_minus.red(),      2, 16,QChar('0'))
                                   .arg(color_x_minus.green(),    2, 16,QChar('0'))
                                   .arg(color_x_minus.blue(),     2, 16,QChar('0')));
    ui->btn_y_plus->setStyleSheet(QString("background:#%1%2%3")
                                  .arg(color_y_plus.red(),      2, 16,QChar('0'))
                                  .arg(color_y_plus.green(),    2, 16,QChar('0'))
                                  .arg(color_y_plus.blue(),     2, 16,QChar('0')));
    ui->btn_y_minus->setStyleSheet(QString("background:#%1%2%3")
                                   .arg(color_y_minus.red(),      2, 16,QChar('0'))
                                   .arg(color_y_minus.green(),    2, 16,QChar('0'))
                                   .arg(color_y_minus.blue(),     2, 16,QChar('0')));
    ui->btn_sum->setStyleSheet(QString("background:#%1%2%3")
                               .arg(color_sum.red(),      2, 16,QChar('0'))
                               .arg(color_sum.green(),    2, 16,QChar('0'))
                               .arg(color_sum.blue(),     2, 16,QChar('0')));

}
//--------------------------------------------------------------------------------
void Oscilloscope_options::t_accept(void)
{
    block_this_button(true);

    PETCat_options *o = new PETCat_options();
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_sync,              ui->cb_sync->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_auto,              ui->cb_auto->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_type_curve,        ui->cb_type_curve->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_channel,           ui->cb_channel->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_time,              ui->sb_time->value());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_min,    ui->sb_vert_scale_min->value());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_max,    ui->sb_vert_scale_max->value());

    o->set_oscilloscope_option(PARAM_oscilloscope_clr_x_plus,   ui->btn_x_plus->palette().background().color().rgb());
    o->set_oscilloscope_option(PARAM_oscilloscope_clr_x_minus,  ui->btn_x_minus->palette().background().color().rgb());
    o->set_oscilloscope_option(PARAM_oscilloscope_clr_y_plus,   ui->btn_y_plus->palette().background().color().rgb());
    o->set_oscilloscope_option(PARAM_oscilloscope_clr_y_minus,  ui->btn_y_minus->palette().background().color().rgb());
    o->set_oscilloscope_option(PARAM_oscilloscope_clr_sum,      ui->btn_sum->palette().background().color().rgb());
    o->deleteLater();

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::color_X_plus(void)
{
    QToolButton *tb = dynamic_cast<QToolButton *>(sender());
    if(tb == nullptr)
    {
        emit error("Histogram_consulience_options: error sender");
        return;
    }

    QColorDialog *dlg = new QColorDialog;
    dlg->setCurrentColor(tb->palette().background().color());
    int btn = dlg->exec();
    dlg->deleteLater();

    if(btn == QColorDialog::Accepted)
    {
        QColor new_color = dlg->currentColor();
        tb->setStyleSheet(QString("background:#%1%2%3")
                          .arg(new_color.red(),      2, 16,QChar('0'))
                          .arg(new_color.green(),    2, 16,QChar('0'))
                          .arg(new_color.blue(),     2, 16,QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::color_X_minus(void)
{
    QToolButton *tb = dynamic_cast<QToolButton *>(sender());
    if(tb == nullptr)
    {
        emit error("Histogram_consulience_options: error sender");
        return;
    }

    QColorDialog *dlg = new QColorDialog;
    dlg->setCurrentColor(tb->palette().background().color());
    int btn = dlg->exec();
    dlg->deleteLater();

    if(btn == QColorDialog::Accepted)
    {
        QColor new_color = dlg->currentColor();
        tb->setStyleSheet(QString("background:#%1%2%3")
                          .arg(new_color.red(),      2, 16,QChar('0'))
                          .arg(new_color.green(),    2, 16,QChar('0'))
                          .arg(new_color.blue(),     2, 16,QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::color_Y_plus(void)
{
    QToolButton *tb = dynamic_cast<QToolButton *>(sender());
    if(tb == nullptr)
    {
        emit error("Histogram_consulience_options: error sender");
        return;
    }

    QColorDialog *dlg = new QColorDialog;
    dlg->setCurrentColor(tb->palette().background().color());
    int btn = dlg->exec();
    dlg->deleteLater();

    if(btn == QColorDialog::Accepted)
    {
        QColor new_color = dlg->currentColor();
        tb->setStyleSheet(QString("background:#%1%2%3")
                          .arg(new_color.red(),      2, 16,QChar('0'))
                          .arg(new_color.green(),    2, 16,QChar('0'))
                          .arg(new_color.blue(),     2, 16,QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::color_Y_minus(void)
{
    QToolButton *tb = dynamic_cast<QToolButton *>(sender());
    if(tb == nullptr)
    {
        emit error("Histogram_consulience_options: error sender");
        return;
    }

    QColorDialog *dlg = new QColorDialog;
    dlg->setCurrentColor(tb->palette().background().color());
    int btn = dlg->exec();
    dlg->deleteLater();

    if(btn == QColorDialog::Accepted)
    {
        QColor new_color = dlg->currentColor();
        tb->setStyleSheet(QString("background:#%1%2%3")
                          .arg(new_color.red(),      2, 16,QChar('0'))
                          .arg(new_color.green(),    2, 16,QChar('0'))
                          .arg(new_color.blue(),     2, 16,QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::color_SUM(void)
{
    QToolButton *tb = dynamic_cast<QToolButton *>(sender());
    if(tb == nullptr)
    {
        emit error("Histogram_consulience_options: error sender");
        return;
    }

    QColorDialog *dlg = new QColorDialog;
    dlg->setCurrentColor(tb->palette().background().color());
    int btn = dlg->exec();
    dlg->deleteLater();

    if(btn == QColorDialog::Accepted)
    {
        QColor new_color = dlg->currentColor();
        tb->setStyleSheet(QString("background:#%1%2%3")
                          .arg(new_color.red(),      2, 16,QChar('0'))
                          .arg(new_color.green(),    2, 16,QChar('0'))
                          .arg(new_color.blue(),     2, 16,QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_options::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
bool Oscilloscope_options::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
