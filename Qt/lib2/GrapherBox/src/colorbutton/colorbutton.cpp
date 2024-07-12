/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "colorbutton.hpp"
//--------------------------------------------------------------------------------
ColorButton::ColorButton(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void ColorButton::setColor(QColor new_color)
{
    color = new_color;
    int R = color.red();
    int G = color.green();
    int B = color.blue();
    btn_color->setStyleSheet(QString("background:#%1%2%3")
                             .arg(R, 2, 16, QChar('0'))
                             .arg(G, 2, 16, QChar('0'))
                             .arg(B, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
QColor ColorButton::getColor(void)
{
    return color;
}
//--------------------------------------------------------------------------------
void ColorButton::setText(const QString &new_text)
{
    text = new_text;
    btn_text->setText(text);
}
//--------------------------------------------------------------------------------
QString ColorButton::getText(void)
{
    return text;
}
//--------------------------------------------------------------------------------
bool ColorButton::isCheckable(void)
{
    return btn_text->isCheckable();
}
//--------------------------------------------------------------------------------
void ColorButton::setCheckable(bool state)
{
    btn_text->setCheckable(state);
}
//--------------------------------------------------------------------------------
ColorButton::~ColorButton()
{

}
//--------------------------------------------------------------------------------
void ColorButton::init(void)
{
    btn_text = new QPushButton(this);
    btn_color = new QToolButton(this);

    QHBoxLayout *box = new QHBoxLayout();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    box->setMargin(0);
#endif
    box->addWidget(btn_color);
    box->addWidget(btn_text);

    setColor(Qt::black);
    setLayout(box);

    connect(btn_color,  &QToolButton::clicked, this, &ColorButton::set_color);
    connect(btn_text,   &QToolButton::clicked, this, &ColorButton::set_text);
}
//--------------------------------------------------------------------------------
void ColorButton::set_color(void)
{
    QColorDialog *dlg = new QColorDialog();
    dlg->setCurrentColor(color);

    int btn = dlg->exec();
    if(btn == QColorDialog::Accepted)
    {
        color = dlg->selectedColor();

        int R = color.red();
        int G = color.green();
        int B = color.blue();

        btn_color->setStyleSheet(QString("background:#%1%2%3")
                                 .arg(R, 2, 16, QChar('0'))
                                 .arg(G, 2, 16, QChar('0'))
                                 .arg(B, 2, 16, QChar('0')));
    }
}
//--------------------------------------------------------------------------------
void ColorButton::set_text(void)
{
    bool ok = false;
    QString new_text = QInputDialog::getText(this,
                                             tr("Curve text"),
                                             tr("Text:"),
                                             QLineEdit::Normal,
                                             text,
                                             &ok);
    if (ok && !new_text.isEmpty())
    {
        text = new_text;
        btn_text->setText(text);
    }
}
//--------------------------------------------------------------------------------
