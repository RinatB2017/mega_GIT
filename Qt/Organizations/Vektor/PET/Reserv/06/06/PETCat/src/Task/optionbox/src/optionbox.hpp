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
#ifndef OPTIONBOX_HPP
#define OPTIONBOX_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QSpinBox>
#include <QWidget>
#include <QList>
//--------------------------------------------------------------------------------
#include "PET_comm.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
namespace Ui {
class OptionBox;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class QSplitter;
class QTimer;
class Bos;
//--------------------------------------------------------------------------------
class PET_bos_main_page;
class PET_brg_main_page;
class BRG_sim_page_options;
class TD_page_options;
//--------------------------------------------------------------------------------
class OptionBox : public MyWidget
{
    Q_OBJECT

public:
    explicit OptionBox(QList<pet_discovery_data> pet_config,
                       QWidget *parent);
    ~OptionBox();

private slots:
    void choice_device(QTreeWidgetItem *item, int);

    void update_tree(void);

private:
    Ui::OptionBox *ui = 0;

    PET_comm *comm = 0;

    QSplitter *main_splitter = 0;

    QList<pet_discovery_data> p_pet_config;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
