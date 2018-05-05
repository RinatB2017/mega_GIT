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
*   FILENAME            : image2D.h
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
#ifndef IMAGE_2D_H
#define IMAGE_2D_H

#include <CL/cl.h>
#include "buffer.h"
namespace iv
{
/**
 * @brief
 *
 */
class Image2D : public Buffer
{
public:
    /**
 * @brief
 *
 * @param mem
 * @param queue
 * @param rowPitch
 */
    Image2D(cl_mem mem, cl_command_queue* queue, int rowPitch = 0);
    /**
         * @brief
         *
         * @param hostMem
         * @param size[]
         * @param offset[]
         * @param blocking
         */
    void read(void* hostMem, const size_t size[2],
    const size_t offset[2], cl_bool blocking = CL_TRUE);
    /**
         * @brief
         *
         * @param hostMem
         * @param size[]
         * @param offset[]
         * @param blocking
         */
    void write(void* hostMem, const size_t size[2],
    const size_t offset[2], cl_bool blocking = CL_TRUE);
    /**
         * @brief
         *
         * @param flags
         * @param size[]
         * @param offset[]
         * @param rowPitch
         * @param blocking
         */
    void* map(cl_map_flags flags, const size_t size[2],
    const size_t offset[2], size_t& rowPitch, cl_bool blocking = CL_TRUE);
    /**
         * @brief
         *
         * @param dst
         * @param size[]
         * @param srcOffset[]
         * @param dstOffset
         */
    void copyToBuffer(Buffer& dst, const size_t size[2],
    const size_t srcOffset[2], const size_t dstOffset = 0);
    /**
         * @brief
         *
         * @param paramName
         */
    void* getInfo(const cl_image_info paramName);
    /**
         * @brief
         *
         */
    ~Image2D()
    {

    }

protected:
    size_t _rowPitch;

private:
};


}//end of namespace

#endif //IMAGE_2D_H
