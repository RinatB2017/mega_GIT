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
#include <QModelIndex>
#include <QDomElement>
#include <QWidget>
#include <QEvent>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class QResizeEvent;
class QVBoxLayout;
class QHBoxLayout;
class TreeFilms;
class QAction;
class QEvent;
class QLabel;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
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

    TreeFilms *tree_films;
    QLabel *lblFilmPicture;
    QVBoxLayout *picture_vbox;
    QHBoxLayout *picture_hbox;

    void init_tree_widget(void);

    void load_translations(void);
    void create_menu(void);
    void updateText(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
