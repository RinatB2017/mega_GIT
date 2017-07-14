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
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QXmlGet;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void test(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QPixmap p;
    int x = 0;
    int y = 0;
    int picture_width = 0;
    int picture_height = 0;
    int step = 0;

    void init(void);

    bool get_int(QXmlGet *xml, QString name, int *data);

    bool load_wall(QPixmap *pixmap);
    bool load_field(QPixmap *pixmap);
    void load_labirint(void);
    void load_images(void);

    void move_left(void);
    void move_right(void);
    void move_up(void);
    void move_down(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

protected:
    virtual void changeEvent(QEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent (QPaintEvent * event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
