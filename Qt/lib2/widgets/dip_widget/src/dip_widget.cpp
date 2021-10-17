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
#include "dip_widget.hpp"
//--------------------------------------------------------------------------------
DIP_widget::DIP_widget(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
DIP_widget::~DIP_widget()
{

}
//--------------------------------------------------------------------------------
void DIP_widget::init(void)
{
    DIP_button btn;

    int pos_x = DIP_BEGIN_X;
    for(int n=0; n<NUM_BUTTONS; n++)
    {
        int x = pos_x;
        int y = DIP_BEGIN_Y;
        int w = DIP_WIDTH;
        int h = DIP_HEIGHT;
        btn.rect = QRect(x,y,w,h);
        btn.state = false;
        btn.num = n;
        buttons.append(btn);

        pos_x += (DIP_WIDTH + 4);
    }

    setToolTip(QString("Адрес = %1").arg(dip_value));
    setFixedSize(MAX_WIDTH, MAX_HEIGHT);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void DIP_widget::set_value(int value)
{
    if(value < MIN_DIP_VALUE)   value = MIN_DIP_VALUE;
    if(value > MAX_DIP_VALUE)   value = MAX_DIP_VALUE;

    dip_value = value;

    union DATA_INT8 {
        uint8_t value;
        struct {
            uint8_t bit_0 : 1;
            uint8_t bit_1 : 1;
            uint8_t bit_2 : 1;
            uint8_t bit_3 : 1;
            uint8_t bit_4 : 1;
            uint8_t bit_5 : 1;
            uint8_t bit_6 : 1;
            uint8_t bit_7 : 1;
        } bites;
    };

    DATA_INT8 b;
    b.value = static_cast<uint8_t>(value);
    buttons[0].state = b.bites.bit_0;
    buttons[1].state = b.bites.bit_1;
    buttons[2].state = b.bites.bit_2;
    buttons[3].state = b.bites.bit_3;
    buttons[4].state = b.bites.bit_4;
    buttons[5].state = b.bites.bit_5;
    buttons[6].state = b.bites.bit_6;
    buttons[7].state = b.bites.bit_7;

    repaint();
    setToolTip(QString("Адрес = %1").arg(dip_value));
}
//--------------------------------------------------------------------------------
int DIP_widget::get_value(void)
{
    return dip_value;
}
//--------------------------------------------------------------------------------
void DIP_widget::block_interface(bool state)
{
    is_blocked = state;
    if(state)
    {
        color = Qt::gray;
    }
    else
    {
        color = Qt::blue;
    }
    repaint();
}
//--------------------------------------------------------------------------------
void DIP_widget::unlock_interface(bool state)
{
    block_interface(!state);
}
//--------------------------------------------------------------------------------
bool DIP_widget::check_pos(QRect rect, QPoint pos)
{
    if(pos.x() < rect.x())      return false;
    if(pos.x() > rect.right())  return false;
    if(pos.y() < rect.y())      return false;
    if(pos.y() > rect.bottom()) return false;

    return true;
}
//--------------------------------------------------------------------------------
void DIP_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.setPen(QPen(Qt::blue));
    painter.setBrush(QBrush(color));
    painter.drawRect(0, 0, width()-1, height()-1);

    QFont font("Courier", 6);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
#ifdef FLIP_DIP_WIDGET
    painter.drawText(8,
                     DIP_BEGIN_Y + 5,
                     "ON");
    painter.drawLine(15,
                     DIP_BEGIN_Y + 7,
                     15,
                     DIP_BEGIN_Y + DIP_HEIGHT);
    painter.drawLine(15,
                     DIP_BEGIN_Y + 7,
                     10,
                     DIP_BEGIN_Y + 15);
    painter.drawLine(15,
                     DIP_BEGIN_Y + 7,
                     20,
                     DIP_BEGIN_Y + 15);
#else
    painter.drawText(8,
                     DIP_BEGIN_Y + DIP_HEIGHT,
                     "ON");
    painter.drawLine(15,
                     DIP_BEGIN_Y,
                     15,
                     DIP_BEGIN_Y + 15);
    painter.drawLine(10,
                     DIP_BEGIN_Y + 10,
                     15,
                     DIP_BEGIN_Y + 15);
    painter.drawLine(20,
                     DIP_BEGIN_Y + 10,
                     15,
                     DIP_BEGIN_Y + 15);
#endif

    foreach (auto button, buttons)
    {
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(button.rect);

        painter.setBrush(QBrush(Qt::gray));
#ifdef FLIP_DIP_WIDGET
        QRect pos_slider_on = QRect(button.rect.x(),
                                    button.rect.y(),
                                    button.rect.width(),
                                    8);
        QRect pos_slider_off = QRect(button.rect.x(),
                                     button.rect.bottom() - 8,
                                     button.rect.width(),
                                     8);
#else
        QRect pos_slider_on = QRect(button.rect.x(),
                                    button.rect.bottom() - 8,
                                    button.rect.width(),
                                    8);
        QRect pos_slider_off = QRect(button.rect.x(),
                                     button.rect.y(),
                                     button.rect.width(),
                                     8);
#endif
        if(button.state)
        {
            painter.drawRect(pos_slider_on);
        }
        else
        {
            painter.drawRect(pos_slider_off);
        }

        painter.setFont(font);
        painter.setPen(QPen(Qt::white));
        painter.drawText(button.rect.x() + 2,
                         button.rect.y() + button.rect.bottom() + 2,
                         QString("%1").arg(button.num+1));
    }

    Q_UNUSED(event)
}
//--------------------------------------------------------------------------------
void DIP_widget::mousePressEvent(QMouseEvent *event)
{
    if(is_blocked)
    {
        return;
    }
    if(event->button()==Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();
        for(int n=0; n<buttons.size(); n++)
        {
            if(check_pos(buttons[n].rect, QPoint(x,y)))
            {
                buttons[n].state = !buttons[n].state;
                int val = 0;
                foreach (auto btn, buttons)
                {
                    if(btn.state)
                    {
                        val |= (1 << btn.num);
                    }
                }
                dip_value = val;
                emit value(val);
                repaint();
                setToolTip(QString("Адрес = %1").arg(dip_value));
            }
        }
    }
}
//--------------------------------------------------------------------------------
