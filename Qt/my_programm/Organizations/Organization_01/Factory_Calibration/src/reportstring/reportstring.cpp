/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QVector>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "reportstring.hpp"
//--------------------------------------------------------------------------------
ReportString::ReportString(QWidget *parent) :
    QWidget(parent)
{
    clear();

    int temp;

    temp = 0;
    x[0] = temp;
    temp+=65;
    x[1] = temp;
    temp+=25;
    x[2] = temp;
    temp+=20;
    x[3] = temp;
    temp+=40;
    x[4] = temp;
    temp+=20;
    x[5] = temp;
    temp+=20;
    x[6] = temp;
    temp+=20;
    x[7] = temp;
    temp+=20;
    x[8] = temp;
    temp+=20;
    x[9] = temp;
    temp+=20;
    x[10] = temp;

    temp = 6;
    y[0] = temp;
    temp+=20;
    y[1] = temp;
    temp+=6;
    y[2] = temp;
    temp+=5;
    y[3] = temp;
    temp+=10;
    y[4] = temp;
    temp+=5;
    y[5] = temp;
    temp+=10;
    y[6] = temp;
    temp+=5;
    y[7] = temp;
    temp+=10;
    y[8] = temp;
    temp+=5;
    y[9] = temp;
}
//--------------------------------------------------------------------------------
void ReportString::clear(void)
{
    title.clear();
    name.clear();
    description.clear();

    font_name.clear();
    font_size = 0;
    font_weight.clear();

    result_str.clear();

    topmargin = 0.0f;
    bottommargin = 0.0f;
    rightmargin = 0.0f;
    leftmargin = 0.0f;
}
//--------------------------------------------------------------------------------
void ReportString::set_title(const QString &title)
{
    this->title = title;
}
//--------------------------------------------------------------------------------
void ReportString::set_name(const QString &name)
{
    this->name = name;
}
//--------------------------------------------------------------------------------
void ReportString::set_description(const QString &description)
{
    this->description = description;
}
//--------------------------------------------------------------------------------
void ReportString::set_multimeter_serno(int serno)
{
    this->multimeter_serno = serno;
}
//--------------------------------------------------------------------------------
void ReportString::set_font_name(const QString &name)
{
    font_name = name;
}
//--------------------------------------------------------------------------------
void ReportString::set_font_size(int size)
{
    font_size = size;
}
//--------------------------------------------------------------------------------
void ReportString::set_font_weight(const QString &weight)
{
    font_weight = weight;
}
//--------------------------------------------------------------------------------
void ReportString::set_topmargin(int value)
{
    topmargin = value;
}
//--------------------------------------------------------------------------------
void ReportString::set_bottommargin(int value)
{
    bottommargin = value;
}
//--------------------------------------------------------------------------------
void ReportString::set_rightmargin(int value)
{
    rightmargin = value;
}
//--------------------------------------------------------------------------------
void ReportString::set_leftmargin(int value)
{
    leftmargin = value;
}
//--------------------------------------------------------------------------------
int ReportString::mm_to_pixels(int value_mm)
{
    //return (int)((((float)value_mm * 100.0f) / 25.4f) + 0.5f);
    return (4 * value_mm);
}
//--------------------------------------------------------------------------------
void ReportString::add_hline_text(QString *text,
                                  int x_mm,
                                  int y_mm,
                                  int width_mm,
                                  int weight)
{
    text->append("          <line>\n");
    text->append(QString("            <xstart>%1</xstart>\n").arg(mm_to_pixels(x_mm)));
    text->append(QString("            <ystart>%1</ystart>\n").arg(mm_to_pixels(y_mm)));
    text->append(QString("            <xend>%1</xend>\n").arg(mm_to_pixels(x_mm) + mm_to_pixels(width_mm)));
    text->append(QString("            <yend>%1</yend>\n").arg(mm_to_pixels(y_mm)));
    text->append(QString("            <weight>%1</weight>\n").arg(weight));
    text->append("          </line>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_rect(QString *data,
                            int x_mm,
                            int y_mm,
                            int width_mm,
                            int heigth_mm,
                            int weight)
{
    data->append("          <rect>\n");
    data->append(QString("            <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("            <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("            <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("            <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("            <weight>%1</weight>\n").arg(weight));
    data->append("          </rect>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_rect_text(QString *data,
                                 const QString &text,
                                 int x_mm,
                                 int y_mm,
                                 int width_mm,
                                 int heigth_mm,
                                 int weight)
{
    data->append("          <rect>\n");
    data->append(QString("            <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("            <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("            <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("            <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("            <weight>%1</weight>\n").arg(weight));
    data->append("          </rect>\n");
    data->append("          <label>\n");
    data->append("              <rect>\n");
    data->append(QString("              <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("              <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("              <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("              <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("              <weight>%1</weight>\n").arg(weight));
    data->append("              </rect>\n");
    data->append("              <font>\n");
    data->append(QString("                  <face>%1</face>\n").arg(font_name));
    data->append(QString("                  <size>%1</size>\n").arg(font_size));
    data->append(QString("                  <weight>%1</weight>\n").arg(font_weight));
    data->append("              </font>\n");
    data->append("              <hcenter/>\n");
    data->append("              <vcenter/>\n");
    data->append(QString("              <string>%1</string>\n").arg(text));
    data->append("          </label>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_label_text(QString *data,
                                  const QString &text,
                                  int x_mm,
                                  int y_mm,
                                  int width_mm,
                                  int heigth_mm,
                                  int font_size,
                                  int weight)
{
    //qDebug() << x_mm << y_mm << width_mm << heigth_mm << weight;
    data->append("          <label>\n");
    data->append("              <rect>\n");
    data->append(QString("              <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("              <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("              <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("              <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("              <weight>%1</weight>\n").arg(weight));
    data->append("              </rect>\n");
    data->append("              <font>\n");
    data->append(QString("                  <face>%1</face>\n").arg(font_name));
    data->append(QString("                  <size>%1</size>\n").arg(font_size));
    data->append(QString("                  <weight>%1</weight>\n").arg(font_weight));
    data->append("              </font>\n");
    data->append("              <hcenter/>\n");
    data->append("              <vcenter/>\n");
    data->append(QString("              <string>%1</string>\n").arg(text));
    data->append("          </label>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_text(QString *data,
                            float begin_y_mm,
                            int height_mm,
                            const QString &text_1,
                            const QString &text_2,
                            const QString &text_3,
                            const QString &text_4,
                            const QString &text_5,
                            const QString &text_6,
                            const QString &text_7,
                            const QString &text_8,
                            const QString &text_9,
                            const QString &text_10)
{
    add_rect_text(data, text_1,  x[0], begin_y_mm, x[1] - x[0], height_mm);
    add_rect_text(data, text_2,  x[1], begin_y_mm, x[2] - x[1], height_mm);
    add_rect_text(data, text_3,  x[2], begin_y_mm, x[3] - x[2], height_mm);
    add_rect_text(data, text_4,  x[3], begin_y_mm, x[4] - x[3], height_mm);
    add_rect_text(data, text_5,  x[4], begin_y_mm, x[5] - x[4], height_mm);
    add_rect_text(data, text_6,  x[5], begin_y_mm, x[6] - x[5], height_mm);
    add_rect_text(data, text_7,  x[6], begin_y_mm, x[7] - x[6], height_mm);
    add_rect_text(data, text_8,  x[7], begin_y_mm, x[8] - x[7], height_mm);
    add_rect_text(data, text_9,  x[8], begin_y_mm, x[9] - x[8], height_mm);
    add_rect_text(data, text_10, x[9], begin_y_mm, x[10] - x[9], height_mm);
}
//--------------------------------------------------------------------------------
void ReportString::add_field(QString *data,
                             const QString &query,
                             const QString &field_name,
                             int x_mm,
                             int y_mm,
                             int width_mm,
                             int heigth_mm,
                             int weight)
{
    data->append("          <rect>\n");
    data->append(QString("          <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("          <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("          <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("          <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("          <weight>%1</weight>\n").arg(weight));
    data->append("          </rect>\n");
    data->append("          <field>\n");
    data->append("            <rect>\n");
    data->append(QString("            <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("            <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("            <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("            <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("            <weight>%1</weight>\n").arg(weight));
    data->append("            </rect>\n");
    data->append("            <font>\n");
    data->append(QString("                <face>%1</face>\n").arg(font_name));
    data->append(QString("                <size>%1</size>\n").arg(font_size));
    data->append(QString("                <weight>%1</weight>\n").arg(font_weight));
    data->append("            </font>\n");
    data->append("            <hcenter/>\n");
    data->append("            <vcenter/>\n");
    data->append("            <top/>\n");
    data->append("            <data>\n");
    data->append(QString("                <query>%1</query>\n").arg(query));
    data->append(QString("                <column>%1</column>\n").arg(field_name));
    data->append("            </data>\n");
    data->append("          </field>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_section(QString *data,
                               int height_mm,
                               int height_data_mm,
                               const QString &section_name,
                               const QString &text_1,
                               const QString &text_2,
                               const QString &text_3,
                               const QString &text_4,
                               const QString &text_5,
                               const QString &text_6,
                               const QString &text_7,
                               const QString &text_8,
                               const QString &text_9,
                               const QString &text_10,
                               const QString &query_name,
                               bool is_pagebreak)
{
    data->append("      <section>\n");
    if(is_pagebreak)
        data->append("          <pagebreak when=\"at end\"/>\n");

    data->append(QString("        <name>%1</name>\n").arg(section_name));

    data->append("      <group>\n");
    data->append("          <head>\n");
    data->append(QString("              <height>%1</height>\n").arg(mm_to_pixels(height_mm)));
    add_text(data, 0, height_mm, text_1, text_2, text_3, text_4, text_5, text_6, text_7, text_8, text_9, text_10);
    data->append("          </head>\n");
    data->append("      </group>\n");

    data->append("        <detail>\n");
    data->append("          <key>\n");
    data->append(QString("            <query>%1</query>\n").arg(query_name));
    data->append("          </key>\n");
    data->append(QString("          <height>%1</height>\n").arg(mm_to_pixels(height_data_mm)));
    add_field(data, query_name, "measure_name", x[0], 0, x[1] - x[0], height_data_mm);
    add_rect(data, x[1], 0, x[2] - x[1], height_data_mm);
    add_rect(data, x[2], 0, x[3] - x[2], height_data_mm);
    add_field(data, query_name, "deviation",  x[3], 0, x[4] - x[3], height_data_mm);
    add_field(data, query_name, "measure_v1", x[4], 0, x[5] - x[4], height_data_mm);
    add_field(data, query_name, "measure_v2", x[5], 0, x[6] - x[5], height_data_mm);
    add_field(data, query_name, "measure_v3", x[6], 0, x[7] - x[6], height_data_mm);
    add_field(data, query_name, "measure_v4", x[7], 0, x[8] - x[7], height_data_mm);
    add_field(data, query_name, "measure_v5", x[8], 0, x[9] - x[8], height_data_mm);
    add_field(data, query_name, "measure_v6", x[9], 0, x[10] - x[9], height_data_mm);
    data->append("        </detail>\n");
    data->append("      </section>\n");
}
//--------------------------------------------------------------------------------
void ReportString::add_simple_text(QString *data,
                                   const QString &text,
                                   const QString &alignment,
                                   int x_mm,
                                   int y_mm,
                                   int width_mm,
                                   int heigth_mm,
                                   int weight)
{
    data->append("  <label>\n");
    data->append("      <rect>\n");
    data->append(QString("          <x>%1</x>\n").arg(mm_to_pixels(x_mm)));
    data->append(QString("          <y>%1</y>\n").arg(mm_to_pixels(y_mm)));
    data->append(QString("          <width>%1</width>\n").arg(mm_to_pixels(width_mm)));
    data->append(QString("          <height>%1</height>\n").arg(mm_to_pixels(heigth_mm)));
    data->append(QString("          <weight>%1</weight>\n").arg(weight));
    data->append("       </rect>\n");
    data->append("       <font>\n");
    data->append(QString("           <face>%1</face>\n").arg(font_name));
    data->append(QString("           <size>%1</size>\n").arg(font_size));
    data->append(QString("           <weight>%1</weight>\n").arg(font_weight));
    data->append("       </font>\n");
    data->append(QString("       <%1/>\n").arg(alignment));
    data->append(QString("       <string>%1</string>\n").arg(text));
    data->append("   </label>\n");
}
//--------------------------------------------------------------------------------
#include <QFile>
bool ReportString::load_xml(const QString &filename, QString *result_str)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    *result_str = file.readAll();
    return true;
}
//--------------------------------------------------------------------------------
QString ReportString::prepare_report_xml(void)
{
    QString temp;
    temp.clear();

    temp.append("<!DOCTYPE openRPTDef>\n");
    temp.append("    <report>\n");
    temp.append(QString("      <title>%1</title>\n").arg(title));
    temp.append(QString("      <name>%1</name>\n").arg(name));
    temp.append(QString("      <description>%1</description>\n").arg(description));
    temp.append("      <grid>\n");
    temp.append("        <x>0.05</x>\n");
    temp.append("        <y>0.05</y>\n");
    temp.append("      </grid>\n");
    temp.append("      <size>A4</size>\n");
    temp.append("      <landscape/>\n");
    temp.append(QString("      <topmargin>%1</topmargin>\n").arg(mm_to_pixels(topmargin)));
    temp.append(QString("      <bottommargin>%1</bottommargin>\n").arg(mm_to_pixels(bottommargin)));
    temp.append(QString("      <rightmargin>%1</rightmargin>\n").arg(mm_to_pixels(rightmargin)));
    temp.append(QString("      <leftmargin>%1</leftmargin>\n").arg(mm_to_pixels(leftmargin)));
    temp.append("      <querysource>\n");
    temp.append("        <name>report_query_UDC</name>\n");
    temp.append("        <sql>SELECT * FROM data_report_UDC</sql>\n");
    temp.append("      </querysource>\n");
    temp.append("      <querysource>\n");
    temp.append("        <name>report_query_IDC</name>\n");
    temp.append("        <sql>SELECT * FROM data_report_IDC</sql>\n");
    temp.append("      </querysource>\n");

    temp.append("      <rpthead>\n");
    temp.append(QString("           <height>%1</height>\n").arg(mm_to_pixels(y[9])));

    add_label_text(&temp,
                   QString(tr("ПРОТОКОЛ\n"
                              "предъявительских и приемо-сдаточных испытаний\n"
                              "мультиметра B7-86___ ИСМК-411257.001 ТУ    заводской номер %1").arg(multimeter_serno)),
                   0, y[0], 295, y[1] - y[0],
            13);
    add_rect_text(&temp,
                  tr("Виды проверок и испытаний,\n"
                     "наименование параметров и\n"
                     "режимов работы"),
                  x[0], y[1], x[1] - x[0], y[4] - y[1]);
    add_rect_text(&temp,
                  tr("Номер пункта\n"
                     "ИСМК.4111257.001ТУ"),
                  x[1], y[1], x[3] - x[1], y[3] - y[1]);
    add_rect_text(&temp,
                  tr("технических\n"
                     "требований"),
                  x[1], y[3], x[2] - x[1], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("методов\n"
                     "испытаний"),
                  x[2], y[3], x[3] - x[2], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("Значение\n параметра\n"
                     "по\n"
                     "КМСИ.4111645.003 ТУ"),
                  x[3], y[1], x[4] - x[3], y[4] - y[1]);
    add_rect_text(&temp,
                  tr("Результаты проверки"),
                  x[4], y[1], x[10] - x[4], y[2] - y[1]);
    add_rect_text(&temp,
                  tr("регулировщика"),
                  x[4], y[2], x[6] - x[4], y[3] - y[2]);
    add_rect_text(&temp,
                  tr("ОТК"),
                  x[6], y[2], x[8] - x[6], y[3] - y[2]);
    add_rect_text(&temp,
                  tr("ПЗ"),
                  x[8], y[2], x[10] - x[8], y[3] - y[2]);
    add_rect_text(&temp,
                  tr("до технол.\n"
                     "приработки"),
                  x[4], y[3], x[5] - x[4], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("после техн.\n"
                     "приработки"),
                  x[5], y[3], x[6] - x[5], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("1 предъ-\nявление"),
                  x[6], y[3], x[7] - x[6], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("2 предъ-\nявление"),
                  x[7], y[3], x[8] - x[7], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("1 предъ-\nявление"),
                  x[8], y[3], x[9] - x[8], y[4] - y[3]);
    add_rect_text(&temp,
                  tr("2 предъ-\nявление"),
                  x[9], y[3], x[10] - x[9], y[4] - y[3]);
    add_text(&temp, y[4], y[5] - y[4], "1", "2", "3", "4", "5", "6", "7", "8", "9", "10");
    add_text(&temp, y[5], y[6] - y[5],
            tr("Проверка соответствия прибора\n"
               "требованиям комплекта КД"),
            "1.1.1",
            "4.45",
            tr("Соответствие ТУ"),
            "", "", "", "", "", "");
    add_text(&temp, y[6], y[7] - y[6],
            tr("Проверка комплектности прибора"),
            "1.2.1",
            "4.3",
            tr("Соответствие ТУ"),
            "", "", "", "", "", "");
    add_text(&temp, y[7], y[8] - y[7],
            tr("Проверка внешнего вида прибора (внешний осмотр)"),
            "1.3.30,\n1.3.34",
            "4.4,\n4.45",
            tr("Соответствие ТУ"),
            "", "", "", "", "", "");
    add_text(&temp, y[8], y[9] - y[8],
            tr("Проверка эксплуатационной документации"),
            "1.4.1",
            "4.5",
            tr("Соответствие ТУ"),
            "", "", "", "", "", "");
    temp.append("      </rpthead>\n");

    temp.append("      <pghead>\n");
    temp.append("           <even/>\n");
    temp.append(QString("           <height>%1</height>\n").arg(mm_to_pixels(5)));
    add_text(&temp, 0, 5, "1", "2", "3", "4", "5", "6", "7", "8", "9", "10");
    temp.append("      </pghead>\n");

    temp.append("      <rptfoot>\n");
    temp.append(QString("           <height>%1</height>\n").arg(mm_to_pixels(179)));

    int y = 10;
    add_simple_text(&temp, tr("Регулировка и проверка согласно КД:"), "bottom",
                    0, y, 200, 5);
    y+=6;
    add_simple_text(&temp, tr("до технологической приработки    _____________________________________  __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);

    y+=4;
    add_simple_text(&temp, tr("(регулировщик, фамилия, подпись)"), "top",
                    45, y, 200, 5);
    add_simple_text(&temp, tr("(начальник участка регулировки, подпись)"), "top",
                    95, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("после технологической приработки _____________________________________  __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);

    y+=4;
    add_simple_text(&temp, tr("(регулировщик, фамилия, подпись)"), "top",
                    45, y, 200, 5);
    add_simple_text(&temp, tr("(начальник участка регулировки, подпись)"), "top",
                    100, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Заключение ОТК по первому предьявлению прибора _________________________________ __________________________________________"), "bottom",
                    0, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Представитель ОТК _________________________________ __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);
    y+=4;
    add_simple_text(&temp, tr("(подпись, фамилия)"), "top",
                    45, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Заключение ОТК по второму предьявлению прибора _________________________________ __________________________________________"), "bottom",
                    0, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Представитель ОТК _________________________________ __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);
    y+=4;
    add_simple_text(&temp, tr("(подпись, фамилия)"), "top",
                    45, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Заключение ПЗ по первому предьявлению прибора _________________________________ __________________________________________"), "bottom",
                    0, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Представитель заказчика _________________________________ __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);
    y+=4;
    add_simple_text(&temp, tr("(подпись, фамилия)"), "top",
                    45, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Заключение ПЗ по второму предьявлению прибора _________________________________ __________________________________________"), "bottom",
                    0, y, 200, 5);

    y+=6;
    add_simple_text(&temp, tr("Представитель заказчика _________________________________ __________________________________________ \"____\"____________20___г."), "bottom",
                    0, y, 200, 5);
    y+=4;
    add_simple_text(&temp, tr("(подпись, фамилия)"), "top",
                    45, y, 200, 5);

    temp.append("      </rptfoot>\n");

    add_section(&temp,
                20,
                5,
                "my_section_UDC",
                tr("Проверка электрических характеристик прибора\n"
                   "в нормальных условиях применения\n"
                   "Проверка диапазона и основной погрешности\n"
                   "измерения напряжения постоянного тока\n"
                   "в точках:"),
                "1.3.1",
                "4.7",
                tr("Допускаемое значение\n"
                   "основной погрешности, +/-"),
                tr(""), tr(""), tr(""), tr(""), tr(""), tr(""),
                "report_query_UDC",
                true);

    add_section(&temp,
                10,
                5,
                "my_section_IDC",
                tr("Проверка диапазона и основной погрешности\n"
                   "измерения силы постоянного тока в точках:"),
                "1.3.1",
                "4.10",
                tr("Допускаемое значение\n"
                   "основной погрешности, +/-"),
                tr(""), tr(""), tr(""), tr(""), tr(""), tr(""),
                "report_query_IDC");

    temp.append("    </report>\n");

    return temp;
}
//--------------------------------------------------------------------------------
