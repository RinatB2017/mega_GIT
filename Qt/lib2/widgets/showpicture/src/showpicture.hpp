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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class ShowPicture;
}
//--------------------------------------------------------------------------------
class ShowPicture : public MyWidget
{
    Q_OBJECT

signals:
    void add_pixmap(const QPixmap &);

public:
    explicit ShowPicture(QWidget *parent = nullptr);
    ~ShowPicture();

    bool show_picture(const QString &filename);

private:
    Ui::ShowPicture *ui;
    QRubberBand *rubberBand = nullptr;
    QPoint origin;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif // SHOWPICTURE_HPP
