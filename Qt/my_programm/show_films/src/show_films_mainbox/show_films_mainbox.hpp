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
#include <QFileDialog>
#include <QPointer>
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
    virtual ~MainBox();

signals:
    void double_click();

private slots:
    void show_picture(const QString &data);
    void change_player();
    void save();
    void select_last_film();

private:
    Ui::MainBox *ui;

    QPixmap current_picture;
    QString current_picture_name;

    QPointer<QAction> change_player_action;
    QPointer<QAction> save_action;
    QPointer<QAction> last_film;

    void init();

    QPointer<TreeFilms> tree_films;
    QPointer<QLabel> lblFilmPicture;
    QPointer<QVBoxLayout> picture_vbox;
    QPointer<QHBoxLayout> picture_hbox;

    void init_tree_widget();

    void load_translations();
    void create_menu();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
