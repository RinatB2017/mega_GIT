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
#include <QtMath>
//--------------------------------------------------------------------------------
#include "gauss.hpp"
//--------------------------------------------------------------------------------
Gauss::Gauss(QObject *parent) : QObject(parent)
{

}
//--------------------------------------------------------------------------------
Gauss::Gauss(double mean, double dev)
{
    Mean = mean;
    Dev = dev;
}
//--------------------------------------------------------------------------------
double Gauss::Next(void)
{
    if (this->ready)
    {
        this->ready = false;
        return this->second * Dev + Mean;
    }
    else
    {
        double u, v, s;
        do
        {
            u = 2.0 * qrand() - 1.0;
            v = 2.0 * qrand() - 1.0;
            s = u * u + v * v;
        } while (s > 1.0 || s == 0.0);

        double r = qSqrt(-2.0 * qLn(s) / s);

        this->second = r * u;
        this->ready = true;
        return r * v * Dev + Mean;
    }
}
//--------------------------------------------------------------------------------
