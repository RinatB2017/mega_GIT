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
#ifndef FINDLINKS_HPP
#define FINDLINKS_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
class QWebFrame;
class QLineEdit;
//--------------------------------------------------------------------------------
class FindLinks : public QDialog
{
    Q_OBJECT
public:
    explicit FindLinks(QWebFrame *frame, QWidget *parent = 0);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

private:
    QWebFrame *main_frame;
    QLineEdit *le;
    QLineEdit *le2;
    void init(void);
    void connect_log(void);
    
protected:
    void accept(void);
};
//--------------------------------------------------------------------------------
#endif // FINDLINKS_HPP
