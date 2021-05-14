/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef BONE_15_HPP
#define BONE_15_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
class Bone_15 : public QWidget
{
    Q_OBJECT

public:
    explicit Bone_15(int number,
                        int size,
                        int font_size,
                        QWidget *parent = nullptr);
    void save(QString filename);

private:
    int width = 0;
    int height = 0;
    int border = 0;
    int number = 0;
    int font_size = 0;

    QColor begin_color = Qt::lightGray;
    QColor end_color   = Qt::darkGray;

    QColor begin_border_color = Qt::lightGray;
    QColor end_border_color   = Qt::darkGray;

    QColor number_color = Qt::white;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
