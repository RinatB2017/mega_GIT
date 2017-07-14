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
#include <QNetworkAddressEntry>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_petcat_optionsbox.h"
//--------------------------------------------------------------------------------
#include "petcat_optionsbox.hpp"
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
PETCat_optionsBox::PETCat_optionsBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PETCat_optionsBox)
{
    init();
}
//--------------------------------------------------------------------------------
PETCat_optionsBox::~PETCat_optionsBox()
{
    if(o_options)       o_options->deleteLater();
    if(o_oscilloscope)  o_oscilloscope->deleteLater();
    if(o_floodmap)      o_floodmap->deleteLater();
    if(o_consulience)   o_consulience->deleteLater();
    if(o_energy)        o_energy->deleteLater();
    if(o_imitator_brg)  o_imitator_brg->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void PETCat_optionsBox::init(void)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(ICON_PROGRAMM));
    setWindowTitle("Настройки программы");

    //---
    ui->tw_main->header()->hide();
    //---
    QTreeWidgetItem *i_main_options = new QTreeWidgetItem(ui->tw_main);
    i_main_options->setText(0, NAME_MAIN_OPTIONS);
    i_main_options->setToolTip(0, NAME_MAIN_OPTIONS);
    i_main_options->setStatusTip(0, NAME_MAIN_OPTIONS);
    i_main_options->setIcon(0, QIcon(NAME_MAIN_OPTIONS_ICON));
    i_main_options->setData(0, Qt::UserRole,    ID_OPTIONS);

    QTreeWidgetItem *i_oscilloscope = new QTreeWidgetItem(ui->tw_main);
    i_oscilloscope->setText(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setToolTip(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setStatusTip(0, NAME_OSCILLOSCOPE);
    i_oscilloscope->setIcon(0, QIcon(NAME_OSCILLOSCOPE_ICON));
    i_oscilloscope->setData(0, Qt::UserRole,    ID_OSCILLOSCOPE);

    QTreeWidgetItem *i_floodmap = new QTreeWidgetItem(ui->tw_main);
    i_floodmap->setText(0, NAME_FLOODMAP);
    i_floodmap->setToolTip(0, NAME_FLOODMAP);
    i_floodmap->setStatusTip(0, NAME_FLOODMAP);
    i_floodmap->setIcon(0, QIcon(NAME_FLOODMAP_ICON));
    i_floodmap->setData(0, Qt::UserRole,    ID_FLOODMAP);

    QTreeWidgetItem *i_histogram_consulience = new QTreeWidgetItem(ui->tw_main);
    i_histogram_consulience->setText(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setToolTip(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setStatusTip(0, NAME_HISTOGRAM_CONSULIENCE);
    i_histogram_consulience->setIcon(0, QIcon(NAME_HISTOGRAM_CONSULIENCE_ICON));
    i_histogram_consulience->setData(0, Qt::UserRole,    ID_HISTOGRAM_CONSULIENCE);

    QTreeWidgetItem *i_histogram_energy = new QTreeWidgetItem(ui->tw_main);
    i_histogram_energy->setText(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setToolTip(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setStatusTip(0, NAME_HISTOGRAM_ENERGY);
    i_histogram_energy->setIcon(0, QIcon(NAME_HISTOGRAM_ENERGY_ICON));
    i_histogram_energy->setData(0, Qt::UserRole,    ID_HISTOGRAM_ENERGY);

    QTreeWidgetItem *i_imitator_brg = new QTreeWidgetItem(ui->tw_main);
    i_imitator_brg->setText(0, NAME_IMITATOR_BRG);
    i_imitator_brg->setToolTip(0, NAME_IMITATOR_BRG);
    i_imitator_brg->setStatusTip(0, NAME_IMITATOR_BRG);
    i_imitator_brg->setIcon(0, QIcon(NAME_IMITATOR_BRG_ICON));
    i_imitator_brg->setData(0, Qt::UserRole,    ID_IMITATOR_BRG);
    //---
    connect(ui->tw_main,    SIGNAL(clicked(QModelIndex)),   this,   SLOT(click(QModelIndex)));
    //---

    //---
    o_options       = new Main_options();
    o_oscilloscope  = new Oscilloscope_options();
    o_floodmap      = new Floodmap_options();
    o_consulience   = new Histogram_consulience_options();
    o_energy        = new Histogram_energy_options();
    o_imitator_brg  = new Imitator_BRG_options();

    index_options = ui->sw_main->addWidget(o_options);
    index_oscilloscope = ui->sw_main->addWidget(o_oscilloscope);
    index_floodmap = ui->sw_main->addWidget(o_floodmap);
    index_consulience = ui->sw_main->addWidget(o_consulience);
    index_energy = ui->sw_main->addWidget(o_energy);
    index_imitator_brg = ui->sw_main->addWidget(o_imitator_brg);

    ui->sw_main->setCurrentIndex(index_options);
    //---

    block_wheel();

    //connect(ui->btn_close,  SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));

    setFixedSize(sizeHint());

    QTimer::singleShot(100, this, SLOT(block_wheel()));
}
//--------------------------------------------------------------------------------
void PETCat_optionsBox::click(QModelIndex)
{
    int index = ui->tw_main->currentItem()->data(0, Qt::UserRole).toInt();
    switch(index)
    {
    case ID_OPTIONS:
        ui->sw_main->setCurrentIndex(index_options);
        break;

    case ID_OSCILLOSCOPE:
        ui->sw_main->setCurrentIndex(index_oscilloscope);
        break;

    case ID_FLOODMAP:
        ui->sw_main->setCurrentIndex(index_floodmap);
        break;

    case ID_HISTOGRAM_CONSULIENCE:
        ui->sw_main->setCurrentIndex(index_consulience);
        break;

    case ID_HISTOGRAM_ENERGY:
        ui->sw_main->setCurrentIndex(index_energy);
        break;

    case ID_IMITATOR_BRG:
        ui->sw_main->setCurrentIndex(index_imitator_brg);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void PETCat_optionsBox::block_wheel(void)
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
void PETCat_optionsBox::t_accept(void)
{
    accept();
}
//--------------------------------------------------------------------------------
bool PETCat_optionsBox::eventFilter(QObject*, QEvent* event)
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
