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
#ifndef DATA_GRAPHERBOX_HPP
#define DATA_GRAPHERBOX_HPP
//--------------------------------------------------------------------------------
#include "adc_label.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
struct CURVES
{
    QString name;
    ADC_label *obj;
    int curve_index;

    QVector<QPointF> real_data;
};
//--------------------------------------------------------------------------------
namespace Ui {
    class DataGrapherBox;
}
//--------------------------------------------------------------------------------
class DataGrapherBox : public MyWidget
{
    Q_OBJECT

public:
    explicit DataGrapherBox(QWidget *parent = nullptr);
    virtual ~DataGrapherBox();

    int add_curve(QString curve_name);
    void update_curve(int curve_index, const QString &curve_name);

    void add_curves(QStringList sl);
    void add_curves(QList<QByteArray> sl);
    void update_curves(QList<QByteArray> sl);

    void clr_curves();
    bool add_data(int curve_index, qreal value);
    int get_max_index();
    bool get_curve_data(int channel,
                        int index,
                        qreal *data);

private:
    Ui::DataGrapherBox *ui;
    QList<CURVES> curves;

    void init();
    QString convert(qreal value);
    qreal convert_adc(QVariant value);
    QVariant convert_string(QString str_value);

    void show_data_ADC(QStringList sl);
    void change_text(int curve_index, QString text);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // DATA_GRAPHERBOX_HPP
