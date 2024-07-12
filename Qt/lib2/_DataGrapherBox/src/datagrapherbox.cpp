/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "datagrapherbox.hpp"
#include "ui_datagrapherbox.h"
//--------------------------------------------------------------------------------
DataGrapherBox::DataGrapherBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::DataGrapherBox)
{
    init();
}
//--------------------------------------------------------------------------------
DataGrapherBox::~DataGrapherBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void DataGrapherBox::add_curves(QList<QByteArray> sl)
{
    foreach(QByteArray ba_curve_name, sl)
    {
        add_curve(QString(ba_curve_name));
    }
}
//--------------------------------------------------------------------------------
void DataGrapherBox::update_curves(QList<QByteArray> sl)
{
    if(sl.count() <= 2)
    {
        emit error("Bad sl count");
        return;
    }

    sl.removeFirst();

    int index = 0;
    foreach(QByteArray ba_curve_name, sl)
    {
        update_curve(index, QString(ba_curve_name));
        index++;
    }
}
//--------------------------------------------------------------------------------
void DataGrapherBox::clr_curves(void)
{
    ui->grapher_widget->remove_all_curve();
    foreach(CURVES cur, curves)
    {
        delete cur.obj;
    }
    curves.clear();
}
//--------------------------------------------------------------------------------
bool DataGrapherBox::add_data(int curve_index, qreal value)
{
    if(curve_index < 0)
    {
        emit error("index too small");
        return false;
    }
    if(curve_index >= curves.count())
    {
        emit error("index too large");
        return false;
    }

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curves.at(curve_index).curve_index, value);
#endif
    curves.at(curve_index).obj->display(value);

    return true;
}
//--------------------------------------------------------------------------------
int DataGrapherBox::get_max_index(void)
{
    return curves.count();
}
//--------------------------------------------------------------------------------
bool DataGrapherBox::get_curve_data(int channel,
                                int index,
                                qreal *data)
{
    if(curves.count() <= 0)
    {
        emit error(tr("curves.count() <= 0"));
        return false;
    }
    if(channel >= curves.count())
    {
        emit error(QString(tr("channel > %1"))
                   .arg(curves.count()));
        return false;
    }
    qreal point;
    ui->grapher_widget->get_curve_data(channel, index, &point);
    *data = point;
    return true;
}
//--------------------------------------------------------------------------------
void DataGrapherBox::init(void)
{
    ui->setupUi(this);

#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 5);

    connect(ui->grapher_widget, &GrapherBox::change_text,   this,   &DataGrapherBox::change_text);

    QTimer::singleShot(100, [this]{
        ui->grapher_widget->set_title("Датчики");
        ui->grapher_widget->set_title_axis_X("время");
        ui->grapher_widget->set_title_axis_Y("параметр");

        ui->grapher_widget->set_visible_btn_Options(false);
        ui->grapher_widget->set_visible_btn_all_ON(false);
        ui->grapher_widget->set_visible_btn_all_OFF(false);
        ui->grapher_widget->set_visible_btn_Statistic(false);
        ui->grapher_widget->set_visible_btn_Autoscroll(false);
        ui->grapher_widget->set_visible_btn_Load(false);
        ui->grapher_widget->set_visible_btn_Save(false);
    });

    ui->grapher_widget->push_btn_Horizontal(true);
    ui->grapher_widget->push_btn_Vertical(true);
#else
    ui->grapher_widget->setVisible(false);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    ui->lcd_layout->setMargin(0);
#endif
    ui->lcd_layout->setSpacing(0);
}
//--------------------------------------------------------------------------------
void DataGrapherBox::change_text(int curve_index, QString text)
{
    if(curve_index < 0)
    {
        emit error("curve_index too small");
        return;
    }
    if(curve_index >= curves.count())
    {
        emit error("curve_index too large");
        return;
    }

    emit debug(QString("change_text %1 %2")
              .arg(curve_index)
              .arg(text));
    curves[curve_index].obj->set_label_text(text);
}
//--------------------------------------------------------------------------------
QString DataGrapherBox::convert(qreal value)
{
    return QString("%1").arg(value, 0, 'f', 2);
}
//--------------------------------------------------------------------------------
qreal DataGrapherBox::convert_adc(QVariant value)
{
    // 5V
    // 10 bit

    // 1024 - 5V
    // x    - y

    qreal res = -1;

    bool ok = false;

    int i_value = value.toInt(&ok);
    if(ok) res = i_value;

    double d_value = value.toDouble(&ok);
    if(ok) res = d_value;

    float f_value = value.toFloat(&ok);
    if(ok) res = static_cast<qreal>(f_value);

    return res;
}
//--------------------------------------------------------------------------------
QVariant DataGrapherBox::convert_string(QString str_value)
{
    bool ok = false;
    QVariant value = -1;

    value = str_value.toLongLong(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toUInt(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toInt(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toDouble(&ok);
    if(ok)
    {
        return value;
    }

    value = str_value.toFloat(&ok);
    if(ok)
    {
        return value;
    }

    emit error(QString("DataGrapherBox: unknown format [%1]").arg(str_value));
    return value;
}
//--------------------------------------------------------------------------------
void DataGrapherBox::show_data_ADC(QStringList sl)
{
    int max_index = sl.count();
    if(curves.length() != max_index)
    {
        emit error("curves.length() != max_index");
        return;
    }
    for(int index=0; index<max_index; index++)
    {
        QVariant value = convert_string(sl.at(index));
#ifndef NO_GRAPHER
        ui->grapher_widget->add_curve_data(curves.at(index).curve_index,    convert_adc(value));
#endif
        curves.at(index).obj->display(convert(convert_adc(value)));
    }
}
//--------------------------------------------------------------------------------
int DataGrapherBox::add_curve(QString curve_name)
{
    // emit trace(Q_FUNC_INFO);

    ADC_label *adc_label = new ADC_label(curve_name, this);
    Q_ASSERT(adc_label);

    CURVES cur;

    cur.name = curve_name;
    cur.obj = adc_label;
#ifndef NO_GRAPHER
    cur.curve_index = ui->grapher_widget->add_curve(curve_name);
#else
    cur.curve_index = 0;
#endif

    curves.append(cur);
    ui->lcd_layout->addWidget(adc_label);
    return cur.curve_index;
}
//--------------------------------------------------------------------------------
void DataGrapherBox::update_curve(int curve_index,
                                  const QString &curve_name)
{
    if(curve_index < 0)
    {
        emit error("curve_index < 0");
        return;
    }
    if(curve_index >= curves.count())
    {
        emit error("curve_index > curves.count()");
        return;
    }
    curves[curve_index].obj->set_label_text(curve_name);
    ui->grapher_widget->set_curve_title(curve_index, curve_name);
}
//--------------------------------------------------------------------------------
void DataGrapherBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool DataGrapherBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void DataGrapherBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void DataGrapherBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
