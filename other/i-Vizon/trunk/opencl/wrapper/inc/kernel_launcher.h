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
*   FILENAME            : KernelLauncher.cpp
*
*   DESCRIPTION         : A wrapper library for OpenCL and its native counter part
*                         intialization. With boost thread support.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 17th Mar 2014
*
*   File Created.
*
* =============================================================================
*/

#ifndef KERNELLAUNCHER_H
#define KERNELLAUNCHER_H

#include <iv_common.h>
#include "CL/cl.h"

namespace iv {
class KernelLauncher
{
public:
    KernelLauncher(cl_kernel* kernel, cl_command_queue* queue);

    ///For a continuous aggignment using an object
    KernelLauncher& global(const int g);
    KernelLauncher& global(const int gx, const int gy);
    KernelLauncher& global(const int gx, const int gy, const int gz);
    KernelLauncher& local(const int l);
    KernelLauncher& local(const int lx, const int ly);
    KernelLauncher& local(const int lx, const int ly, const int lz);
    ///For a continuous aggignment using an pointer object
    KernelLauncher* pGlobal(const int g);
    KernelLauncher* pGlobal(const int gx, const int gy);
    KernelLauncher* pGlobal(const int gx, const int gy, const int gz);
    KernelLauncher* pLocal(const int l);
    KernelLauncher* pLocal(const int lx, const int ly);
    KernelLauncher* pLocal(const int lx, const int ly, const int lz);

    int countArgs();

    ///For a continuous aggignment using an object
    template<class T>
    KernelLauncher& arg(const int index, T x) {
        if (index >= _numArgs || index < 0) {
            std::cout << "Error: argument index out of range" << std::endl;
            exit(-1);///!TODO: Custom exit code
        }
        cl_int status = clSetKernelArg(*_pKernel, index, sizeof(x), &x);
        DEBUG_CL(status);
        _argListData[index] = true;
        return *this;
    }
    ///For a continuous aggignment using an pointer object
    template<class T>
    KernelLauncher& arg(T x) {
        int nArgs = countArgs();
        if (nArgs >= _numArgs) {
            std::cout << "Error trying to enqueue too much arguments" << std::endl;
            std::cout << "Expected " << _numArgs << ", got " << nArgs << std::endl;
            exit(-1);///!TODO: Custom exit code
        }
        for(int i=0; i<_numArgs; i++)
            if(!_argListData[i])
                return arg(i, x);
        return *this;
    }

    ///For a continuous aggignment using an object
    template<class T>
    KernelLauncher* pArg(const int index, T &x) {
        if (index >= _numArgs || index < 0) {
            std::cout << "Error: argument index out of range" << std::endl;
            exit(-1);///!TODO: Custom exit code
        }
        cl_int status = clSetKernelArg(*_pKernel, index, sizeof(x), &x);
        DEBUG_VALUE("Setting Kernel Argument: ", index);
        DEBUG_VALUE("Value/Address: ", x);
        DEBUG_VALUE("Size : ", sizeof(x));
        DEBUG_CL(status);
        _argListData[index] = true;
        return this;
    }
    ///For a continuous aggignment using an pointer object
    template<class T>
    KernelLauncher* pArg(T &x) {
        int nArgs = countArgs();
        if (nArgs > _numArgs) {
            std::cout << "Error trying to enqueue too much arguments" << std::endl;
            std::cout << "Expected " << _numArgs << ", got " << nArgs << std::endl;
            exit(-1);///!TODO: Custom exit code
        }
        for(int i=0; i<_numArgs; i++)
            if(!_argListData[i])
                return pArg(i, x);
        return this;
    }
    void run();

    ~KernelLauncher()
    {
        clReleaseKernel(*_pKernel);
    }

protected:
private:
    cl_kernel*          _pKernel;
    cl_command_queue*   _pQueue;
    cl_int              _numArgs;
    size_t              _globalWorkSize[3];
    size_t              _localWorkSize[3];
    cl_int              _dimensions;

    cl_bool*            _argListData;
};

}


#endif // KERNELLAUNCHER_H
