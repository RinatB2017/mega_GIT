/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MYMAINWINDOW_HPP
#define MYMAINWINDOW_HPP
//--------------------------------------------------------------------------------
#include <QPainter>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#define COLOR   Qt::yellow
#define PEN     QPen(COLOR, 2, Qt::SolidLine)
#define ELLIPSE QPointF(width()/2, height()/2), width()/8, height()/8
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class HorizontalWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.setPen(PEN);
        p.drawLine(0, height()/2, width(), height()/2);
    }
};

class VerticalWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.setPen(PEN);
        p.drawLine(width()/2, 0, width()/2, height());
    }
};

class LeftTopWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QRectF rectangle(0, 0, width(), height());

        QPainter p(this);
        p.setPen(PEN);
        p.drawArc(rectangle, 0, -90 * 16);
        p.drawEllipse(ELLIPSE);
    }
};
class RightTopWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QRectF rectangle(0, 0, width(), height());

        QPainter p(this);
        p.setPen(PEN);
        p.drawArc(rectangle, 180 * 16, 90 * 16);
        p.drawEllipse(ELLIPSE);
    }
};

class LeftBottomWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QRectF rectangle(0, 0, width(), height());

        QPainter p(this);
        p.setPen(PEN);
        p.drawArc(rectangle, 0, 90 * 16);
        p.drawEllipse(ELLIPSE);
    }
};
class RightBottomWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        QRectF rectangle(0, 0, width(), height());

        QPainter p(this);
        p.setPen(PEN);
        p.drawArc(rectangle, 180 * 16, -90 * 16);
        p.drawEllipse(ELLIPSE);
    }
};
//--------------------------------------------------------------------------------
class MyMainWindow : public MainWindow
{
    Q_OBJECT

public:
    MyMainWindow(MainWindow *parent = 0);
    ~MyMainWindow();

    void setCentralWidget(QWidget *widget);

#ifdef HAVE_QT5
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);
#else
    static void messageHandler(QtMsgType type,
                               const char *msg);
#endif

signals:
    void notifySignal(void);

private slots:
    void file_action_click(void);
    void options_action_click(void);
    void help_action_click(void);

    void click_btn(void);

private:
    static QFile *m_logFile;
    void create_local_menus(void);
    void create_toolbars(void);

protected:
    //void closeEvent(QCloseEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MYMAINWINDOW_HPP
