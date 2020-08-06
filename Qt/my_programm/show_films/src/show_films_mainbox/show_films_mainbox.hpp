/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class TreeFilms;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = nullptr);
    ~MainBox();

signals:
    void double_click(void);

private slots:
    void show_picture(const QString &data);
    void change_player(void);
    void save(void);
    void select_last_film(void);

private:
    Ui::MainBox *ui;
    QWidget *parent;

    QPixmap current_picture;
    QString current_picture_name;

    QAction *change_player_action;
    QAction *save_action;
    QAction *last_film;

    void init(void);

    QPointer<TreeFilms> tree_films;
    QPointer<QLabel> lblFilmPicture;
    QPointer<QVBoxLayout> picture_vbox;
    QPointer<QHBoxLayout> picture_hbox;

    void init_tree_widget(void);

    void load_translations(void);
    void create_menu(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
