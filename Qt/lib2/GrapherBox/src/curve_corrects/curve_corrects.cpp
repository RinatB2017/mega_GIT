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
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QStyle>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "qwt_plot_picker.h"
//--------------------------------------------------------------------------------
#include "grapher_settings.hpp"
#include "curve_corrects.hpp"
#include "grapher_curvebox.hpp"
//--------------------------------------------------------------------------------
CurveCorrects::CurveCorrects(QVector<GRAPHER_CURVE> curves,
                             QWidget *parent) :
    QDialog(parent)
{
    connect_log();

    QVBoxLayout *vbox = new QVBoxLayout();

    temp_curves.clear();
    foreach (GRAPHER_CURVE curve, curves)
    {
        temp_curves.append(curve);
    }

    foreach (GRAPHER_CURVE curve, temp_curves)
    {
        vbox->addWidget(new QLabel(QString("%1").arg(curve.title)));
    }

    setLayout(vbox);
    setWindowIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon));
    setWindowTitle(tr("Настройки"));
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
bool CurveCorrects::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void CurveCorrects::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  parent(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), parent(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), parent(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
