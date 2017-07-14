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
#include "histogram_energy_options.hpp"
#include "ui_histogram_energy_options.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Histogram_energy_options::Histogram_energy_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Histogram_energy_options)
{
    init();
}
//--------------------------------------------------------------------------------
Histogram_energy_options::~Histogram_energy_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Histogram_energy_options::init(void)
{
    ui->setupUi(this);

    //---
    QVariant sb_e_min = 0;
    QVariant sb_e_max = 0;
    QVariant sb_N = 0;
    QVariant clr_histogramm = 0;
    QVariant f_value = 00;
    QVariant flag_e_min = 0;
    QVariant flag_e_max = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_e_min,         &sb_e_min);
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_e_max,         &sb_e_max);
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_N,             &sb_N);
    o->get_histogram_energy_option(PARAM_histogram_energy_flag_e_min,       &flag_e_min);
    o->get_histogram_energy_option(PARAM_histogram_energy_flag_e_max,       &flag_e_max);
    o->get_histogram_energy_option(PARAM_histogram_energy_color_histogramm, &clr_histogramm);
    o->get_histogram_energy_option(PARAM_histogram_energy_events_flags,     &f_value);

    o->deleteLater();
    //---
    ui->sb_e_min->setRange(HISTOGRAM_MIN_ENERGY, HISTOGRAM_MAX_ENERGY);
    ui->sb_e_min->setValue(sb_e_min.toInt());
    //---
    ui->sb_e_max->setRange(HISTOGRAM_MIN_ENERGY+1, HISTOGRAM_MAX_ENERGY);
    ui->sb_e_max->setValue(sb_e_max.toInt());
    //---
    QColor color_clr_histogramm = QColor(clr_histogramm.toInt());
    ui->btn_histogramm->setStyleSheet(QString("background:#%1%2%3")
                                      .arg(color_clr_histogramm.red(),      2, 16,QChar('0'))
                                      .arg(color_clr_histogramm.green(),    2, 16,QChar('0'))
                                      .arg(color_clr_histogramm.blue(),     2, 16,QChar('0')));
    //---
    ui->sb_N->setRange(HISTOGRAM_MIN_N, HISTOGRAM_MAX_N);
    ui->sb_N->setValue(sb_N.toInt());
    //---
    ui->cb_enable_filter_min_energy->setChecked(flag_e_min.toBool());
    ui->cb_enable_filter_max_energy->setChecked(flag_e_max.toBool());
    //---
    uint8_t value = f_value.toInt();

    ui->cb_PET_EVENT_FLAG_FAULT->setChecked(value & PET_EVENT_FLAG_FAULT);
    ui->cb_PET_EVENT_FLAG_TS_INVALID->setChecked(value & PET_EVENT_FLAG_TS_INVALID);
    ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->setChecked(value & PET_EVENT_FLAG_GATE_OVERLAY);
    ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_QDC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_OVERFLOW);
    ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->setChecked(value & PET_EVENT_FLAG_ADC_UNDERFLOW);
    ui->cb_PET_EVENT_FLAG_SYNC->setChecked(value & PET_EVENT_FLAG_SYNC);
    //---

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));

    connect(ui->btn_histogramm, SIGNAL(clicked(bool)),  this,   SLOT(color_histogramm()));

    QColor color_histogramm = QColor(clr_histogramm.toInt());
    ui->btn_histogramm->setStyleSheet(QString("background:#%1%2%3")
                                      .arg(color_histogramm.red(),      2, 16,QChar('0'))
                                      .arg(color_histogramm.green(),    2, 16,QChar('0'))
                                      .arg(color_histogramm.blue(),     2, 16,QChar('0')));
}
//--------------------------------------------------------------------------------
void Histogram_energy_options::t_accept(void)
{
    block_this_button(true);
    //---
    uint8_t value = 0;

    if(ui->cb_PET_EVENT_FLAG_FAULT->isChecked())            value |= PET_EVENT_FLAG_FAULT;
    if(ui->cb_PET_EVENT_FLAG_TS_INVALID->isChecked())       value |= PET_EVENT_FLAG_TS_INVALID;
    if(ui->cb_PET_EVENT_FLAG_GATE_OVERLAY->isChecked())     value |= PET_EVENT_FLAG_GATE_OVERLAY;
    if(ui->cb_PET_EVENT_FLAG_QDC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_QDC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_QDC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_QDC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_OVERFLOW->isChecked())     value |= PET_EVENT_FLAG_ADC_OVERFLOW;
    if(ui->cb_PET_EVENT_FLAG_ADC_UNDERFLOW->isChecked())    value |= PET_EVENT_FLAG_ADC_UNDERFLOW;
    if(ui->cb_PET_EVENT_FLAG_SYNC->isChecked())             value |= PET_EVENT_FLAG_SYNC;
    //---

    emit info(QString("value %1").arg(value));

    PETCat_options *o = new PETCat_options();
    o->set_histogram_energy_option(PARAM_histogram_energy_sb_e_min,         ui->sb_e_min->value());
    o->set_histogram_energy_option(PARAM_histogram_energy_sb_e_max,         ui->sb_e_max->value());
    o->set_histogram_energy_option(PARAM_histogram_energy_sb_N,             ui->sb_N->value());
    o->set_histogram_energy_option(PARAM_histogram_energy_color_histogramm, ui->btn_histogramm->palette().background().color().rgb());
    o->set_histogram_energy_option(PARAM_histogram_energy_flag_e_min,       ui->cb_enable_filter_min_energy->isChecked());
    o->set_histogram_energy_option(PARAM_histogram_energy_flag_e_max,       ui->cb_enable_filter_max_energy->isChecked());
    o->set_histogram_energy_option(PARAM_histogram_energy_events_flags,     value);
    o->deleteLater();

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Histogram_energy_options::color_histogramm(void)
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
void Histogram_energy_options::block_wheel(void)
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
bool Histogram_energy_options::eventFilter(QObject*, QEvent* event)
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
