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
#ifndef SECRETBOX_HPP
#define SECRETBOX_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class SecretBox;
}
//--------------------------------------------------------------------------------
class SecretBox : public QDialog
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    explicit SecretBox(const QString &secret, QWidget *parent = nullptr);
    ~SecretBox();

private slots:
    void click_number(void);
    void click_button(void);

private:
    Ui::SecretBox *ui;
    QString secret_word = "";
    QString input_word = "";

    void init(void);
    void show_word(void);
};
//--------------------------------------------------------------------------------
#endif // SECRETBOX_HPP
