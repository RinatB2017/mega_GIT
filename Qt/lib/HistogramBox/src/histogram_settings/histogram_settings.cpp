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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qwt_plot_histogram.h"
#include "qwt_plot_picker.h"
//--------------------------------------------------------------------------------
#include "histogram_settings.hpp"
#include "histogrambox.hpp"
#include "histogram_curvebox.hpp"
//--------------------------------------------------------------------------------
HistogramSettings::HistogramSettings(QVector<HISTOGRAM_CURVE> curves,
                                     QwtPlotPicker *picker,
                                     QWidget *parent) :
    QDialog(parent)
{
    connect_log();

    QVBoxLayout *vbox = new QVBoxLayout();

    temp_curves.clear();
    foreach (HISTOGRAM_CURVE curve, curves)
    {
        temp_curves.append(curve);
    }

    //int cmd = cb_Cmd->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    rubberband_curve = new QComboBox(this);
    rubberband_curve->addItem("NoRubberBand",       Qt::UserRole + QwtPicker::NoRubberBand);
    rubberband_curve->addItem("HLineRubberBand",    Qt::UserRole + QwtPicker::HLineRubberBand);
    rubberband_curve->addItem("VLineRubberBand",    Qt::UserRole + QwtPicker::VLineRubberBand);
    rubberband_curve->addItem("CrossRubberBand",    Qt::UserRole + QwtPicker::CrossRubberBand);
    rubberband_curve->addItem("RectRubberBand",     Qt::UserRole + QwtPicker::RectRubberBand);
    rubberband_curve->addItem("EllipseRubberBand",  Qt::UserRole + QwtPicker::EllipseRubberBand);
    rubberband_curve->addItem("PolygonRubberBand",  Qt::UserRole + QwtPicker::PolygonRubberBand);
    rubberband_curve->addItem("UserRubberBand",     Qt::UserRole + QwtPicker::UserRubberBand);
    rubberband_curve->setCurrentIndex(picker->rubberBand());

    rubberband_color = new QLabel(this);
    rubberband_color->setFixedSize(32, 32);
    rubberband_color->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                                    .arg(picker->rubberBandPen().color().red())
                                    .arg(picker->rubberBandPen().color().green())
                                    .arg(picker->rubberBandPen().color().blue()));


    QHBoxLayout *rubber_hbox = new QHBoxLayout();
    rubber_hbox->addWidget(rubberband_curve);
    rubber_hbox->addWidget(rubberband_color);

    vbox->addLayout(rubber_hbox);

    curve_boxs.clear();
    foreach (HISTOGRAM_CURVE curve, temp_curves)
    {
        Histogram_CurveBox *histogram_box = new Histogram_CurveBox(this);
        histogram_box->set_index(curve.index_histogram);
        histogram_box->set_caption(curve.title);
        histogram_box->set_color(curve.plot_histogram->brush().color().red(),
                                 curve.plot_histogram->brush().color().green(),
                                 curve.plot_histogram->brush().color().blue());
        vbox->addWidget(histogram_box);
        curve_boxs.append(histogram_box);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    vbox->addWidget(buttonBox);

    rubberband_color->installEventFilter(this);

    setLayout(vbox);
    setWindowIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon));
    setWindowTitle(tr("Настройки"));
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
bool HistogramSettings::eventFilter(QObject *obj, QEvent *event)
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
void HistogramSettings::change_rubberband_color(void)
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
QColor HistogramSettings::get_rubberband_color(void)
{
    return rubberband_color->palette().background().color();
}
//--------------------------------------------------------------------------------
QwtPicker::RubberBand HistogramSettings::get_rubberband(void)
{
    int ribber = rubberband_curve->currentData().toInt() - Qt::UserRole;
    return (QwtPicker::RubberBand)ribber;
}
//--------------------------------------------------------------------------------
void HistogramSettings::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  parent(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), parent(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), parent(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
void HistogramSettings::get_RGB(int index, int *r, int *g, int *b, bool *ok)
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
QString HistogramSettings::get_caption(int index, bool *ok)
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
