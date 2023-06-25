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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qwt_plot_picker.h"
//--------------------------------------------------------------------------------
#include "grapher_settings.hpp"
#include "grapherbox.hpp"
#include "grapher_curvebox.hpp"
//--------------------------------------------------------------------------------
GrapherSettings::GrapherSettings(QVector<GRAPHER_CURVE> curves,
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

    curve_boxs.clear();
    foreach (GRAPHER_CURVE curve, temp_curves)
    {
        GrapherCurveBox *curve_box = new GrapherCurveBox(this);
        curve_box->set_caption(curve.title);
        curve_box->set_color(curve.plot_curve->pen().color().red(),
                             curve.plot_curve->pen().color().green(),
                             curve.plot_curve->pen().color().blue());
        vbox->addWidget(curve_box);
        curve_boxs.append(curve_box);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    vbox->addWidget(buttonBox);

    setLayout(vbox);
    setWindowIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon));
    setWindowTitle(tr("Настройки"));
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
bool GrapherSettings::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        change_rubberband_color();
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void GrapherSettings::change_rubberband_color(void)
{
    QColorDialog *dlg = new QColorDialog(this);

    int r = rubberband_color->palette().background().color().red();
    int g = rubberband_color->palette().background().color().green();
    int b = rubberband_color->palette().background().color().blue();
    if(r>254) r=254;
    if(g>254) g=254;
    if(b>254) b=254;
    QColor color = dlg->getColor(QColor(r, g, b), this);
    if(color.isValid())
    {
        rubberband_color->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                                        .arg(color.red())
                                        .arg(color.green())
                                        .arg(color.blue()));
    }
}
//--------------------------------------------------------------------------------
QColor GrapherSettings::get_rubberband_color(void)
{
    return rubberband_color->palette().background().color();
}
//--------------------------------------------------------------------------------
void GrapherSettings::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),    parent(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)),   parent(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)),   parent(), SIGNAL(error(QString)));
    connect(this, SIGNAL(message(QString)), parent(), SIGNAL(message(QString)));
}
//--------------------------------------------------------------------------------
void GrapherSettings::get_RGB(int index, int *r, int *g, int *b, bool *ok)
{
    if(index > curve_boxs.count())
    {
        *ok = false;
        return;
    }
    curve_boxs[index]->get_color(r, g, b);
    *ok = true;
}
//--------------------------------------------------------------------------------
QString GrapherSettings::get_caption(int index, bool *ok)
{
    if(index > curve_boxs.count())
    {
        *ok = false;
        return "";
    }
    *ok = true;
    return curve_boxs[index]->get_caption(ok);
}
//--------------------------------------------------------------------------------
