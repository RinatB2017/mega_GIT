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
*   FILENAME            : Buffer.h
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
#include "buffer.h"
namespace iv
{

iv::Buffer::Buffer(cl_mem tmp, cl_command_queue *queue)
{
    this->_memory = tmp;
    this->_pQueue = queue;
}

cl_mem& Buffer::getMem()
{
    return _memory;
}

void Buffer::read(void *hostMem, const size_t size, const size_t offset, const cl_bool blocking)
{
    cl_int status = 0;
    status = clEnqueueReadBuffer(*_pQueue, _memory, blocking, offset, size, hostMem, 0, NULL, NULL);
    DEBUG_CL(status);
}

void Buffer::write(const void *hostMem, const size_t size, const size_t offset, const cl_bool blocking)
{
    cl_int status = 0;
    status = clEnqueueWriteBuffer(*_pQueue, _memory, blocking, offset, size, hostMem, 0, NULL, NULL);
    DEBUG_CL(status);
}

void Buffer::copy(Buffer &dst, const size_t size, const size_t srcOffset, const size_t dstOffset)
{

}

void *Buffer::map(const cl_map_flags flags, const size_t size, const size_t offset, const cl_bool blocking)
{

}

void Buffer::unmap(void *mappedPtr)
{

}

void Buffer::copyToImage2D(Image2D &dst, const size_t size[], const size_t srcOffset, const size_t dstOffset[])
{

}

void Buffer::copyToImage3D(Image2D &dst, const size_t size[], const size_t srcOffset, const size_t dstOffset[])
{

}

} //end of namespace
