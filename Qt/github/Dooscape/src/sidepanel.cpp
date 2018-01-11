#include "sidepanel.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modifyse
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

SidePanel::SidePanel(QWidget *centralWidget, QWidget *parent):
    QWidget(parent)
{
    settings = new QSettings;

    blackBar = new QToolBar;
    blackBar->setOrientation(Qt::Vertical);
    blackBar->setIconSize(QSize(18,18));

    actionsBar = new QToolBar;
    blackBar->addWidget(actionsBar);
    actionsBar->setOrientation(Qt::Vertical);
    actionsBar->setIconSize(QSize(18,18));

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    blackBar->addWidget(spacer);

    progressWidget = new ProgressIndicator;
    progressWidget->setColor(Qt::white);
    progressWidget->hide();
    progressWidget->setFixedSize(28,28);

    QVBoxLayout *progressLayout = new QVBoxLayout;
    progressLayout->setContentsMargins(10,10,10,10);
    progressLayout->addWidget(progressWidget);
    progressLayout->setAlignment(Qt::AlignCenter);

    QWidget *progressHolder = new QWidget;
    progressHolder->setLayout(progressLayout);

    blackBar->addWidget(progressHolder);

    btnMainMenu = new QToolButton;
    btnMainMenu->setText(tr("Main menu"));
    btnMainMenu->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    btnMainMenu->setPopupMode(QToolButton::MenuButtonPopup);
    blackBar->addWidget(btnMainMenu);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0,0,0,0);
    centerLayout->addWidget(blackBar);
    centerLayout->addWidget(centralWidget);
    setLayout(centerLayout);

    setColor("rgb(150,150,150)");

    QShortcut *keyShowMainMenu = new QShortcut(QKeySequence("F2"),this);
    connect(keyShowMainMenu,SIGNAL(activated()),this,SLOT(showMainMenu()));

    if(settings->value("Apparence/Animations").toBool())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(blackBar,"minimumWidth");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::OutCirc);
        animation->setStartValue(1000);
        animation->setEndValue(0);
        animation->start();
    }
}

ProgressIndicator *SidePanel::progress()
{
    return progressWidget;
}

void SidePanel::setColor(QString color)
{
    colorName = color;
    applyStyle();
}

void SidePanel::addAction(QAction *action)
{
    actionsBar->addAction(action);
}

void SidePanel::addMenu(QMenu *menu, QIcon icon)
{
    QToolButton *btnMenu = new QToolButton;
    btnMenu->setText(menu->title());
    btnMenu->setToolTip(menu->toolTip());
    btnMenu->setIcon(icon);
    btnMenu->setMenu(menu);
    btnMenu->setPopupMode(QToolButton::MenuButtonPopup);

    actionsBar->addWidget(btnMenu);
}

void SidePanel::setMainMenu(QMenu *menu, QIcon icon)
{
    btnMainMenu->setText(menu->title());
    btnMainMenu->setToolTip(menu->toolTip());
    btnMainMenu->setIcon(icon);
    btnMainMenu->setMenu(menu);
}

void SidePanel::showMainMenu()
{
   /* if(blackBar->isHidden())
    {
        blackBar->show();

        QPropertyAnimation *animation = new QPropertyAnimation(blackBar,"maximumWidth");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::InCirc);
        animation->setStartValue(0);
        animation->setEndValue(1000);
        animation->start();
    }

    QPropertyAnimation *animation = new QPropertyAnimation(blackBar,"maximumWidth");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutCirc);
    animation->setStartValue(blackBar->geometry().width());
    animation->setEndValue(0);*/

   /*// btnMainMenu->menu()->exec(QCursor::pos());

    QTimer *timer = new QTimer;
    timer->setInterval(5*1000); // 5s
    timer->start();
    connect(timer,SIGNAL(timeout()),animation,SLOT(start()));
    //animation->start();
    connect(animation,SIGNAL(finished()),blackBar,SLOT(hide()));*/

    btnMainMenu->menu()->exec(QCursor::pos());
}

void SidePanel::setToolBarVisible(bool visible)
{
    blackBar->setVisible(visible);
}

void SidePanel::clearSideBar()
{
    actionsBar->clear();
}

void SidePanel::applyStyle()
{
    QColor color2;
    color2.setNamedColor(colorName);

    qApp->setStyleSheet("\
                  QToolBar {\
                      border: none; border-right: 1px solid rgba(0,0,0,.5);\
                      background: " + color2.darker(200).name() + ";\
                      background: rgba(0,0,0,.2);\
                      padding: 0px;\
                  }\
                  QToolBar QToolBar {\
                      border: none;\
                      background: rgba(0,0,0,0);\
                      padding: 0px;\
                      }\
                  QToolBar QToolButton {\
                      border: none;\
                      padding: 15px;\
                      color: white;\
                      margin: 0px;\
                  }\
                  QToolBar QToolButton[popupMode=\"1\"]{\
                      border: none;\
                      background: transparent;\
                      width: 18px;\
                  }\
                  QToolBar QToolButton:hover {\
                      background: rgba(0,0,0,.3);\
                  }\
                  QToolBar QToolButton:pressed,QToolBar QToolButton:checked {\
                      background: rgba(0,0,0,.5);\
                  }\
                  QToolBar QToolButton::menu-button,QToolButton::menu-button:hover,QToolButton::menu-button:pressed {\
                      width: 65px;\
                      color: black;\
                      border: none;\
                      background: transparent;}\
                  QToolBar QToolButton::menu-arrow {\
                      image: url(downarrow.png);\
                  }\
                  QToolBar QLineEdit {\
                      color: white;\
                      font-size: 14px;\
                      font-size: 16px;\
                      font-family: segoe ui;\
                      border-radius: 14px;\
                      border-radius: 15px;\
                      padding: 3px 20px;\
                      padding: 3px 17px;\
                      margin-left: 5px;\
                      margin-right: 5px;\
                      border: 1px solid rgba(0,0,0,.5);\
                      background: rgba(0,0,0,.2);\
                  }\
                  QToolBar QLineEdit:focus {\
                      background: rgba(0,0,0,.5);\
                      border: 1px solid rgba(0,0,0,1);\
                  }\
                  QMenu {\
                      border: none;\
                      background: " + colorName + ";\
                      padding: 0px;\
                  }\
                  QMenu::icon {\
                      margin-left: 15px;\
                  }\
                  QMenu::item {\
                      background: transparent;\
                      border: none;\
                      min-width: 200px;\
                      padding: 9px 25px 9px 35px;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                      color: white;\
                  }\
                  QMenu::item:selected {\
                      background-color: rgba(0,0,0,.3);\
                  }\
                  QMenu::separator {\
                      margin: 0px 50px;\
                      border: none;\
                      background: white;\
                      height: 1px;\
                  }\
                  QMenu::icon:checked {\
                      background: rgba(0,0,0,.5);\
                      border-radius: 5px;\
                      border: none;\
                      padding: 9px 8px;\
                      margin-left: 0px;\
                  }\
                        QTabBar {background: rgba(0,0,0,.3);}\
                        QTabBar::tab {\
                            margin: 0px;\
                            width: 250px;\
                            height: 30px;\
                            background: rgba(255,255,255,.2);\
                            border: none;\
                            border-left: 1px solid rgba(0,0,0,.5);\
                            border-right: 1px solid rgba(0,0,0,.5);\
                            font-family: segoe ui;\
                            font-size: 14px;\
                            padding: 0px;\
                            padding-right: 5px; padding-left: 5px;\
                            color: white;\
                            height: 28px;\
                            margin-left: -1px; margin-left: 6px; margin-right: 0px;\
                        }\
                        QTabBar::tab:selected {\
                            background: rgba(255,255,255,.4);\
                            border-left: 1px solid black;\
                            border-right: 1px solid black;\
                        }\
                        QTabBar::tab:hover {\
                            background: rgba(0,0,0,.3);\
                            border-left: 1px solid black;\
                            border-right: 1px solid black;\
                        }\
                        QTabBar::close-button {image: url(\":/img/close-tab.png\");subcontrol-position: right;}\
                        QTabBar::close-button:hover {\
                            image: url(\":/img/close-tab.png\");\
                            padding: -2px -2px -2px -2px;\
                         }\
                        QTabBar::close-button:pressed {\
                            image: url(\":/img/close-tab.png\");\
                            padding: -3px -3px -3px -3px;\
                        }");
}
