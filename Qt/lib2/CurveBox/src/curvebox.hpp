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
#ifndef CURVEBOX_HPP
#define CURVEBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QVector>
#include <QPointF>
//--------------------------------------------------------------------------------
#include "qwt_legend_data.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class CurveBox;
}
//--------------------------------------------------------------------------------
class QwtPlotPicker;
class QwtPlotCurve;
class QwtPlotItem;
class QToolButton;
class CurveData;
class QwtLegend;
class QwtSymbol;
class QwtPlot;
class Plot;
//--------------------------------------------------------------------------------
class CurveBox : public QWidget
{
    Q_OBJECT

public:
    explicit CurveBox(unsigned int max_axis_X,
                      double min_axis_Y,
                      double max_axis_Y,
                      const QString &title,
                      const QString &title_axis_X,
                      const QString &title_axis_Y,
                      QWidget *parent = nullptr);
    ~CurveBox();
    void updateText(void);

    void set_title(const QString &title);
    void set_title_axis_X(const QString &title);
    void set_title_axis_Y(const QString &title);

    void setVisible_btn_Load(bool state);
    void setVisible_btn_Save(bool state);

    void set_silense(bool state);

    void test(void);

    void retranslateUi(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

public slots:
    void set_axis_label_rotation(int angle);
    void set_axis_label_alignment(int axisId, Qt::Alignment alignment);

private slots:
    void updateGraphics(void);
    void legend_checked(const QVariant &itemInfo, bool on);
    void log(const QString &data);

    void load_curve(void);
    void save_curve(void);

private:
    Ui::CurveBox *ui;

    QwtPlotPicker *d_picker;

    QwtPlotCurve *plot_curve;
    CurveData *view_curve;

    QwtLegend *legend;

    bool flag_symbol;
    bool is_silence;

    double max_axis_X;
    double min_axis_Y;
    double max_axis_Y;

    QString title;
    QString title_axis_X;
    QString title_axis_Y;

    void init(void);
    void create_widgets(void);

    QVariant itemToInfo(QwtPlotItem *plotItem) const;

#ifndef NO_TESTBAR_CURVEBOX
    void createTestBar(void);
#endif

    void add_data(void);

    void connect_log(void);
    int getPositionStep(void);
    int getSizeStep(void);

    QPainterPath createPath(QPointF cord, QList<QPointF> points);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void changeEvent(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif
