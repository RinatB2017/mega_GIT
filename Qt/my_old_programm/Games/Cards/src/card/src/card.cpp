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
#include <QSvgRenderer>
#include <QVBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "card.hpp"
//--------------------------------------------------------------------------------
Card::Card(QString name,
           int pos_x,
           int pos_y,
           int card_width,
           int card_height,
           QSvgRenderer *renderer,
           QWidget *parent) :
    MyWidget(parent)
{
    card_name = name;

    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->card_width = card_width;
    this->card_height = card_height;
    this->re = renderer;

    init();
}
//--------------------------------------------------------------------------------
void Card::init(void)
{
    bool ok = false;
    label = new QLabel();

    QPixmap *card_0 = new QPixmap(re->defaultSize().width(),
                                  re->defaultSize().height());
    card_0->fill( Qt::transparent );
    QPainter pai(card_0);
    re->render(&pai);

    QPixmap orig = card_0->copy(pos_x,
                                pos_y,
                                card_width,
                                card_height);

    QBitmap mask = orig.createHeuristicMask();
#if 0
    ok = mask.save(QString("%1_mask.png").arg(card_name));
    if(!ok)
    {
        emit error("cannot create mask.png");
    }
#endif

    int corr_x1 = get_corr_x1(&mask);
    int corr_x2 = get_corr_x2(&mask);
    int corr_y1 = get_corr_y1(&mask);
    int corr_y2 = get_corr_y2(&mask);

#if 0
    emit info(QString("pos_x %1").arg(pos_x));
    emit info(QString("pos_y %1").arg(pos_y));
    emit info(QString("card_width %1").arg(card_width));
    emit info(QString("card_height %1").arg(card_height));

    emit info(QString("corr_x1 %1").arg(corr_x1));
    emit info(QString("corr_x2 %1").arg(corr_x2));
    emit info(QString("corr_y1 %1").arg(corr_y1));
    emit info(QString("corr_y2 %1").arg(corr_y2));

    emit info(QString("result pos_x %1").arg(pos_x + corr_x1));
    emit info(QString("result pos_y %1").arg(corr_y1));
    emit info(QString("result card_width %1").arg(card_width - corr_x1 - corr_x2));
    emit info(QString("result card_height %1").arg(card_height - corr_y1 - corr_y2));
#endif

    QPixmap result = orig.copy(corr_x1,
                               corr_y1,
                               card_width - corr_x1 - corr_x2,
                               card_height - corr_y1 - corr_y2);

#ifdef SAVE_CARD
    ok = result.save(QString("cards/%1.png").arg(card_name));
    if(ok == false)
    {
        emit error(QString("cannot create %1.png").arg(card_name));
    }
#endif

    label->setPixmap(result);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(label);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
int Card::get_corr_x1(QPixmap *pix)
{
    emit debug("get_corr_x1");

    QImage image = pix->toImage();
    for(int n=0; n<20; n++)
    {
        QRgb rgb = image.pixel(n, card_height/2);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit debug(QString("%1 %2 %3 %4 %5")
                   .arg(rgb)
                   .arg(r)
                   .arg(g)
                   .arg(b)
                   .arg(qAlpha(rgb)));
        if(r!=255 && b!=255 && g!=255)
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
int Card::get_corr_y1(QPixmap *pix)
{
    emit debug("get_corr_y1");

    QImage image = pix->toImage();
    for(int n=0; n<20; n++)
    {
        QRgb rgb = image.pixel(card_width/2, n);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit debug(QString("%1 %2 %3 %4 %5")
                   .arg(rgb)
                   .arg(r)
                   .arg(g)
                   .arg(b)
                   .arg(qAlpha(rgb)));
        if(r!=255 && b!=255 && g!=255)
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
int Card::get_corr_x2(QPixmap *pix)
{
    emit debug("get_corr_x2");

    QImage image = pix->toImage();
    for(int n=0; n<20; n++)
    {
        QRgb rgb = image.pixel(card_width - n - 1, card_height/2);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit debug(QString("%1 %2 %3 %4 %5")
                   .arg(rgb)
                   .arg(r)
                   .arg(g)
                   .arg(b)
                   .arg(qAlpha(rgb)));
        if(r!=255 && b!=255 && g!=255)
        {
            return n;
        }
    }
    return 0;

}
//--------------------------------------------------------------------------------
int Card::get_corr_y2(QPixmap *pix)
{
    emit debug("get_corr_y2");

    QImage image = pix->toImage();
    for(int n=0; n<20; n++)
    {
        QRgb rgb = image.pixel(card_width/2, card_height - n - 1);
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);
        emit debug(QString("%1 %2 %3 %4 %5")
                   .arg(rgb)
                   .arg(r)
                   .arg(g)
                   .arg(b)
                   .arg(qAlpha(rgb)));
        if(r!=255 && b!=255 && g!=255)
        {
            return n;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
void Card::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        raise();
        lastPoint = event->pos();
        b_move = true;
    }
}
//--------------------------------------------------------------------------------
void Card::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - lastPoint));
    }
}
//--------------------------------------------------------------------------------
void Card::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && b_move)
    {
        b_move = false;
    }
}
//--------------------------------------------------------------------------------
