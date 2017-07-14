/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef DATABASE_HPP
#define DATABASE_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_td.h"
#include "pet_sim.h"
#include "pet_brg_sim.h"
//--------------------------------------------------------------------------------
#include "qttreepropertybrowser.h"
#include "qtvariantproperty.h"
//--------------------------------------------------------------------------------
typedef struct StructProperty
{
    QString name;
    QString object_name;
    int propertyType;
    QtVariantProperty *item;
} StructProperty_t;
//--------------------------------------------------------------------------------
namespace Ui {
    class Database;
}
//--------------------------------------------------------------------------------
class Database : public MyWidget
{
    Q_OBJECT

public:
    explicit Database(QWidget *parent = 0);
    ~Database();

    PET_RESULT set(pet_discovery_info data);
    PET_RESULT set(pet_device_info data);
    PET_RESULT set(pet_update_data data);
    PET_RESULT set(pet_wave_point data);
    PET_RESULT set(pet_event data);
    PET_RESULT set(pet_bos_ctrl data);
    PET_RESULT set(pet_bos_state data);
    PET_RESULT set(pet_bos_ch_ctrl data);
    PET_RESULT set(pet_bos_ch_state data);
    PET_RESULT set(pet_brg_ctrl data);
    PET_RESULT set(pet_brg_state data);
    PET_RESULT set(pet_brg_ch_ctrl data);
    PET_RESULT set(pet_brg_ch_state data);
    PET_RESULT set(pet_brg_ch_calibration data);
    //PET_RESULT set(pet_brg_calibration data);

    PET_RESULT get(pet_discovery_info *data);
    PET_RESULT get(pet_device_info *data);
    PET_RESULT get(pet_update_data *data);
    PET_RESULT get(pet_wave_point *data);
    PET_RESULT get(pet_event *data);
    PET_RESULT get(pet_bos_ctrl *data);
    PET_RESULT get(pet_bos_state *data);
    PET_RESULT get(pet_bos_ch_ctrl *data);
    PET_RESULT get(pet_bos_ch_state *data);
    PET_RESULT get(pet_brg_ctrl *data);
    PET_RESULT get(pet_brg_state *data);
    PET_RESULT get(pet_brg_ch_ctrl *data);
    PET_RESULT get(pet_brg_ch_state *data);
    PET_RESULT get(pet_brg_ch_calibration *data);
    //PET_RESULT get(pet_brg_calibration *data);

    void add_property(QtVariantPropertyManager *variantManager,
                      QList<StructProperty_t>  *l_property,
                      QString name,
                      QString object_name,
                      int propertyType);

private:
    Ui::Database *ui = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // DATABASE_HPP
