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
#ifndef PTZ_VIDEOWIDGET_HPP
#define PTZ_VIDEOWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QVideoWidget>
#include <QToolButton>
//--------------------------------------------------------------------------------
class PTZ_VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit PTZ_VideoWidget(QWidget *parent = nullptr);

private:
    QToolButton *btn_lu = 0;
    QToolButton *btn_ru = 0;

    QToolButton *btn_u = 0;
    QToolButton *btn_d = 0;
    QToolButton *btn_l = 0;
    QToolButton *btn_r = 0;

    QToolButton *btn_ld = 0;
    QToolButton *btn_rd = 0;

protected:
    void resizeEvent (QResizeEvent *);
};
//--------------------------------------------------------------------------------
#endif // PTZ_VIDEOWIDGET_HPP
