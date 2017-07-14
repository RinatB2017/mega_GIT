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
#ifndef GRAPHERCURVEBOX_HPP
#define GRAPHERCURVEBOX_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QCheckBox;
class QComboBox;
class QLineEdit;
class QLabel;
//--------------------------------------------------------------------------------
class GrapherCurveBox : public QFrame
{
    Q_OBJECT

public:
    explicit GrapherCurveBox(QWidget *parent);

    void set_color(int r, int g, int b);
    void set_caption(const QString &caption);
    void set_index(int index);

    QString get_caption(bool *ok);
    void get_color(int *r, int *g, int *b);
    int get_index(void);

    void set_symbol_state(bool state);
    bool get_symbol_state(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void change_color(void);

private:
    enum CURVE {
        DOTS = 0,
        LINES,
        SPLINE_LINES
    };

    int index;
    QLineEdit *caption_curve;
    QLabel *color_curve;
    QCheckBox *cb_symbol;
    QComboBox *cb_type_curve;

    void connect_log(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // GRAPHERCURVEBOX_HPP
