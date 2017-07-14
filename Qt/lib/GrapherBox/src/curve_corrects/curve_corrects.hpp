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
#ifndef CURVE_CORRECTS_HPP
#define CURVE_CORRECTS_HPP
//--------------------------------------------------------------------------------
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QColor>
//--------------------------------------------------------------------------------
#include "qwt_picker.h"
//--------------------------------------------------------------------------------
#include "curve_corrects.hpp"
#include "grapher_curvebox.hpp"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
class CurveCorrects : public QDialog
{
    Q_OBJECT

public:
    explicit CurveCorrects(QVector<GRAPHER_CURVE> curves,
                           QWidget *parent);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private:
    QVector<GRAPHER_CURVE> temp_curves;
    QVector<GrapherCurveBox *> curve_boxs;

    void connect_log(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif
