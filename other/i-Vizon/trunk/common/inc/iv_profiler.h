/*
****************************************************************************
BSD License
Copyright (c) 2014, i-Vizon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the i-Vizon.
4. Neither the name of the i-Vizon nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Mageswaran.D ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Mageswaran.D BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************
*/
/*
* =============================================================================
*
*
*   FILENAME            : iv_profiler.h
*
*   DESCRIPTION         : To find the time elapsed between operations
*
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 16th May 2014
*
*   File Created.
*
* =============================================================================
*/
#ifndef IV_PROFILER_H
#define IV_PROFILER_H

#include <time.h>
#include <math.h>

namespace iv {

class Profiler
{
public:
    Profiler();
    void startProfiler();
    void endProfiler();
    double elapsedTimeInNanosecs();
    double elapsedTimeInSecs();

    void startCpuProfiler();
    void endCpuProfiler();

    void startGpuProfiler();
    void endGpuProfiler();

    double calculateEfficiency();

private:
protected:
    ///! TODO: Consider moving arttributes to src file
    ///         to reduce the size
    struct timespec _startTime;
    struct timespec _endTime;

    struct timespec _startTimeCpu;
    struct timespec _endTimeCpu;
    struct timespec _startTimeGpu;
    struct timespec _endTimeGpu;

    double _cpuProfiling;
    double _gpuProfiling;
    double _profileDifference;
    double _efficiency;
};


} //end of namespace
#endif
