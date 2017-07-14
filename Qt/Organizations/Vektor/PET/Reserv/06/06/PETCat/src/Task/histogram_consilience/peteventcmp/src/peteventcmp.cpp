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
#include "peteventcmp.hpp"
//--------------------------------------------------------------------------------
PETEventCmp::PETEventCmp(double cmpWindow,
                         double timeScale,
                         uint intNum,
                         char sourceA,
                         char sourceB,
                         QObject *parent) : QObject(parent)
{
    mPoints = new long[intNum];
    mSourceA = sourceA;
    mSourceB = sourceB;
    mCmpWindow = cmpWindow;
    mTimeScale = timeScale;

    mStep = (double)cmpWindow / ((double)intNum - 1.0);
    mIntNum = intNum;
}
//--------------------------------------------------------------------------------
long * PETEventCmp::Points(void)
{
    return mPoints;
}
//--------------------------------------------------------------------------------
double PETEventCmp::CmpWindow(void)
{
    return mCmpWindow;
}
//--------------------------------------------------------------------------------
void PETEventCmp::ProcEvent(pet_event petEvent)
{
    double ts = mTimeScale * ((double)petEvent.ts + (double)petEvent.ts_fract / (double)UINT16_MAX);

    if (petEvent.src == mSourceA)
    {
        tsA = ts;
    }
    else if (petEvent.src == mSourceB)
    {
        tsB = ts;
    }
    else return;

    int ix = (int)(((sizeof(mPoints) - 1.0) / 2.0) + (tsB - tsA) / mStep);

    if (ix >= 0 && ix < (int)sizeof(mPoints))
    {
        if (++mPoints[ix] > mMax)
        {
            mMax++;
            MaxIx = (uint)ix;
        }
        CmpNum++;
    }

    EventTotal++;
}
//--------------------------------------------------------------------------------
double PETEventCmp::Mean(void)
{
    double mean = 0;
    //double p;
    //double timeStep = 1.0;
    //double delta = CmpWindow * timeStep / (Points.Length - 1);
    //double start = -1.0 * CmpWindow * timeStep / 2;

    for (unsigned int i = 0; i < mIntNum; i++)
    {
        mean += (double)(i - (sizeof(mPoints) - 1) / 2) * (double)mPoints[i] / (double)CmpNum;
    }
    return mean;
}
//--------------------------------------------------------------------------------
double PETEventCmp::Variance(void)
{
    double mean2 = 0;

    for (unsigned int i = 0; i < sizeof(mPoints); i++)
    {
        mean2 += qPow((double)(i - (sizeof(mPoints) - 1) / 2), 2) * (double)mPoints[i] / (double)CmpNum;
    }
    return qSqrt(mean2 - qPow(Mean(), 2));
}
//--------------------------------------------------------------------------------
