#include "commandbar.h"

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

CommandBar::CommandBar(QWidget *parent) :
    QFrame(parent)
{
    textEdit = new QLineEdit;
    btnExecCmd = new QToolButton;
    btnGoToWeb = new QToolButton;
    btnSearchWeb = new QToolButton;
    btnSearchWord = new QToolButton;
    btnClose = new QPushButton(this);
    helpMessage = new QTextEdit;

    helpMessage->setText(tr("<center>Type a word or an internet address and tap \"Enter\".</center>"));
    helpMessage->setReadOnly(true);

    btnClose->setIcon(QIcon(":/img/close.png"));
    btnClose->setIconSize(QSize(20,20));
    btnClose->move(0,0);
    btnClose->setFixedSize(50,50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(btnSearchWeb);
    layout->addWidget(btnGoToWeb);
    layout->addWidget(btnSearchWord);
    layout->addWidget(btnExecCmd);
    layout->addWidget(helpMessage);
    layout->setContentsMargins(40,40,40,40);
    layout->setSpacing(0);
    setLayout(layout);

    btnExecCmd->setText(tr("Run the command"));
    btnGoToWeb->setText(tr("Go to the website"));
    btnSearchWeb->setText(tr("Search on the web"));
    btnSearchWord->setText(tr("Search on the page"));

    btnExecCmd->setCheckable(true);
    btnGoToWeb->setCheckable(true);
    btnSearchWeb->setCheckable(true);
    btnSearchWord->setCheckable(true);

    btnExecCmd->setIcon(QIcon(":/img/cmd.png"));
    btnGoToWeb->setIcon(QIcon(":/img/file.png"));
    btnSearchWeb->setIcon(QIcon(":/img/search.png"));
    btnSearchWord->setIcon(QIcon(":/img/word.png"));

    btnExecCmd->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnGoToWeb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnSearchWeb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnSearchWord->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btnExecCmd->hide();
    btnGoToWeb->hide();

    connect(textEdit,SIGNAL(returnPressed()),this,SLOT(finishedEditing()));
    connect(btnGoToWeb,SIGNAL(clicked()),this,SIGNAL(goToWeb()));
    connect(btnSearchWeb,SIGNAL(clicked()),this,SIGNAL(searchWeb()));
    connect(btnClose,SIGNAL(clicked()),this,SIGNAL(closeRequested()));

    int maxWidth(400);
    int minWidth(200);

    for(int i(0);i<layout->count();i++)
    {
        layout->itemAt(i)->widget()->setMaximumWidth(maxWidth);
        layout->itemAt(i)->widget()->setMinimumWidth(minWidth);
        layout->itemAt(i)->widget()->setFixedWidth(maxWidth);

        layout->itemAt(i)->widget()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
        layout->setAlignment(layout->itemAt(i)->widget(),Qt::AlignHCenter);
    }

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget(spacer);

    connect(textEdit,SIGNAL(textEdited(QString)),this,SLOT(textEdited(QString)));
    setColor("rgb(150,150,150)");

    shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(25);
    shadow->setOffset(0,3);
    shadow->setColor(QColor(0,0,0,125));
    this->setGraphicsEffect(shadow);
}

void CommandBar::setFocus()
{
    textEdit->setFocus();
}

void CommandBar::enableShadow(bool enable)
{
    if(enable)
        shadow->setColor(QColor(0,0,0,125));
    else
        shadow->setColor(QColor(0,0,0,0));
}

void CommandBar::textEdited(QString text)
{
    btnExecCmd->hide();
    btnGoToWeb->hide();

    btnGoToWeb->setChecked(false);
    btnSearchWeb->setChecked(true);
    btnSearchWord->setChecked(false);
    btnExecCmd->setChecked(false);

    if(text.left(7)=="http://" || text.left(8)=="https://" || text.left(9)=="localhost" || text.contains("."))
    {
        btnGoToWeb->setVisible(true);
        btnGoToWeb->setChecked(true);

        btnSearchWeb->setChecked(false);
        btnSearchWord->setChecked(false);
        btnExecCmd->setChecked(false);
    }
    if(text.left(2)==":>")
        btnExecCmd->setVisible(true);

    if(text.isEmpty())
        reset();
    if(!text.isEmpty())
    {
        btnSearchWeb->setVisible(true);
        helpMessage->hide();
    }
}

void CommandBar::finishedEditing()
{
    if(btnSearchWeb->isChecked())
        emit searchWeb();
    else if(btnGoToWeb->isChecked())
        emit goToWeb();
    else if(btnSearchWord)
        emit searchWord();
}

void CommandBar::reset()
{
    textEdit->clear();

    btnExecCmd->setChecked(false);
    btnGoToWeb->setChecked(false);
    btnSearchWeb->setChecked(false);
    btnSearchWord->setChecked(false);

    btnExecCmd->hide();
    btnGoToWeb->hide();
    btnSearchWeb->hide();
    btnSearchWord->hide();
    helpMessage->show();
}

void CommandBar::resizeEvent(QResizeEvent *event)
{
    int margin(0);
    btnClose->move(this->geometry().width()-(margin+btnClose->geometry().width()),margin);

    event->accept();
}

void CommandBar::keyPressEvent(QKeyEvent *event)
{
   /* if(event->key() == Qt::Key_Down)
    {
        if(textEdit->hasFocus())
        {
            btnSearchWeb->setFocus();
            btnSearchWeb->setChecked(true);

            btnExecCmd->setChecked(false);
            btnGoToWeb->setChecked(false);
            btnSearchWord->setChecked(false);
        }
        else if(btnSearchWeb->isChecked())
        {
            if(btnGoToWeb->isVisible())
            {
                btnGoToWeb->setFocus();
                btnGoToWeb->setChecked(true);

                btnSearchWord->setChecked(false);
            }
            else
            {
                btnSearchWord->setFocus();
                btnSearchWord->setChecked(true);

                btnGoToWeb->setChecked(false);
            }

            btnExecCmd->setChecked(false);
            btnSearchWeb->setChecked(false);
        }
    }*/

    event->accept();
}

void CommandBar::setColor(QString color)
{
    QColor color2;
    color2.setNamedColor(color);

    this->setStyleSheet("\
                        QFrame {\
                            border: none;\
                            background: " + color + ";\
                            }\
                        QTextEdit {\
                            font-family: segoe ui;\
                            font-size: 14px;\
                            color: white;\
                            margin-top: 10px;\
                            background: transparent;\
                        }\
                        QFrame QLineEdit {\
                            border: 1px solid rgba(255,255,255,1);\
                            background: rgba(0,0,0,0);\
                            color: white;\
                            border-radius: 19px;\
                            padding: 7px 15px;\
                            font-family: segoe ui;\
                            font-size: 16px;\
                            margin-bottom: 10px;\
                            }\
                        QFrame QLineEdit::focus {\
                            border-color: rgba(0,0,0,.5);\
                            background: rgba(0,0,0,.2);\
                            }\
                        QFrame QToolButton {\
                            border: none;\
                            background: transparent;\
                            color: white;\
                            padding: 6px 6px 7px 6px;\
                            font-family: segoe ui;\
                            font-size: 14px;\
                            text-align: left;\
                            margin-bottom: 5px;\
                            }\
                        QFrame QToolButton:hover, QFrame QToolButton:checked {\
                            background: rgba(0,0,0,.2);\
                            }\
                        QFrame QPushButton {\
                            border: none;\
                            background: rgba(0,0,0,0);\
                        }\
                        QFrame QPushButton:hover {\
                            border: none;\
                            background: rgba(0,0,0,.2);\
                            }");
}
