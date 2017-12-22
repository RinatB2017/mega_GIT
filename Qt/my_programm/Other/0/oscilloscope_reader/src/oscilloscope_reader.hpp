/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef OSCILLOSCOPE_READER_HPP
#define OSCILLOSCOPE_READER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFile>
//--------------------------------------------------------------------------------
namespace Ui {
    class OscilloscopeBox_reader;
}
//--------------------------------------------------------------------------------
class GrapherBox;
//--------------------------------------------------------------------------------
class OscilloscopeBox_reader : public QWidget
{
    Q_OBJECT

public:
    explicit OscilloscopeBox_reader(QWidget *parent);
    ~OscilloscopeBox_reader();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private slots:
    void choose_file(void);
    void prev_osc(void);
    void next_osc(void);

private:
    Ui::OscilloscopeBox_reader *ui;

    QFile filedata;

    QList<QByteArray> samples;
    int index_samples;

    GrapherBox *grapher;
    int curve_xp;
    int curve_xn;
    int curve_yp;
    int curve_yn;

    int cnt_samples;

    void init(void);
    void block_this_button(bool state);
    void block_interface(bool state);
};
//--------------------------------------------------------------------------------
#endif
