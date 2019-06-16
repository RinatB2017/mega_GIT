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
#ifndef GRAPHERBOX_HPP
#define GRAPHERBOX_HPP
//--------------------------------------------------------------------------------
#include "qwt_curve_fitter.h"
#include "qwt_legend_data.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class GrapherBox;
}
//--------------------------------------------------------------------------------
class QwtPlotPicker;
class QwtScaleDraw;
class QwtPlotCurve;
class QwtPlotItem;
class QToolButton;
class CurveData;
class QwtLegend;
class QwtSymbol;
class QwtPlot;
class Plot;

class QwtPlotPanner;
class QwtPlotMagnifier;
//--------------------------------------------------------------------------------
#define MAX_CHANNELS 32
//--------------------------------------------------------------------------------
typedef struct
{
    bool is_active;
#ifdef USE_SCALE_POINT_DATETIME
    //nothing
#elif defined USE_SCALE_POINT_TIME
    //nothing
#else
    long pos_x;
#endif
    QString title;
    QColor color;

    bool symbol_is_active;
    int curve_ID;
    QwtSymbol::Style symbol_style;
    QBrush symbol_brush;
    QPen symbol_pen;
    QSize symbol_size;

    QwtPlotCurve *plot_curve;

    CurveData *view_curve;
    QVector<QPointF> real_data;
} GRAPHER_CURVE;
//--------------------------------------------------------------------------------
class GrapherBox : public MyWidget
{
    Q_OBJECT

public:
    explicit GrapherBox(QWidget *parent);
    ~GrapherBox();

    void set_axis_scale_x(qreal new_axis_X_min, qreal new_axis_X_max);
    void set_axis_scale_y(qreal new_axis_Y_min, qreal new_axis_Y_max);

    void set_title(const QString &title);
    void set_title_axis_X(const QString &title);
    void set_title_axis_Y(const QString &title);

    void set_legend_is_visible(bool state);

    void set_visible_btn_all_ON(bool state);
    void set_visible_btn_all_OFF(bool state);

    void set_visible_btn_Horizontal(bool state = false);
    void set_visible_btn_Vertical(bool state = false);

    void set_visible_btn_Options(bool state);
    void set_visible_btn_Load(bool state);
    void set_visible_btn_Save(bool state);
    void set_visible_btn_Statistic(bool state);
    void set_visible_btn_Clear(bool state);

    void push_btn_Horizontal(bool state);
    void push_btn_Vertical(bool state);

    void set_silense(bool state);

    int get_curves_count(void);

    void set_zoom(bool x_state, bool y_state);

    void set_panning(bool x_state, bool y_state);

    bool get_vertical_alignment(void);
    bool get_horizontal_alignment(void);

public slots:
    int add_curve(const QString &title,
                  QwtSymbol *symbol = nullptr);
    bool add_curve(const QString &title,
                   int curve_ID);

    void remove_curve(int curve_ID);
    void remove_all_curve(void);

    QColor get_curve_color(int channel);
    bool set_curve_color(int channel, QColor color);

    void set_curve_symbol(int channel,
                          QwtSymbol *symbol);
    void set_curve_style(int channel,
                         QwtPlotCurve::CurveStyle style);
    void set_curve_attribute(int channel,
                             QwtPlotCurve::CurveAttribute attribute);
    void set_curve_fitter(int channel,
                          QwtSplineCurveFitter *fitter);

    void add_curve_data_points(int channel,
                               QVector<QPointF> *points);
    void add_curve_data(int channel,
                        int x,
                        qreal data);
    bool add_curve_data(int channel,
                        qreal data);
    bool add_curve_array(int channel,
                         QList<QPointF> a_points);
    bool get_curve_data(int channel,
                        int index,
                        qreal *data);

    void clear(void);
    void reset(void);

    void click_legend(void);

    //---
    void setAxisScaleDraw(int axisId, QwtScaleDraw *scaleDraw);
    void clear_d_picker(void);
    void set_d_picker(QwtPlotPicker *picker);
    QWidget *get_qwtPlot_canvas(void);
    //---

    void set_axis_label_rotation(int angle);
    void set_axis_label_alignment(int axisId,
                                  Qt::Alignment alignment);

    void correct(int channel,
                 float mul);

    void test(void);
    void test2(void);
    void test_sinus(void);
    void test_single_sinus(void);
    void test_draw_circle(void);

    void legend_on(int curve_ID);
    void legend_off(int curve_ID);

    void legends_all_on(void);
    void legends_all_off(void);

    void updateGraphics(void);

private slots:
    void legend_checked(const QVariant &itemInfo, bool on);
    void options(void);

    void load_curves(void);
    void save_curves(void);

    void set_vertical_alignment(bool state = true);
    void set_horizontal_alignment(bool state = true);

    void popup(QPoint);

    void statistic(void);

    //---
    void clicked(QVariant v, int i);
    void checked(QVariant v, bool b, int i);
    //---

private:
    Ui::GrapherBox *ui;
    bool legend_is_visible = true;

    QwtPlotPicker *d_picker;

    QwtLegend *legend;

    QVector<GRAPHER_CURVE> curves;
    QColor curve_colors[MAX_CHANNELS];

    bool flag_symbol = false;
    bool is_silence = false;

    qreal axis_X_min = 0;
    qreal axis_X_max = 0;
    qreal axis_Y_min = 0;
    qreal axis_Y_max = 0;

    QString title;
    QString title_axis_X;
    QString title_axis_Y;

    //---
    typedef QList<QPointF> data_channel;
    data_channel data_channels[MAX_CHANNELS];
    //---

    bool flag_vertical_alignment = false;
    bool flag_horizontal_alignment = false;

    bool flag_device_RECORDER = false;
    bool flag_device_OSCILLOSCOPE = false;

    bool flag_type_curve_DOTS = false;
    bool flag_type_curve_LINES = false;
    bool flag_type_curve_SPLINE_LINES = false;

    void init(void);
    void create_widgets(void);

    QVariant itemToInfo(QwtPlotItem *plotItem) const;
    void showCurve( QwtPlotItem *, bool on );

    void set_device_RECORDER(void);
    void set_device_OSCILLOSCOPE(void);

    void set_type_curve_DOTS(void);
    void set_type_curve_LINES(void);
    void set_type_curve_SPLINE_LINES(void);

#ifndef GRAPHER_NOT_PANNING
    // panning with the left mouse button
    QwtPlotPanner *plot_panner;
#endif

#ifndef GRAPHER_NOT_ZOOM
    // zoom in/out with the wheel
    QwtPlotMagnifier *plot_magnifier;
#endif

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
