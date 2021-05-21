/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef HISTOGRAMBOX_HPP
#define HISTOGRAMBOX_HPP
//--------------------------------------------------------------------------------
#include <QFileDialog>
//--------------------------------------------------------------------------------
#include "qwt_plot_histogram.h"
#include "qwt_series_data.h"
#include "qwt_legend_data.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class HistogramBox;
}
//--------------------------------------------------------------------------------
#define MAX_CHANNELS 10
//--------------------------------------------------------------------------------
typedef struct
{
    bool is_active;
    unsigned int pos_x;
    int index_histogram;
    QString title;
    QColor color;
    QwtPlotHistogram *plot_histogram;
    QVector<QwtIntervalSample> data_histogram;
} HISTOGRAM_CURVE;
//--------------------------------------------------------------------------------
class HistogramBox : public MyWidget
{
    Q_OBJECT

public:
    HistogramBox(QWidget *parent = nullptr);
    virtual ~HistogramBox();

    void set_axis_scale_x(double new_axis_X_min, double new_axis_X_max);
    void set_axis_scale_y(double new_axis_Y_min, double new_axis_Y_max);

    void set_title(const QString &title);
    void set_title_axis_X(const QString &title);
    void set_title_axis_Y(const QString &title);

    void setVisible_btn_Options(bool state);
    void setVisible_btn_Load(bool state);
    void setVisible_btn_Save(bool state);
    void setVisible_btn_Statistic(bool state);

    void push_btn_Horizontal(bool state);
    void push_btn_Vertical(bool state);

    void test(void);

    int get_histograms_count(void);

    int get_pos_x(int index_histogram);

    bool get_histogram_data(int channel,
                            int index,
                            float *data);

    void remove_all_histogram(void);

public slots:
    int add_histogram(int index_histogram,
                      const QString &title,
                      const QColor &color);
    void remove_histogram(int index_histogram);

    void set_histogram_style(int index_histogram,
                             QwtPlotHistogram::HistogramStyle style);

    bool add_histogram_data(int index_histogram,
                            unsigned int pos_x,
                            unsigned int width,
                            int height);
    bool add_histogram_data(int index_histogram,
                            unsigned int width,
                            int height);
    void clear(void);
    void reset(void);

private slots:
    void updateGraphics(void);
    void legend_checked(const QVariant &itemInfo, bool on);
    void options(void);

    void load_histograms(void);
    void save_histograms(void);

    void tune_vertical_axis(bool state = true);
    void tune_horizontal_axis(bool state = true);

    void popup(QPoint);
    void slot_setting_action(void);

    void statistic(void);

private:
    Ui::HistogramBox *ui;

    QwtPlotPicker *d_picker;

    QwtLegend *legend;

    QVector<HISTOGRAM_CURVE> histograms;

    double axis_X_min = 0;
    double axis_X_max = 0;
    double axis_Y_min = 0;
    double axis_Y_max = 0;

    QString title;
    QString title_axis_X;
    QString title_axis_Y;

    void init(void);
    void create_widgets(void);

    QVariant itemToInfo(QwtPlotItem *plotItem) const;
    void show_histogram( QwtPlotItem *, bool on );

    int getPositionStep(void);
    int getSizeStep(void);

    void test_get_info(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
