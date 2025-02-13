/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef MEMORIES_HPP
#define MEMORIES_HPP
//--------------------------------------------------------------------------------
#include <QTextStream>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QtMath>
#include <QLabel>
#include <QImage>
#include <QBrush>
#include <QTimer>
#include <QFile>
#include <QList>
#include <QTime>
#include <QPen>

#include <QtGlobal>
#ifdef Q_OS_WIN
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
class Memories : public QWidget
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    explicit Memories(QWidget *parent = nullptr);

    void read_file(const QString &filename);
    void save_file(const QString &filename);

    void show_image(QImage image);
    void draw_sinus(void);
    void name_filters(void);

    void set_directory(void);
    void single_shot(void);

    QTime diff_time(QTime time_0, QTime time_1);
    QTime elapsed_time(int msec);

    uint16_t convert_array(const QByteArray &ba);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
//--------------------------------------------------------------------------------
#endif // MEMORIES_HPP
