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
#include "histogram_consulience_options.hpp"
#include "ui_histogram_consulience_options.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
Histogram_consulience_options::Histogram_consulience_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Histogram_consulience_options)
{
    init();
}
//--------------------------------------------------------------------------------
Histogram_consulience_options::~Histogram_consulience_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Histogram_consulience_options::init(void)
{
    ui->setupUi(this);

    //---
    QVariant sb_nudChANumber = 0;
    QVariant sb_nudChBNumber = 0;
    QVariant dsb_nudChACorr = 0;
    QVariant dsb_nudChBCorr = 0;
    QVariant dsb_nudTInt = 0;

    QVariant dsb_nudWindow = 0;
    QVariant dsb_nudN = 0;

    QVariant clr_p = 0;
    QVariant clr_gauss = 0;

    QVariant f_value = 0xFE;
    //---
    PETCat_options *o = new PETCat_options();

    o->get_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChANumber, &sb_nudChANumber);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChBNumber, &sb_nudChBNumber);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChACor,   &dsb_nudChACorr);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChBCor,   &dsb_nudChBCorr);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudTInt,     &dsb_nudTInt);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudWindow,   &dsb_nudWindow);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudN,        &dsb_nudN);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_clr_p,           &clr_p);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_clr_gauss,       &clr_gauss);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_events_flags,    &f_value);

    o->deleteLater();
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
    ui->sb_nudChANumber->setRange(0, 0xFF);
    ui->sb_nudChANumber->setValue(sb_nudChANumber.toInt());

    ui->sb_nudChBNumber->setRange(0, 0xFF);
    ui->sb_nudChBNumber->setValue(sb_nudChBNumber.toInt());

    ui->dsb_nudChACorr->setRange(-100000, 100000);
    ui->dsb_nudChACorr->setValue(dsb_nudChACorr.toDouble());

    ui->dsb_nudChBCorr->setRange(-100000, 100000);
    ui->dsb_nudChACorr->setValue(dsb_nudChBCorr.toDouble());

    ui->dsb_nudTInt->setRange(0, 100);
    ui->dsb_nudTInt->setValue(dsb_nudTInt.toDouble());

    ui->dsb_nudWindow->setRange(0, 100000);
    ui->dsb_nudWindow->setValue(dsb_nudWindow.toDouble());

    ui->dsb_nudN->setRange(0, 100000);
    ui->dsb_nudN->setValue(dsb_nudN.toDouble());

    ui->dsb_nudTInt->setDecimals(1);

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));

    connect(ui->btn_p,      SIGNAL(clicked(bool)),  this,   SLOT(color_p()));
    connect(ui->btn_gauss,  SIGNAL(clicked(bool)),  this,   SLOT(color_gauss()));

    QColor color_p = QColor(clr_p.toInt());
    QColor color_gauss = QColor(clr_gauss.toInt());
    ui->btn_p->setStyleSheet(QString("background:#%1%2%3")
                             .arg(color_p.red(),      2, 16,QChar('0'))
                             .arg(color_p.green(),    2, 16,QChar('0'))
                             .arg(color_p.blue(),     2, 16,QChar('0')));
    ui->btn_gauss->setStyleSheet(QString("background:#%1%2%3")
                                 .arg(color_gauss.red(),      2, 16,QChar('0'))
                                 .arg(color_gauss.green(),    2, 16,QChar('0'))
                                 .arg(color_gauss.blue(),     2, 16,QChar('0')));
}
//--------------------------------------------------------------------------------
void Histogram_consulience_options::t_accept(void)
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

    PETCat_options *o = new PETCat_options();
    o->set_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChANumber, ui->sb_nudChANumber->value());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChBNumber, ui->sb_nudChBNumber->value());

    o->set_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChACor,   ui->dsb_nudChACorr->value());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChBCor,   ui->dsb_nudChBCorr->value());

    o->set_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudTInt,     ui->dsb_nudTInt->value());

    o->set_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudWindow,   ui->dsb_nudWindow->value());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudN,        ui->dsb_nudN->value());

    o->set_histogram_consulience_option(PARAM_histogram_consulience_clr_p,           ui->btn_p->palette().background().color().rgb());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_clr_gauss,       ui->btn_gauss->palette().background().color().rgb());

    o->set_histogram_consulience_option(PARAM_histogram_consulience_events_flags,    value);
    o->deleteLater();

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void Histogram_consulience_options::color_p(void)
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
void Histogram_consulience_options::color_gauss(void)
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
void Histogram_consulience_options::block_wheel(void)
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
bool Histogram_consulience_options::eventFilter(QObject*, QEvent* event)
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
