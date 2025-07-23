/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef MULTIEDIT_HPP
#define MULTIEDIT_HPP
//--------------------------------------------------------------------------------
#include <QTextDocument>
#include "codeeditor.hpp"
#include <QTextEdit>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MultiEdit;
}
//--------------------------------------------------------------------------------
class MultiEdit : public MyWidget
{
    Q_OBJECT

public:
    explicit MultiEdit(QWidget *parent = nullptr);
    virtual ~MultiEdit();

    void setPlainText(const QString &text);
    QString toPlainText(void) const;
    void setTabStopDistance(qreal distance);
    QTextDocument *document(void) const;

private:
    Ui::MultiEdit *ui;

    void init(void);

    CodeEditor *add_page(void);
    void rem_page(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MULTIEDIT_HPP
