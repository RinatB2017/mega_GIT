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
#include "creatortoolbars.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
CreatorToolBars::CreatorToolBars(QWidget *parent) :
    CreatorWindow(parent)
{
#ifndef NO_TOOLBAR
    init();
#endif
}
//--------------------------------------------------------------------------------
CreatorToolBars::~CreatorToolBars()
{
    if(toolbar) delete toolbar;
}
//--------------------------------------------------------------------------------
void CreatorToolBars::init(void)
{
    toolbar = new QToolBar("toolbar", this);
    Q_ASSERT(toolbar);

    toolbar->setObjectName("toolbar");
    toolbar->setMovable(true);

    addToolBar(Qt::TopToolBarArea, toolbar);

#ifndef NO_TOOLBAR_BUTTON_EXIT
    app_toolbar_add_exit();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif

#ifndef NO_TOOLBAR_BUTTON_FONT
#ifndef NO_LOG
    app_toolbar_add_font();
#endif
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif

#ifndef ONLY_ENGLISH
#ifndef NO_TOOLBAR_BUTTON_LANG
    app_toolbar_add_lang();
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif
#endif

#ifndef NO_TOOLBAR_BUTTON_STYLE
    app_toolbar_add_style();
#ifdef USE_CUSTOM_STYLE
    app_toolbar_add_custom_style();
#endif
#ifndef NO_TOOLBAR_SEPARATORS
    app_toolbar_add_separator();
#endif
#endif

#ifndef NO_TOOLBAR_BUTTON_ABOUT
    app_toolbar_add_about();
#endif
#ifndef NO_TOOLBAR_BUTTON_HELP
    app_toolbar_add_help();
#endif

#ifdef TOOLBAR_ORIENTATION
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(toolbar, &QToolBar::orientationChanged, [this]()
    {
        switch(toolbar->orientation())
        {
        case Qt::Horizontal:
            toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            break;

        case Qt::Vertical:
            toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            break;
        }
    });
#endif
}
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_separator(void)
{
    Q_ASSERT(toolbar);
    toolbar->addSeparator();
}
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_exit(void)
{
    QToolButton *btnExit = new QToolButton(this);
    btnExit->setObjectName("btnExit");
    btnExit->setIcon(QIcon(P_ICON_EXIT));
    btnExit->setToolTip("Exit");
    btnExit->setStatusTip("Exit");
    btnExit->setProperty(P_APP_ENG_TEXT, "Exit");
    connect(btnExit,    &QToolButton::clicked,  this,   &CreatorWindow::close);

    toolbar->addWidget(btnExit);
    app_buttons.append(btnExit);
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void CreatorToolBars::app_toolbar_add_font(void)
{
    QMenu *menu_fonts = new QMenu();

    QAction *a_app_font = new QAction(this);
    a_app_font->setText("Select the font program");
    a_app_font->setToolTip("Select the font program");
    a_app_font->setStatusTip("Select the font program");
    a_app_font->setProperty(P_APP_ENG_TEXT, "Select the font program");
    connect(a_app_font, &QAction::triggered,  this,   &CreatorWindow::set_app_font);

    QAction *a_log_font = new QAction(this);
    a_log_font->setText("Select the font logging");
    a_log_font->setToolTip("Select the font logging");
    a_log_font->setStatusTip("Select the font logging");
    a_log_font->setProperty(P_APP_ENG_TEXT, "Select the font logging");
    connect(a_log_font, &QAction::triggered,  this,   &CreatorWindow::set_log_font);

    menu_fonts->addAction(a_app_font);
    menu_fonts->addAction(a_log_font);

    QToolButton *btn_font = new QToolButton(this);
    btn_font->setObjectName("btn_font");
    btn_font->setMenu(menu_fonts);
    btn_font->setIcon(QIcon(P_ICON_FONT));
    btn_font->setPopupMode(QToolButton::MenuButtonPopup);

    toolbar->addWidget(btn_font);

    app_buttons.append(btn_font);
    app_actions.append(a_app_font);
    app_actions.append(a_log_font);
}
#endif
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_lang(void)
{
    QMenu *menu = new QMenu();

    QString lang = "English";

    QAction *a_us = new QAction(this);
    a_us->setObjectName("a_us");
    a_us->setIcon(QIcon(P_ICON_US));
    a_us->setToolTip(lang);
    a_us->setStatusTip(lang);
    a_us->setProperty(P_APP_ENG_TEXT, lang);
    a_us->setProperty(P_LANG, P_US);
    a_us->setText(lang);
    connect(a_us,  &QAction::triggered,    this,   &CreatorWindow::setToolBarLanguage);
    app_actions.append(a_us);
    menu->addAction(a_us);

    foreach (TRANSLATOR translator, l_translators)
    {
        QAction *a_lang = new QAction(this);
        lang = translator.language;
        a_lang->setProperty(P_APP_ENG_TEXT, lang);
        a_lang->setText(lang);
        a_lang->setToolTip(lang);
        a_lang->setStatusTip(lang);
        a_lang->setIcon(QIcon(translator.icon_name));
        a_lang->setProperty(P_LANG, translator.property);
        connect(a_lang, &QAction::triggered,
                this,   &CreatorWindow::setMenuLanguage);
        app_actions.append(a_lang);
        menu->addAction(a_lang);
    }

    QToolButton *btn_lang = new QToolButton(this);
    btn_lang->setMenu(menu);
    btn_lang->setIcon(QIcon(P_ICON_LANG));
    btn_lang->setPopupMode(QToolButton::MenuButtonPopup);

    toolbar->addWidget(btn_lang);
    app_buttons.append(btn_lang);
}
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_style(void)
{
    QMenu *menu = new QMenu(this);

    QStringList sl;
    sl.clear();
    sl.append(QStyleFactory::keys());
    foreach (QString style, sl)
    {
        QAction *temp = new QAction(style, menu);
        menu->addAction(temp);
        connect(temp, &QAction::triggered, this, &CreatorWindow::setStyles);
    }

    QToolButton *btnStyle = new QToolButton(this);
    btnStyle->setObjectName("btnStyle");
    btnStyle->setIcon(QIcon(P_ICON_STYLE));
    btnStyle->setToolTip("Style");
    btnStyle->setStatusTip("Style");
    btnStyle->setProperty(P_APP_ENG_TEXT, "Style");
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);
    connect(btnStyle,    &QToolButton::clicked,  this,   &CreatorWindow::close);

    toolbar->addWidget(btnStyle);
    app_buttons.append(btnStyle);
}
//--------------------------------------------------------------------------------
#ifdef USE_CUSTOM_STYLE
void CreatorToolBars::app_toolbar_add_custom_style(void)
{
    //TODO пробую самописные стили

    QMenu *menu = new QMenu(this);

    QAction *a_norton = new QAction("Norton", menu);
    menu->addAction(a_norton);

    QToolButton *btnStyle = new QToolButton(this);
    btnStyle->setObjectName("btn_norton");
    btnStyle->setIcon(QIcon(P_ICON_STYLE));
    btnStyle->setMenu(menu);
    btnStyle->setPopupMode(QToolButton::InstantPopup);

    connect(a_norton,   &QAction::triggered,    this,   &CreatorWindow::set_norton_style);

    toolbar->addWidget(btnStyle);
}
#endif
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_about(void)
{
    QToolButton *btnAbout = new QToolButton(this);
    btnAbout->setObjectName("btnExit");
    btnAbout->setIcon(QIcon(ICON_PROGRAMM));
    btnAbout->setToolTip("About");
    btnAbout->setStatusTip("About");
    btnAbout->setProperty(P_APP_ENG_TEXT, "About");
    connect(btnAbout,    &QToolButton::clicked,  this,   &CreatorWindow::about);

    toolbar->addWidget(btnAbout);
    app_buttons.append(btnAbout);
}
//--------------------------------------------------------------------------------
void CreatorToolBars::app_toolbar_add_help(void)
{
    QToolButton *btnHelp = new QToolButton(this);
    btnHelp->setObjectName("btnHelp");
    btnHelp->setIcon(QIcon(P_ICON_HELP));
    btnHelp->setToolTip("Help");
    btnHelp->setStatusTip("Help");
    btnHelp->setProperty(P_APP_ENG_TEXT, "Help");
    btnHelp->setShortcut(Qt::Key_F1);
    connect(btnHelp,    &QToolButton::clicked,  this,   &CreatorWindow::help);

    toolbar->addWidget(btnHelp);
    app_buttons.append(btnHelp);
}
//--------------------------------------------------------------------------------
void CreatorToolBars::app_updateText(void)
{
    foreach (QAbstractButton *btn, app_buttons)
    {
        QString text = tr(btn->property(P_APP_ENG_TEXT).toString().toLocal8Bit());
        btn->setText(text);
        btn->setToolTip(text);
        btn->setStatusTip(text);
    }
    foreach (QAction *action, app_actions)
    {
        QString text = tr(action->property(P_APP_ENG_TEXT).toString().toLocal8Bit());
        action->setText(text);
        action->setToolTip(text);
        action->setStatusTip(text);
    }
}
//--------------------------------------------------------------------------------
void CreatorToolBars::changeEvent(QEvent *event)
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
}
//--------------------------------------------------------------------------------
