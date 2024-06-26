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
#ifndef OscilloscopeBOX_HPP
#define OscilloscopeBOX_HPP
//--------------------------------------------------------------------------------
#include <QColorDialog>
//#include <limits.h>
//--------------------------------------------------------------------------------
#include <qwt_picker_machine.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend_label.h>
#include "qwt_legend_data.h"
#include <qwt_legend.h>
#include <qwt_knob.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define CURVE_COLORS "curve_colors"
//#define COLOR "color"
#define COLOR_R "color_r"
#define COLOR_G "color_g"
#define COLOR_B "color_b"
//--------------------------------------------------------------------------------
#define MIN_CURVES  1
#define MAX_CURVES  8
//--------------------------------------------------------------------------------
namespace Ui {
    class OscilloscopeBox;
}
//--------------------------------------------------------------------------------
class QwtPlotPicker;
class QwtPlotCurve;
class QwtPlotItem;
class QToolButton;
class SerialBox5;
class CurveData;
class QwtLegend;
class QwtSymbol;
class QCheckBox;
class QwtPlot;
class QFrame;
class QTimer;
class Plot;
//--------------------------------------------------------------------------------
typedef struct
{
    int index_curve;
    int curve_ID;
    QString title;
    QColor color;
    float last_value;
    float correction_gain;
    float correction_pos_y;
    QwtPlotCurve *plot_curve;
    CurveData *data_curve;
} OSCILLOSCOPE_CURVE;
//--------------------------------------------------------------------------------
class OscilloscopeBox : public MyWidget
{
    Q_OBJECT

public:
    explicit OscilloscopeBox(QWidget *parent = nullptr);
    virtual ~OscilloscopeBox();

    void test(void);
    bool set_curve_color(int index, QColor color);
    bool get_curve_color(int index, QColor *color);

public slots:
    void add_curve_data(int channel,
                        qreal data);

private slots:
    void legend_state(int curve_ID, bool state);

    void set_legend_is_visible(bool state);

    int add_curve(int index_curve,
                  int pos_y,
                  const QColor &color);
    void updateGraphics(void);
    void legend_checked(const QVariant &itemInfo, bool on);

    void click(bool state);

    void update(void);

    void click_channel(int channel, bool state);
    void click_RUN(void);

    void position_changed(double value);
    void multiply_changed(double value);

    void set_color(int index);

private:
    Ui::OscilloscopeBox *ui;
    bool legend_is_visible = false;

    QwtPlotPicker *d_picker = nullptr;
    QwtLegend *legend = nullptr;

    QColor color = Qt::black;

    int num_curves = 0;
    QVector<OSCILLOSCOPE_CURVE> curves;

    QTimer *timer = nullptr;

    double axis_X_min = 0;
    double axis_X_max = 0;
    double axis_Y_min = 0;
    double axis_Y_max = 0;

    bool state_RUN = true;

    int current_channel = -1;

    QString title;
    QString title_axis_X;
    QString title_axis_Y;

    void init(void);
    void create_widgets(void);
    void create_timer(void);
    void create_curves(void);

    QVariant itemToInfo(QwtPlotItem *plotItem) const;
    void showCurve(QwtPlotItem *, bool on);

#ifndef GRAPHER_NOT_PANNING
    // panning with the left mouse button
    QwtPlotPanner *plot_panner = nullptr;
#endif

#ifndef GRAPHER_NOT_ZOOM
    // zoom in/out with the wheel
    QwtPlotMagnifier *plot_magnifier = nullptr;
#endif

    void set_zoom(bool x_state, bool y_state);
    void set_panning(bool x_state, bool y_state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
