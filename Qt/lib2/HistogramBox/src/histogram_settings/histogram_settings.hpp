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
#ifndef GRAPHER_SETTINGS_HPP
#define GRAPHER_SETTINGS_HPP
//--------------------------------------------------------------------------------
#include <QDialogButtonBox>
#include <QApplication>
#include <QComboBox>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "qwt_picker.h"
//--------------------------------------------------------------------------------
#include "histogrambox.hpp"
#include "histogram_curvebox.hpp"
//--------------------------------------------------------------------------------
class HistogramSettings : public QDialog
{
    Q_OBJECT
public:
    HistogramSettings(QVector<HISTOGRAM_CURVE> curves,
                      QwtPlotPicker *picker,
                      QWidget *parent);
    void get_RGB(int index, int *r, int *g, int *b, bool *ok);
    QString get_caption(int index, bool *ok);
    QwtPicker::RubberBand get_rubberband(void);
    QColor get_rubberband_color(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private:
    QVector<HISTOGRAM_CURVE> temp_curves;
    QVector<Histogram_CurveBox *> curve_boxs;
    QComboBox *rubberband_curve = nullptr;
    QLabel *rubberband_color = nullptr;

    void connect_log(void);
    void change_rubberband_color(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // GRAPHER_SETTINGS_HPP
