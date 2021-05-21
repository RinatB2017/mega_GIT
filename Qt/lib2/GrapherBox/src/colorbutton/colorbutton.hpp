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
#ifndef COLORBUTTON_HPP
#define COLORBUTTON_HPP
//--------------------------------------------------------------------------------
#include <QColorDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPointer>
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class ColorButton;
}
//--------------------------------------------------------------------------------
class ColorButton : public QWidget
{
    Q_OBJECT

public:
    explicit ColorButton(QWidget *parent = nullptr);
    virtual ~ColorButton();

    void setColor(QColor new_color);
    QColor getColor(void);

    void setText(const QString &new_text);
    QString getText(void);

    bool isCheckable(void);
    void setCheckable(bool state);

private slots:
    void set_color(void);
    void set_text(void);

private:
    QPointer<QPushButton> btn_text;
    QPointer<QToolButton> btn_color;
    QColor color;
    QString text;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // COLORBUTTON_HPP
