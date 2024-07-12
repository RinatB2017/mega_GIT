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
#include <QColorDialog>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QEvent>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "grapher_curvebox.hpp"
//--------------------------------------------------------------------------------
GrapherCurveBox::GrapherCurveBox(QWidget *parent) :
    QFrame(parent)
{
    connect_log();

    index = -1;
    caption_curve = new QLineEdit(this);
    caption_curve->setText("caption");

    color_curve = new QLabel(this);
    color_curve->setFixedSize(32, 32);
    color_curve->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                               .arg(255)
                               .arg(0)
                               .arg(0));


    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(caption_curve);
    hbox->addWidget(color_curve);
    setLayout(hbox);

    color_curve->installEventFilter(this);

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}
//--------------------------------------------------------------------------------
bool GrapherCurveBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        change_color();
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  topLevelWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), topLevelWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), topLevelWidget(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::set_color(int r, int g, int b)
{
    if(r>254) r=254;
    if(g>254) g=254;
    if(b>254) b=254;
    color_curve->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                               .arg(r)
                               .arg(g)
                               .arg(b));
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::set_caption(const QString &caption)
{
    caption_curve->setText(caption);
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::set_index(int index)
{
    this->index = index;
}
//--------------------------------------------------------------------------------
QString GrapherCurveBox::get_caption(bool *ok)
{
    if(caption_curve->text().isEmpty())
    {
        *ok = false;
        return "";
    }
    *ok = true;
    return caption_curve->text();
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::get_color(int *r, int *g, int *b)
{
    *r = color_curve->palette().background().color().red();
    *g = color_curve->palette().background().color().green();
    *b = color_curve->palette().background().color().blue();
}
//--------------------------------------------------------------------------------
int GrapherCurveBox::get_index(void)
{
    return index;
}
//--------------------------------------------------------------------------------
void GrapherCurveBox::change_color(void)
{
    emit info("change_color");
    QColorDialog *dlg = new QColorDialog(this);

    int r = color_curve->palette().background().color().red();
    int g = color_curve->palette().background().color().green();
    int b = color_curve->palette().background().color().blue();
    if(r>254) r=254;
    if(g>254) g=254;
    if(b>254) b=254;
    QColor color = dlg->getColor(QColor(r, g, b), this);
    if(color.isValid())
    {
        color_curve->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                                   .arg(color.red())
                                   .arg(color.green())
                                   .arg(color.blue()));
    }
}
//--------------------------------------------------------------------------------
