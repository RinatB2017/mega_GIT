/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef CREATORMENUS_HPP
#define CREATORMENUS_HPP
//--------------------------------------------------------------------------------
#include <QMenuBar>
//--------------------------------------------------------------------------------
#include "creatortoolbars.hpp"
//--------------------------------------------------------------------------------
class CreatorMenus : public CreatorToolBars
{
    Q_OBJECT

public:
    explicit CreatorMenus(CreatorToolBars *parent = nullptr);
    virtual ~CreatorMenus();

    bool add_menu(int pos_x, QMenu *menu);
    bool add_filemenu_menu(int pos_y,
                           QMenu *menu);
    bool add_optionsmenu_menu(int pos_y,
                              QMenu *menu);
    bool add_helpmenu_menu(int pos_y,
                           QMenu *menu);

    bool add_filemenu_action(int pos_y,
                             QAction *action);
    bool add_optionsmenu_action(int pos_y,
                                QAction *action);
    bool add_helpmenu_action(int pos_y,
                             QAction *action);

    bool add_filemenu_separator(int pos_y);
    bool add_optionsmenu_separator(int pos_y);
//    bool add_windowsmenu_action(QWidget *widget, QAction *action);
    bool add_helpmenu_separator(int pos_y);

private:
    QPointer<QMenuBar> app_mainBar;

    QList<QMenu   *> app_menus;
    QList<QAction *> app_actions;

    QPointer<QMenu> m_app_filemenu;
    QPointer<QMenu> m_app_optionsmenu;
//    QPointer<QMenu> m_app_windowsmenu;
    QPointer<QMenu> m_app_helpmenu;

    void init(void);

    void app_menu_add_separator(QMenu *menu);
    void app_menu_add_exit(QMenu *menu);
    void app_menu_add_fonts(QMenu *menu);

    void app_menu_add_theme(QMenu *menu);
    void app_menu_add_lang(QMenu *menu);
    void app_menu_add_style(QMenu *menu);
#ifdef USE_CUSTOM_STYLE
    void app_menu_add_custom_style(QMenu *menu);
#endif
    void app_menu_add_confirm_exit(QMenu *menu);
    void app_menu_add_show_on_top(QMenu *menu);
    void app_menu_add_about(QMenu *menu);
    void app_menu_add_help(QMenu *menu);

    void app_updateText(void);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // CREATORMENUS
