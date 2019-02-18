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
#ifndef REPORTSTRING_HPP
#define REPORTSTRING_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
class ReportString : public QWidget
{
    Q_OBJECT
public:
    explicit ReportString(QWidget *parent = nullptr);

    void clear(void);
    void set_title(const QString &title);
    void set_name(const QString &name);
    void set_description(const QString &description);
    void set_multimeter_serno(int serno);

    void set_font_name(const QString &name);
    void set_font_size(int size);
    void set_font_weight(const QString &weight);

    void set_topmargin(int value);
    void set_bottommargin(int value);
    void set_rightmargin(int value);
    void set_leftmargin(int value);

    void add_field(int rect_x_mm,
                   int rect_y_mm,
                   QString font_face,
                   int font_size,
                   QString data_column);

    QString prepare_report_xml(void);

signals:
    
public slots:
    
private:
    QString title;
    QString name;
    QString description;
    QString result_str;
    int multimeter_serno;

    QString font_name;
    int font_size;
    QString font_weight;

    int topmargin;
    int bottommargin;
    int rightmargin;
    int leftmargin;

    int x[11];
    int y[10];

    int mm_to_pixels(int value_mm);
    void add_hline_text(QString *text,
                        int x_mm,
                        int y_mm,
                        int width_mm,
                        int weight = 0);
    void add_rect(QString *data,
                  int x_mm,
                  int y_mm,
                  int width_mm,
                  int heigth_mm,
                  int weight = 0);
    void add_rect_text(QString *data,
                       const QString &text,
                       int x_mm,
                       int y_mm,
                       int width_mm,
                       int heigth_mm,
                       int weight = 0);
    void add_label_text(QString *data,
                        const QString &text,
                        int x_mm,
                        int y_mm,
                        int width_mm,
                        int heigth_mm,
                        int font_size = 8,
                        int weight = 0);
    void add_text(QString *data,
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
                  const QString &text_10);
    void add_field(QString *data,
                   const QString &query,
                   const QString &field_name,
                   int x_mm,
                   int y_mm,
                   int width_mm,
                   int heigth_mm,
                   int weight = 0);
    void add_section(QString *data,
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
                     bool is_pagebreak = false);
    void add_section_wo_data(QString *data,
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
                             const QString &text_10);

    void add_simple_text(QString *data,
                         const QString &text,
                         const QString &alignment,
                         int x_mm,
                         int y_mm,
                         int width_mm,
                         int heigth_mm,
                         int weight = 0);
};
//--------------------------------------------------------------------------------
#endif // REPORTSTRING_HPP
