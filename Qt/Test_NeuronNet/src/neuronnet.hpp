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
#ifndef NEURONNET_HPP
#define NEURONNET_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "neuron.hpp"
//--------------------------------------------------------------------------------
class NeuronNet : public QObject
{
    Q_OBJECT
public:
    explicit NeuronNet(QObject *parent = nullptr);

    QList<Neuron> init_web(void);
    Neuron neuron_create(void);
    QString check_litera(QList<arr> arr);
    void save_state(void);
    QStringList get_literas(void);
    void set_training(QString training_name, QList<arr> data);

private:
};
//--------------------------------------------------------------------------------
#endif // NEURONNET_HPP
