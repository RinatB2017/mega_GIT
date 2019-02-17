/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef PET_EVENT_FLAG_HPP
#define PET_EVENT_FLAG_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class PET_event_flag;
}
//--------------------------------------------------------------------------------
class PET_event_flag : public QDialog
{
    Q_OBJECT

public:
    explicit PET_event_flag(QWidget *parent = nullptr);
    ~PET_event_flag();

    void set(uint8_t value);
    uint8_t get(void);

private:
    Ui::PET_event_flag *ui;
};
//--------------------------------------------------------------------------------
#endif
