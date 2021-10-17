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
//#include "for_tests_mainbox.hpp"
#include "creatormenus.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
CreatorMenus::CreatorMenus(QWidget *parent) :
    CreatorToolBars(parent)
{
#ifndef NO_MENU
    init();
#endif
}
//--------------------------------------------------------------------------------
CreatorMenus::~CreatorMenus()
{

}
//--------------------------------------------------------------------------------
void CreatorMenus::init(void)
{
    app_mainBar = menuBar();

    m_app_filemenu    = new QMenu(app_mainBar);
    m_app_optionsmenu = new QMenu(app_mainBar);
//    m_app_windowsmenu = new QMenu(app_mainBar);
    m_app_helpmenu    = new QMenu(app_mainBar);

    m_app_filemenu->setTitle("File");
    m_app_filemenu->setProperty(P_APP_ENG_TEXT,      "File");

    m_app_optionsmenu->setTitle("Settings");
    m_app_optionsmenu->setProperty(P_APP_ENG_TEXT,   "Settings");

//    m_app_windowsmenu->setTitle("Windows");
//    m_app_windowsmenu->setProperty(P_APP_ENG_TEXT,   "Windows");

    m_app_helpmenu->setTitle("Help");
    m_app_helpmenu->setProperty(P_APP_ENG_TEXT,      "Help");

    QTimer::singleShot(0, [this]{
        app_mainBar->addMenu(m_app_filemenu);
        app_mainBar->addMenu(m_app_optionsmenu);
//        app_mainBar->addMenu(m_app_windowsmenu);
        app_mainBar->addMenu(m_app_helpmenu);
    });

    app_menus.append(m_app_filemenu);
    app_menus.append(m_app_optionsmenu);
//    app_menus.append(m_app_windowsmenu);
    app_menus.append(m_app_helpmenu);

    app_menu_add_exit(m_app_filemenu);

    app_menu_add_fonts(m_app_optionsmenu);

    app_menu_add_separator(m_app_optionsmenu);
    app_menu_add_theme(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#ifndef ONLY_ENGLISH
    app_menu_add_lang(m_app_optionsmenu);
#endif
    app_menu_add_style(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);

#ifdef USE_CUSTOM_STYLE
    app_menu_add_custom_style(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);
#endif

    app_menu_add_show_on_top(m_app_optionsmenu);
    app_menu_add_separator(m_app_optionsmenu);

    app_menu_add_confirm_exit(m_app_optionsmenu);

    app_menu_add_about(m_app_helpmenu);
    app_menu_add_help(m_app_helpmenu);

//#ifndef NO_TOOLBAR
//#ifndef NO_MENU
//    add_windowsmenu_action(toolbar, toolbar->toggleViewAction());
//#endif
//#endif
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_menu(int pos_x,
                            QMenu *menu)
{
    Q_ASSERT(menu);

    QList<QAction *> menus = app_mainBar->actions();
    if(menus.count() == 0)
    {
        emit error("add_menu: menu is empty!");
        return false;
    }

    if(pos_x >= menus.count())
    {
        app_mainBar->addMenu(menu);
        return true;
    }

    int pos = 0;
    foreach (QAction *current_menu, menus)
    {
        if(pos == pos_x)
        {
            app_mainBar->insertMenu(current_menu, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_filemenu_menu(int pos_y,
                                     QMenu *menu)
{
    Q_ASSERT(m_app_filemenu);
    Q_ASSERT(menu);

    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_filemenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertSeparator(current_action);
            m_app_filemenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_optionsmenu_menu(int pos_y,
                                        QMenu *menu)
{
    Q_ASSERT(m_app_filemenu);
    Q_ASSERT(menu);

    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_optionsmenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertSeparator(current_action);
            m_app_optionsmenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_helpmenu_menu(int pos_y,
                                     QMenu *menu)
{
    Q_ASSERT(m_app_filemenu);
    Q_ASSERT(menu);

    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_helpmenu_menu: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertSeparator(current_action);
            m_app_helpmenu->insertMenu(current_action, menu);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_filemenu_action(int pos_y,
                                       QAction *action)
{
    Q_ASSERT(m_app_filemenu);
    Q_ASSERT(action);

    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_filemenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertAction(current_action, action);
            app_actions.append(action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_optionsmenu_action(int pos_y,
                                          QAction *action)
{
    Q_ASSERT(m_app_optionsmenu);
    Q_ASSERT(action);

    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_optionsmenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertAction(current_action, action);
            app_actions.append(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_helpmenu_action(int pos_y,
                                       QAction *action)
{
    Q_ASSERT(m_app_helpmenu);
    Q_ASSERT(action);

    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_helpmenu_action: actions.count() == 0!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertAction(current_action, action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_filemenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_filemenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_filemenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_optionsmenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_optionsmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_optionsmenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
//bool CreatorMenus::add_windowsmenu_action(QWidget *widget, QAction *action)
//{
//    Q_ASSERT(widget);
//    Q_ASSERT(action);
//    Q_ASSERT(m_app_windowsmenu);

//    m_app_windowsmenu->addAction(action);
//    return true;
//}
//--------------------------------------------------------------------------------
bool CreatorMenus::add_helpmenu_separator(int pos_y)
{
    QList<QAction *> actions = m_app_helpmenu->actions();
    if(actions.count() == 0)
    {
        emit error("add_separator: menu is empty!");
        return false;
    }

    int pos = 0;
    foreach (QAction *current_action, actions)
    {
        if(pos == pos_y)
        {
            m_app_helpmenu->insertSeparator(current_action);
            return true;
        }
        pos++;
    }

    return false;
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_separator(QMenu *menu)
{
    Q_ASSERT(menu);
    menu->addSeparator();
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_exit(QMenu *menu)
{
    Q_ASSERT(menu);

    QAction *exit = new QAction(menu);
    exit->setProperty(P_APP_ENG_TEXT, "Exit");
    exit->setText("Exit");
    exit->setToolTip("Exit");
    exit->setStatusTip("Exit");
    exit->setIcon(QIcon(P_ICON_EXIT));
    connect(exit,   &QAction::triggered,    this,   &CreatorWindow::close);

    app_actions.append(exit);

    menu->addAction(exit);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_fonts(QMenu *menu)
{
    Q_ASSERT(menu);

    //---
    QMenu *m_fonts = new QMenu(menu);
    m_fonts->setProperty(P_APP_ENG_TEXT, "Fonts");
    m_fonts->setTitle("Fonts");
    m_fonts->setToolTip("Fonts");
    m_fonts->setStatusTip("Fonts");
    m_fonts->setIcon(QIcon(P_ICON_FONT));
    menu->addMenu(m_fonts);
    app_menus.append(m_fonts);

    QAction *a_font_programm = new QAction(m_fonts);
    a_font_programm->setProperty(P_APP_ENG_TEXT, "Select the font program");
    a_font_programm->setText("Select the font program");
    a_font_programm->setToolTip("Select the font program");
    a_font_programm->setStatusTip("Select the font program");
    m_fonts->addAction(a_font_programm);
    connect(a_font_programm,    &QAction::triggered,    this,   &CreatorWindow::set_app_font);
    app_actions.append(a_font_programm);

#ifndef NO_LOG
    QAction *a_font_log = new QAction(m_fonts);
    a_font_log->setProperty(P_APP_ENG_TEXT, "Select the font logging");
    a_font_log->setText("Select the font logging");
    a_font_log->setToolTip("Select the font logging");
    a_font_log->setStatusTip("Select the font logging");
    m_fonts->addAction(a_font_log);
    connect(a_font_log, &QAction::triggered,    this,   &CreatorWindow::set_log_font);
    app_actions.append(a_font_log);
#endif
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_theme(QMenu *menu)
{
    Q_ASSERT(menu);

    QMenu *menu_theme = new QMenu(menu);
    menu_theme->setProperty(P_APP_ENG_TEXT, "Themes");
    menu_theme->setTitle("Themes");
    menu_theme->setToolTip("Themes");
    menu_theme->setStatusTip("Themes");
    menu->addMenu(menu_theme);
    app_menus.append(menu_theme);

    QAction *system_theme = new QAction(menu_theme);
    system_theme->setProperty(P_APP_ENG_TEXT, "System theme");
    system_theme->setText("System theme");
    system_theme->setToolTip("System theme");
    system_theme->setStatusTip("System theme");
    connect(system_theme,   &QAction::triggered,    this,   &CreatorWindow::set_system_palette);
    menu_theme->addAction(system_theme);
    app_actions.append(system_theme);

    QAction *light_theme = new QAction(menu_theme);
    light_theme->setProperty(P_APP_ENG_TEXT, "Light theme");
    light_theme->setText("Light theme");
    light_theme->setToolTip("Light theme");
    light_theme->setStatusTip("Light theme");
    connect(light_theme,   &QAction::triggered,    this,   &CreatorWindow::set_light_palette);
    menu_theme->addAction(light_theme);
    app_actions.append(light_theme);

    QAction *dark_theme = new QAction(menu_theme);
    dark_theme->setProperty(P_APP_ENG_TEXT, "Dark theme");
    dark_theme->setText("Dark theme");
    dark_theme->setToolTip("Dark theme");
    dark_theme->setStatusTip("Dark theme");
    connect(dark_theme,   &QAction::triggered,    this,   &CreatorWindow::set_dark_palette);
    menu_theme->addAction(dark_theme);
    app_actions.append(dark_theme);

    QAction *blue_theme = new QAction(menu_theme);
    blue_theme->setProperty(P_APP_ENG_TEXT, "Blue theme");
    blue_theme->setText("Blue theme");
    blue_theme->setToolTip("Blue theme");
    blue_theme->setStatusTip("Blue theme");
    connect(blue_theme,   &QAction::triggered,    this,   &CreatorWindow::set_blue_palette);
    menu_theme->addAction(blue_theme);
    app_actions.append(blue_theme);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_lang(QMenu *menu)
{
    Q_ASSERT(menu);

    QMenu *menu_language = new QMenu(menu);
    menu_language->setProperty(P_APP_ENG_TEXT, "Language");
    menu_language->setTitle("Language");
    menu_language->setToolTip("Language");
    menu_language->setStatusTip("Language");
    menu_language->setIcon(QIcon(P_ICON_LANG));
    menu->addMenu(menu_language);
    app_menus.append(menu_language);

    QAction *a_us = new QAction(menu_language);
    QString lang = "English";
    a_us->setProperty(P_APP_ENG_TEXT, lang);
    a_us->setText(lang);
    a_us->setToolTip(lang);
    a_us->setStatusTip(lang);
    a_us->setIcon(QIcon(P_ICON_US));
    a_us->setProperty(P_LANG, P_US);
    connect(a_us,   &QAction::triggered,    this,   &CreatorWindow::setMenuLanguage);
    menu_language->addAction(a_us);
    app_actions.append(a_us);

    foreach (TRANSLATOR translator, l_translators)
    {
        QAction *a_lang = new QAction(menu_language);
        lang = translator.language;
        a_lang->setProperty(P_APP_ENG_TEXT, lang);
        a_lang->setText(lang);
        a_lang->setToolTip(lang);
        a_lang->setStatusTip(lang);
        a_lang->setIcon(QIcon(translator.icon_name));
        a_lang->setProperty(P_LANG, translator.property);
        connect(a_lang, &QAction::triggered,
                this,   &CreatorWindow::setMenuLanguage);
        menu_language->addAction(a_lang);
        app_actions.append(a_lang);
    }
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_style(QMenu *menu)
{
    Q_ASSERT(menu);

    QMenu *menu_style = new QMenu(menu);
    menu_style->setProperty(P_APP_ENG_TEXT, "Style");
    menu_style->setTitle("Style");
    menu_style->setToolTip("Style");
    menu_style->setStatusTip("Style");
    menu_style->setIcon(QIcon(P_ICON_STYLE));
    menu->addMenu(menu_style);
    app_menus.append(menu_style);

    QStringList sl;
    sl.clear();
    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *temp = new QAction(style, menu_style);
        temp->setIcon(QIcon(P_ICON_STYLE));
        menu_style->addAction(temp);
        connect(temp,   &QAction::triggered,    this,   &CreatorWindow::setStyles);
    }
}
//--------------------------------------------------------------------------------
#ifdef USE_CUSTOM_STYLE
void CreatorMenus::app_menu_add_custom_style(QMenu *menu)
{
    //TODO пробую самописные стили

    Q_ASSERT(menu);

    QMenu *menu_style = new QMenu(menu);
    menu_style->setProperty(P_APP_ENG_TEXT, "Custom style");
    menu_style->setTitle("Custom style");
    menu_style->setToolTip("Custom style");
    menu_style->setStatusTip("Custom style");
    menu_style->setIcon(QIcon(P_ICON_STYLE));
    menu->addMenu(menu_style);
    app_menus.append(menu_style);

    QAction *temp = new QAction("Norton", menu_style);
    temp->setIcon(QIcon(P_ICON_STYLE));

    menu_style->addAction(temp);
    connect(temp,   &QAction::triggered,    this,   &CreatorWindow::set_norton_style);
}
#endif
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_confirm_exit(QMenu *menu)
{
    Q_ASSERT(menu);

    QAction *exit = new QAction(menu);
    exit->setProperty(P_APP_ENG_TEXT, "Do not ask when you exit");
    exit->setText("Do not ask when you exit");
    exit->setToolTip("Do not ask when you exit");
    exit->setStatusTip("Do not ask when you exit");
    exit->setCheckable(true);
    connect(exit,   &QAction::triggered,    this,   &CreatorWindow::closeOnExit);
    exit->setChecked(flag_close);

    app_actions.append(exit);

    menu->addAction(exit);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_show_on_top(QMenu *menu)
{
    Q_ASSERT(menu);

    QAction *on_top = new QAction(menu);
    on_top->setProperty(P_APP_ENG_TEXT, "Always on top");
    on_top->setText("Always on top");
    on_top->setToolTip("Always on top");
    on_top->setStatusTip("Always on top");
    on_top->setCheckable(true);
    connect(on_top,   &QAction::triggered,    this,   &CreatorWindow::alwaysOnTop);
    on_top->setChecked(flag_always_on_top);

    app_actions.append(on_top);

    menu->addAction(on_top);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_about(QMenu *menu)
{
    Q_ASSERT(menu);

    QAction *about = new QAction(menu);
    about->setProperty(P_APP_ENG_TEXT, "About");
    about->setText("About");
    about->setToolTip("About");
    about->setStatusTip("About");
    about->setIcon(QIcon(ICON_PROGRAMM));
    connect(about,   &QAction::triggered,    this,   &CreatorWindow::about);

    app_actions.append(about);

    menu->addAction(about);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_menu_add_help(QMenu *menu)
{
    Q_ASSERT(menu);

    QAction *help = new QAction(menu);
    help->setProperty(P_APP_ENG_TEXT, "Help");
    help->setText("Help");
    help->setToolTip("Help");
    help->setStatusTip("Help");
    help->setIcon(QIcon(P_ICON_HELP));
    connect(help,   &QAction::triggered,    this,   &CreatorWindow::help);

    app_actions.append(help);

    menu->addAction(help);
}
//--------------------------------------------------------------------------------
void CreatorMenus::app_updateText(void)
{
    foreach (auto menu, app_menus)
    {
        menu->setTitle(tr(menu->property(P_APP_ENG_TEXT).toString().toLocal8Bit()));
    }
    foreach (auto action, app_actions)
    {
        QString text = tr(action->property(P_APP_ENG_TEXT).toString().toLocal8Bit());
        action->setText(text);
        action->setToolTip(text);
        action->setStatusTip(text);
    }
}
//--------------------------------------------------------------------------------
void CreatorMenus::changeEvent(QEvent *event)
{
    if(event == nullptr)
    {
        return;
    }

    switch (event->type())
    {
    case QEvent::LanguageChange:
        app_updateText();
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
    CreatorToolBars::changeEvent(event);
}
//--------------------------------------------------------------------------------
