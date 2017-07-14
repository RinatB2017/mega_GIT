#include "panel.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
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

Panel::Panel(QWidget *parent) :
    QToolBar(parent)
{
    color = "#353535";
    setOrientation(Qt::Vertical);
    setFixedWidth(300);

    ttl = new QLabel;
    ttl->setObjectName("Title");
    ttl->setText(tr("Title"));
    addWidget(ttl);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    addWidget(scrollArea);

    setColor(color);

    btnClose = new QPushButton(QIcon(":/img/close.png"),"",this);
    btnClose->setObjectName("TopBtn");
    btnClose->setIconSize(QSize(20,20));
    btnClose->move(this->geometry().width()-btnClose->geometry().width(),0);
    connect(btnClose,SIGNAL(clicked()),this,SIGNAL(closeRequested()));

    shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(25);
    shadow->setOffset(3,0);
    shadow->setColor(QColor(0,0,0,125));
    this->setGraphicsEffect(shadow);
}

void Panel::resizeEvent(QResizeEvent *event)
{
    btnClose->move(this->geometry().width()-btnClose->geometry().width(),0);
    event->accept();
}

void Panel::setTitle(QString title)
{
    ttl->setText(title);
}

void Panel::setWidget(QWidget *widget)
{
    widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    scrollArea->setWidget(widget);
    //scrollArea->update();
}

void Panel::setLayout(QLayout *boxLayout)
{/*
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    widget->setLayout(boxLayout);
    scrollArea->setWidget(widget);
    scrollArea->update();*/

    scrollArea->setLayout(boxLayout);
}

void Panel::enableShadow(bool enable)
{
    if(enable)
        shadow->setColor(QColor(0,0,0,125));
    else
        shadow->setColor(QColor(0,0,0,0));
}

void Panel::setColor(QString backgroundColor)
{
    color = backgroundColor;
    QColor color2;
    color2.setNamedColor(color);
    // augmenter en rgb+20

    this->setStyleSheet("\
                  QToolBar {\
                      border: none;\
                      background: " + color + ";\
                      padding: 0px;\
                  }\
                  QLabel {\
                      margin-bottom: 15px;\
                      color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                    #Title {\
                        border: none;\
                        padding: 10px;\
                        padding-bottom: 15px;\
                        margin: 0px;\
                        font-size: 18px;\
                        font-weight: lighter;\
                        background: rgba(0,0,0,0);\
                    }\
                  QToolButton, QPushButton {\
                      border: none;\
                      background: none;\
                      padding: 5px; color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                      margin-bottom: 0px;\
                  }\
                  QToolButton:hover, QPushButton:hover {\
                      background: rgba(0,0,0,.2);\
                  }\
                  QToolButton:pressed, QPushButton:pressed {\
                      background: rgba(0,0,0,.5);\
                  }\
                  QPushButton::menu-indicator {\
                      image: url(downarrow.png);\
                  }\
                  #TopBtn {\
                      min-height: 40px;\
                      max-width: 40px;\
                  }\
                  QProgressBar {\
                      border: 4px solid white;\
                      background: white;\
                      border-radius: 14px;\
                      }\
                  QProgressBar::chunk {\
                      border: none;\
                      background: " + color + ";\
                      border-radius: 10px;\
                  }\
                  QLineEdit {\
                      margin: 1px;\
                      margin-bottom: 10px;\
                      border: 1px solid rgba(0,0,0,.5);\
                      background: rgba(0,0,0,.2);\
                      color: white;\
                      padding: 4px 8px;\
                      font-size: 14px;\
                      border-radius: 4px;\
                      font-family: segoe ui;\
                  }\
                  QLineEdit::focus {\
                      border: 1px solid rgba(0,0,0,1);\
                      background: rgba(0,0,0,.5);\
                  }\
                  QTextEdit {\
                      margin-bottom: 10px;\
                      border: none;\
                      background: transparent;\
                      color: white;\
                      padding: 6px 0px;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  QScrollArea {\
                      background: transparent;\
                      border: none;\
                  }\
                  QCheckBox,QRadioButton {\
                      spacing: 10px;\
                      color: white;\
                      font-size: 14px;\
                      font-family: segoe ui;\
                  }\
                  QCheckBox::indicator,QRadioButton::indicator {\
                      width: 15px;\
                      height: 15px;\
                      border: 1px solid rgba(0,0,0,.5);\
                      background: rgba(0,0,0,.3);\
                      padding: 5px;\
                      border-radius: 5px;\
                  }\
                  QRadioButton::indicator {\
                      border-radius: 13px;\
                      width: 15px;\
                      height: 15px;\
                      padding: 5px;\
                  }\
                  QCheckBox::indicator:hover,QRadioButton::indicator:hover {\
                      border-color: rgba(0,0,0,.7);\
                      background: rgba(0,0,0,.5);\
                  }\
                  QCheckBox::indicator:checked {\
                      image: url(:/img/close.png);\
                  }\
                  QRadioButton::indicator:checked {\
                      image: url(:/img/round.png);\
                  }");
}
