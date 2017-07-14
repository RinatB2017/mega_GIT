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
#include <QWidget>
//--------------------------------------------------------------------------------
#include "qwt_plot_magnifier.h"
#include "qwt_legend_data.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_panner.h"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class OscilloscopeBox;
}
//--------------------------------------------------------------------------------
#define MIN_CURVES  1
#define MAX_CURVES  8
//--------------------------------------------------------------------------------
class QwtPlotPicker;
class QwtPlotCurve;
class QwtPlotItem;
class QToolButton;
class SerialBox;
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
    float correction_multiply;
    float correction_pos_y;
    QwtPlotCurve *plot_curve;
    CurveData *data_curve;
} OSCILLOSCOPE_CURVE;
//--------------------------------------------------------------------------------
class OscilloscopeBox : public MyWidget
{
    Q_OBJECT

public:
    explicit OscilloscopeBox(QWidget *parent = 0);
    ~OscilloscopeBox();
    void updateText(void);
    void test(void);

public slots:
    void add_curve_data(int channel,
                        float data);

private slots:
    void legend_state(int curve_ID, bool state);

    void set_legend_is_visible(bool state);

    int add_curve(int index_curve,
                  int pos_y,
                  const QColor &color);
    void updateGraphics(void);
    void legend_checked(const QVariant &itemInfo, bool on);

    void update(void);

    void click_channel(int channel);
    void click_RUN(void);

    void position_changed(double value);
    void multiply_changed(double value);

private:
    Ui::OscilloscopeBox *ui = 0;
    bool legend_is_visible = false;

    QwtPlotPicker *d_picker = 0;
    QwtLegend *legend = 0;

    unsigned int num_curves = 0;
    QVector<OSCILLOSCOPE_CURVE> curves;

    QTimer *timer = 0;

    double axis_X_min = 0;
    double axis_X_max = 0;
    double axis_Y_min = 0;
    double axis_Y_max = 0;

    bool state_RUN = true;

    int current_channel = -1;
    bool state_current_channel = false;

    void clean_background_all_CH(void);

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
    QwtPlotPanner *plot_panner = 0;
#endif

#ifndef GRAPHER_NOT_ZOOM
    // zoom in/out with the wheel
    QwtPlotMagnifier *plot_magnifier = 0;
#endif

    void set_zoom(bool x_state, bool y_state);
    void set_panning(bool x_state, bool y_state);

protected:
    void changeEvent(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif
