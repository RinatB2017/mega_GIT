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
#ifndef CREATORTOOLBARS_HPP
#define CREATORTOOLBARS_HPP
//--------------------------------------------------------------------------------
#include "creatorwindow.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class CreatorToolBars : public CreatorWindow
{
    Q_OBJECT

public:
    explicit CreatorToolBars(CreatorWindow *parent);
    ~CreatorToolBars();

private:
    QPointer<QToolBar> toolbar;
    QList<QAbstractButton *> app_buttons;
    QList<QAction *> app_actions;

    void init(void);

    void app_toolbar_add_separator(void);
    void app_toolbar_add_exit(void);
#ifndef NO_LOG
    void app_toolbar_add_font(void);
#endif
    void app_toolbar_add_lang(void);
    void app_toolbar_add_style(void);
#ifdef USE_CUSTOM_STYLE
    void app_toolbar_add_custom_style(void);
#endif
    void app_toolbar_add_about(void);
    void app_toolbar_add_help(void);

    void app_updateText(void);

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // CREATORTOOLBARS_HPP
