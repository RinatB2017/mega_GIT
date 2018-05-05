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
*   FILENAME            : image2D.cpp
*
*   DESCRIPTION         : A wrapper library for OpenCL and its native counter part
*                         intialization. With boost thread support.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 25th Feb 2014
*
*   File Created.
*
* =============================================================================
*/
#include "image2D.h"

/**
 * @brief
 *
 * @param mem
 * @param rowStep Width/Column * Num of channels
 * @param queue
 */
iv::Image2D::Image2D(cl_mem mem, cl_command_queue *queue, int rowStep)
    :Buffer(mem, queue)
{
    //Needed to copy the right amount of data back to the memory
    DEBUG_VALUE("Image2D::Image2D Constructor: ", mem);
    this->_rowPitch = rowStep;
}

void iv::Image2D::read(void *hostMem, const size_t origin[], const size_t region[], cl_bool blocking)
{
    DEBUG_STRING("Image2D::read");
    DEBUG_VALUE("memory :",_memory);
    cl_int err = 0;
    DEBUG_STRING("Image Region:");
    DEBUG_VALUE("Width :" , region[0]);
    DEBUG_VALUE("Height :", region[1]);
    DEBUG_VALUE("Depth :" , region[2]);
    DEBUG_VALUE("Row pitch is: ", _rowPitch);

    DEBUG_STRING("Image Origin:");
    DEBUG_VALUE("Width :" , origin[0]);
    DEBUG_VALUE("Height :", origin[1]);
    DEBUG_VALUE("Depth :" , origin[2]);

    if(blocking)
        DEBUG_STRING("BLOCKING READ");

    err = clEnqueueReadImage(*_pQueue, _memory, blocking,
                             origin, region, _rowPitch, 0,
                             hostMem, 0, NULL, NULL);

    DEBUG_CL(err);
}


/**
 * @brief
 *
 * @param hostMem
 * @param size[]  region
 * @param offset[] origin
 * @param blocking
 */
void iv::Image2D:: write(void *hostMem, const size_t origin[], const size_t region[], cl_bool blocking)
{
    cl_int err = 0;
    DEBUG_STRING("Image2D:: write");
    DEBUG_VALUE("Image2D _memory write:",_memory);
    err = clEnqueueWriteImage(*_pQueue, _memory, blocking, origin,
                              region, _rowPitch, 0,
                              hostMem, 0, NULL, NULL);
    DEBUG_CL(err);
}


void *iv::Image2D::map(cl_map_flags flags, const size_t size[], const size_t offset[], size_t &_rowPitch, cl_bool blocking)
{
    size_t slicePitch;
    cl_int err = 0;
    void* ret = clEnqueueMapImage(*_pQueue, _memory, blocking, flags, offset, size, &_rowPitch, &slicePitch, 0, NULL, NULL, &err);
    DEBUG_CL(err);
    return ret;
}


void iv::Image2D::copyToBuffer(iv::Buffer &dst, const size_t size[], const size_t srcOffset[], const size_t dstOffset)
{
    cl_int err = 0;
    err = clEnqueueCopyImageToBuffer(*_pQueue, _memory, dst.getMem(), srcOffset, size, dstOffset, 0, NULL, NULL);
    DEBUG_CL(err);
}


void *iv::Image2D::getInfo(const cl_image_info paramName)
{
    cl_int err = 0;
    size_t size;
    err = clGetImageInfo (_memory, paramName, 0, NULL, &size);
    DEBUG_CL(err);

    if(size > 0) {
        void* info = malloc(size);
        err = clGetImageInfo (_memory, paramName, size, info, &size);
        DEBUG_CL(err);
        return info;
    }
    else return NULL;
}
