/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

    void up(void);
    void down(void);
    void left(void);
    void right(void);

    void update(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    void init(void);

    void createTestBar(void);

    QGraphicsPixmapItem *item;
    QGraphicsRectItem *rect_0;
    QTimer *timer;
    qreal pos_x = 0;
    qreal pos_y = 0;
    qreal delta = 0.01;

    qreal min_x = 0;
    qreal max_x = 0;
    qreal min_y = 0;
    qreal max_y = 0;

    bool flag_up = false;
    bool flag_down = false;
    bool flag_left = false;
    bool flag_right = false;

    QGraphicsScene *scene_0;
    QGraphicsScene *scene_1;
    QGraphicsScene *scene_2;

    void create_scene_0(void);
    void create_scene_1(void);
    void create_scene_2(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *, QEvent *e);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
