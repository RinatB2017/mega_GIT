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
//--------------------------------------------------------------------------------
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public QWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = 0);
    ~MainBox();

protected:
    void changeEvent(QEvent *e);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void send(const QString &);

public slots:

private slots:
    void log(const QString &data);
    void test(void);

private:
    Ui::MainBox *ui;
    QWidget *parent;
    void init(void);
    void connect_log(void);
    void createTestBar(void);

    void init_thread(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
