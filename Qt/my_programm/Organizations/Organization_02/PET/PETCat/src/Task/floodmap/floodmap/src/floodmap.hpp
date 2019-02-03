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
#ifndef FLOODMAP_HPP
#define FLOODMAP_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mycombobox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
class Viewer;
//--------------------------------------------------------------------------------
namespace Ui {
    class FloodMapBox;
}
//--------------------------------------------------------------------------------
class QStandardItemModel;
class QwtColorMap;
class QPushButton;
class GrapherBox;
class Viewer;
class DAT3;
class QSplitter;
class MetaData_widget;
//--------------------------------------------------------------------------------
class FloodMapBox : public MyWidget
{
    Q_OBJECT

public:
    explicit FloodMapBox(QString file_auto_open, QWidget *parent);
    ~FloodMapBox();

signals:
    void signal_add_metadata_file(void);

public slots:
    void refresh_metadata(void);

private slots:
    void refresh(void);
    void refresh_click(void);

    void save_picture_to_file(void);
    void save_picture_to_clipboard(void);

    void get_event(pet_event event);
    void set_event_flags(void);

    //---
    void source_expanded(bool state);
    void energy_expanded(bool state);
    void event_flag_expanded(bool state);
    void picture_expanded(bool state);

    void block_btn_save_picture_to_file(void);
    //---

    void add_metadata_file(void);

#ifdef SHOW_LOCAL_ENERGY
    void show_le(void);
#endif

private:
    Ui::FloodMapBox *ui;

    Viewer *viewer;

#ifdef SHOW_LOCAL_ENERGY
    GrapherBox *grapher_le;
    QPushButton *btn_show_le;
    quint64 buf_energy[MAX_SIZE_BUF_ENERGY];
    void clear_buf_energy(void);
#endif

    MetaData_widget *mtw;
    bool convert_metadata(QString name, QString *result);

    uint32_t min_energy = 0;
    uint32_t max_energy = 0;
    bool flag_enable_filter_min_energy = false;
    bool flag_enable_filter_max_energy = false;

    int max_x = 0;
    int max_y = 0;

    DAT3 *dat3;

    QSplitter *main_splitter;

    QStandardItemModel *filter_model;

    bool ignore_filter_address = false;
    uint8_t filter_address = 0;
    uint8_t event_flags = 0;

    uint64_t maxCount = 0;
    uint64_t eventCount = 0;

    uint64_t array[MAX_SIZE_X][MAX_SIZE_Y];

    //---
    int floodmap_size = 512;
    int maxValue = 100;

    // true COLOR
    // false MONO
    bool is_color = true;
    //---

    void init(void);
    void init_widgets(void);

    void init_viewer(void);
    void init_dat3(void);
    void init_filter_model(void);
    void init_floodmap_glass(void);

    void show_error(const QString &text);

    bool check_filter_address(uint8_t filter_address, uint8_t event_address);
    bool check_event_flags(uint8_t filter_flags, uint8_t event_flags);

    void clear_all(void);

protected:
    void resizeEvent(QResizeEvent *event);

};
//--------------------------------------------------------------------------------
#endif
