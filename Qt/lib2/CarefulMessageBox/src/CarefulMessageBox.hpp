/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef CAREFULMESSAGEBOX_HPP
#define CAREFULMESSAGEBOX_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
#define MAX_BUTTONS 5
//--------------------------------------------------------------------------------
class QPushButton;
//--------------------------------------------------------------------------------
class CarefulMessageBox : public QDialog
{
    Q_OBJECT
public:
    CarefulMessageBox(const QString &caption,
                      const QString &message,
                      QWidget *parent = nullptr);
    
signals:
    
private slots:
    void click(void);
    
private:
    QPushButton *buttons[MAX_BUTTONS];
    int true_button;
};
//--------------------------------------------------------------------------------
#endif // CAREFULMESSAGEBOX_HPP
