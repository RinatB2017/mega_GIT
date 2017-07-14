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
#ifndef HISTOGRAM_CONSILIENCE_HPP
#define HISTOGRAM_CONSILIENCE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "mycombobox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
#include "eventcoincidencegen.hpp"
#include "eventsim.hpp"
#include "gauss.hpp"
#include "pet.h"
#include "peteventcmp.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Histogram_consilience;
}
//--------------------------------------------------------------------------------
class GrapherBox;
class DAT3;
class QSplitter;
class MetaData_widget;
//--------------------------------------------------------------------------------
class Histogram_consilience : public MyWidget
{
    Q_OBJECT

public:
    explicit Histogram_consilience(QString file_auto_open, QWidget *parent);
    ~Histogram_consilience();

signals:
    void signal_add_metadata_file(void);

public slots:
    void refresh_metadata(void);

private slots:
    void refresh(void);

    void refresh_click(void);

    void save_picture_to_file(void);
    void save_picture_to_file_old(void);

    void get_event(pet_event event);

    void grapher_refresh(void);

    void set_tbTts(double value);
    void set_event_flags(void);

    //---
    void channel_a_expanded(bool state);
    void channel_b_expanded(bool state);
    void ts_expanded(bool state);
    void consillence_expanded(bool state);
    void analiz_expanded(bool state);
    void events_flag_expanded(bool state);
    void param_expanded(bool state);
    //---

private:
    Ui::Histogram_consilience *ui = 0;

    MetaData_widget *mtw = 0;
    bool convert_metadata(QString name, QString *result);

    GrapherBox *grapher = 0;
    int curve_Pdt = 0;
    int curve_gauss = 0;

    uint8_t event_flags = 0;

    void ShowGaus(long NNN, double mu, double sigma);

    quint64 EventTotal = 0;
    quint64 CmpNum = 0;

    DAT3 *dat3 = 0;

    char mSourceA = 0;
    char mSourceB = 0;

    double tsA = 0;
    double tsB = 0;

    long mMax = 0;

    int *buf = 0;

    QSplitter *main_splitter = 0;

    void init(void);

    void clear_all(void);

    void init_dat3(void);
    void init_grapher(void);

    bool check_event_flags(uint8_t filter_flags, uint8_t event_flags);

    void test(void);
};
//--------------------------------------------------------------------------------
#endif
