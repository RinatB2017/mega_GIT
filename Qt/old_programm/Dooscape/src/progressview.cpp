#include "progressview.h"

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

ProgressView::ProgressView(QWidget *parent) :
    QPushButton(parent)
{
    lblValue = new QLabel;
    lblValue->setText("0%");

    progress = new QProgressBar;
    progress->setValue(24);
    progress->setFixedHeight(5);
    progress->setTextVisible(false);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(10,10,10,10);
    layout->addWidget(lblValue,0,0);
   // layout->setAlignment(lblValue,Qt::AlignCenter);
    layout->addWidget(progress,1,0);
    setLayout(layout);
    setFixedSize(200,60);
    setFixedSize(200,80);

    setFixedSize(300,55);

    //setTitle(tr("Load in progress"));

    connect(this,SIGNAL(clicked()),this,SIGNAL(closeRequested()));

    setColor("rgba(50,50,50,1)");
}

void ProgressView::setValue(int value)
{
    progress->setValue(value);
    lblValue->setText(QString::number(value)+"%");
}

void ProgressView::setColor(QString color)
{
    setStyleSheet("\
                  QLabel {\
                      font-size: 25px;\
                      margin: 0px;\
                      margin-bottom: 7px;\
                      margin-left: -2px;\
                      font-family: segoe ui light, segoe ui;\
                      color: white;\
                      }\
                  QPushButton {\
                      border-top-right-radius: 7px;\
                      background: " + color + ";\
                      border: 1px solid rgba(0,0,0,.5);\
                      border-bottom: none; border-left: none;\
                      }\
                  QProgressBar {\
                      background: rgba(0,0,0,.3);\
                      border: none;\
                      margin: 0px;\
                      }\
                  QProgressBar::chunk {\
                      border: none;\
                      background: white;\
                      }");
}
