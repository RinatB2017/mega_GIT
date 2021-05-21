/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef SHOWPICTURE_HPP
#define SHOWPICTURE_HPP
//--------------------------------------------------------------------------------
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QList>
//--------------------------------------------------------------------------------
class ShowPicture : public QLabel
{
    Q_OBJECT

signals:
    void append_rect(int id, int x, int y, int w, int h);
    void delete_rect(int id, int x, int y, int w, int h);

public:
    explicit ShowPicture(QWidget *parent = nullptr);
    virtual ~ShowPicture();

    QList<QRubberBand *> get_rects(void);
    bool clr_rects(void);
    bool get_pixmap(uint x, uint y, uint w, uint h, QPixmap *pixmap);

    void set_rect_max(uint value);

    void test(void);

    bool correct(int x, int y, QRect rect);
    bool correct(int id, QRect rect);

    void create_rect(int x, int y, int w, int h);

private:
    QPoint origin;
    QRubberBand *rubberBand = nullptr;
    QList<QRubberBand *> l_bands;
    bool flag_clicked = false;

    int max_rect = 100;

    void init(void);

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};
//--------------------------------------------------------------------------------
#endif
