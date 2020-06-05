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
#ifndef CROPPICTURE_HPP
#define CROPPICTURE_HPP
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
    class CropPicture;
}
//--------------------------------------------------------------------------------
class CropPicture : public MyWidget
{
    Q_OBJECT

public:
    explicit CropPicture(QWidget *parent = nullptr);
    ~CropPicture();

    bool load_pixmap(QPixmap pixmap);

private:
    Ui::CropPicture *ui;

    QPixmap orig_pixmap;
    int pixmap_width = 0;
    int pixmap_height = 0;

    void init(void);

    void up(void);
    void down(void);
    void left(void);
    void right(void);
    void up_left(void);
    void down_right(void);
    void plus(void);
    void minus(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // CROPPICTURE_HPP
