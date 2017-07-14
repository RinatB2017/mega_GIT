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
#include "neuron.hpp"
//--------------------------------------------------------------------------------
Neuron::Neuron(QObject *parent) : QObject(parent)
{

}
//--------------------------------------------------------------------------------
// получить имя

QString Neuron::get_name(void)
{
    return name;
}
//--------------------------------------------------------------------------------
// очистить память нейрона и присвоить ему новое имя

void Neuron::clear(QString name, int x, int y)
{
    this->name = name;
    weight.clear();
    for(int n=0; n<y; n++)
    {
        for(int m=0; m<x; m++)
        {
            arr a;
            a.m = m;
            a.n = n;
            weight.append(a);
        }
    }
}
//--------------------------------------------------------------------------------
// функция возвращает сумму величин отклонения входного массива от эталонного
// другими словами чем результат ближе к 1це тем больше похож входной массив
// на образ из памяти нейрона

double Neuron::get_res(QList<arr> data)
{

}
//--------------------------------------------------------------------------------
// добавить входной образ в память массива

int Neuron::training(QList<arr> data)
{

}
//--------------------------------------------------------------------------------
