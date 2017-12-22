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
#ifndef PETCAT_OPTIONS_BOX_HPP
#define PETCAT_OPTIONS_BOX_HPP
//--------------------------------------------------------------------------------
#include <QModelIndex>
#include <QObject>
#include <QDialog>
//--------------------------------------------------------------------------------
#include "page_main_options.hpp"
#include "page_logging.hpp"
#include "page_oscilloscope_options.hpp"
#include "page_floodmap_options.hpp"
#include "page_histogram_consulience_options.hpp"
#include "page_histogram_energy_options.hpp"
#include "page_imitator_brg_options.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class PETCat_optionsBox;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
//--------------------------------------------------------------------------------
class PETCat_optionsBox : public QDialog
{
    Q_OBJECT

public:
    explicit PETCat_optionsBox(QWidget *parent = 0);
    ~PETCat_optionsBox();

    void set_current_index(int index);

signals:
    void signal_is_shows_info(bool);
    void signal_is_shows_debug(bool);
    void signal_is_shows_error(bool);
    void signal_is_shows_trace(bool);

private slots:
    void block_wheel(void);

    void t_accept(void);

    void click(QModelIndex);

private:
    Ui::PETCat_optionsBox *ui = 0;

    //---
    Page_main_options                   *o_options = 0;
    Page_logging                        *o_logging = 0;
    Page_oscilloscope_options           *o_oscilloscope = 0;
    Page_floodmap_options               *o_floodmap = 0;
    Page_histogram_consulience_options  *o_consulience = 0;
    Page_histogram_energy_options       *o_energy = 0;
    Page_imitator_BRG_options           *o_imitator_brg = 0;

    QTreeWidgetItem *i_main_options = 0;
    QTreeWidgetItem *i_logging = 0;
    QTreeWidgetItem *i_oscilloscope = 0;
    QTreeWidgetItem *i_floodmap = 0;
    QTreeWidgetItem *i_histogram_consulience = 0;
    QTreeWidgetItem *i_histogram_energy = 0;
    QTreeWidgetItem *i_imitator_brg = 0;

    int index_main_options = 0;
    int index_logging = 0;
    int index_oscilloscope = 0;
    int index_floodmap = 0;
    int index_consulience = 0;
    int index_energy = 0;
    int index_imitator_brg = 0;
    //---

    void init(void);

protected:
    bool eventFilter(QObject*, QEvent* event);

};
//--------------------------------------------------------------------------------
#endif
