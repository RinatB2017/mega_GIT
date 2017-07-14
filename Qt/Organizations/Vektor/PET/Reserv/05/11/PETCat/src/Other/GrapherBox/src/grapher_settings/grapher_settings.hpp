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
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QColor>
//--------------------------------------------------------------------------------
#include "qwt_picker.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "grapher_curvebox.hpp"
//--------------------------------------------------------------------------------
class GrapherSettings : public QDialog
{
    Q_OBJECT

public:
    explicit GrapherSettings(QVector<GRAPHER_CURVE> curves,
                             QwtPlotPicker *picker,
                             QWidget *parent);
    void get_RGB(int index, int *r, int *g, int *b, bool *ok);
    QString get_caption(int index, bool *ok);
    QwtPicker::RubberBand get_rubberband(void);
    QColor get_rubberband_color(void);
    bool get_symbol_state(int index);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private:
    QVector<GRAPHER_CURVE> temp_curves;
    QVector<GrapherCurveBox *> curve_boxs;
    QComboBox *rubberband_curve;
    QLabel *rubberband_color;

    void connect_log(void);
    void change_rubberband_color(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // GRAPHER_SETTINGS_HPP
