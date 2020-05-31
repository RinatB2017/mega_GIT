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
#ifndef MYTEXTEDIT_HPP
#define MYTEXTEDIT_HPP
//--------------------------------------------------------------------------------
#include <QTextEdit>
//--------------------------------------------------------------------------------
class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget *parent = nullptr);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

protected slots:
    void log(const QString &data);
    void open_file(void);
    void save_file(void);

private:
    void connect_log(void);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MYTEXTEDIT_HPP
