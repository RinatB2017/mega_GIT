/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef DIP_WIDGET_HPP
#define DIP_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QPen>
//--------------------------------------------------------------------------------
// #define MAX_WIDTH   150
#define MAX_WIDTH   100
#define MAX_HEIGHT  50

// #define NUM_BUTTONS 8
#define NUM_BUTTONS 5

#define DIP_WIDTH   10
#define DIP_HEIGHT  25

#define DIP_BEGIN_X 25
#define DIP_BEGIN_Y 10

#define MIN_DIP_VALUE   0
#define MAX_DIP_VALUE   31
//--------------------------------------------------------------------------------
namespace Ui {
    class DIP_widget;
}
//--------------------------------------------------------------------------------
class DIP_widget : public QWidget
{
    Q_OBJECT

public:
    explicit DIP_widget(QWidget *parent = nullptr);
    virtual ~DIP_widget();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void value(int);

public slots:
    void set_value(int value);
    int  get_value(void);

    void block_interface(bool state);
    void unlock_interface(bool state);

private:
    struct DIP_button {
        QRect   rect;
        int     num;
        bool    state;
    };

    bool is_blocked = false;

    QColor color = Qt::blue;

    int dip_value = 0;
    QList<DIP_button> buttons;

    void init(void);
    bool check_pos(QRect rect, QPoint pos);

protected:
    void paintEvent (QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif // DIP_WIDGET_HPP
