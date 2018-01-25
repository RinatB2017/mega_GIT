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
#ifndef PETEVENTCMP_HPP
#define PETEVENTCMP_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
class PETEventCmp : public QObject
{
    Q_OBJECT

public:
    explicit PETEventCmp(double cmpWindow,
                         double timeScale,
                         uint intNum,
                         char sourceA,
                         char sourceB,
            QObject *parent = nullptr);

    long    *Points(void);
    double  CmpWindow(void);
    void    ProcEvent(pet_event petEvent);
    double  Mean(void);
    double  Variance(void);

    char mSourceA = 0;
    char mSourceB = 0;

    long EventTotal = 0;
    long CmpNum = 0;

    long tmp = 0;

    double tsA = 0;
    double tsB = 0;

    uint MaxIx = 0;

    int32_t CorrectionA = 0;
    int32_t CorrectionB = 0;

private:
    long    *mPoints = 0;
    long    mMax = 0;
    double  mCmpWindow = 0;
    double  mTimeScale = 0;
    double  mStep = 0;
    uint    mIntNum = 0;
};
//--------------------------------------------------------------------------------
#endif // PETEVENTCMP_HPP
