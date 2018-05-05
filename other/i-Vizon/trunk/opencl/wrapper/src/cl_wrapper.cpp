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
*   FILENAME            : CLWrapper.h
*
*   DESCRIPTION         : A wrapper library for OpenCL and its native counter part
*                         intialization. With boost thread support.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 22th Feb 2014
*
*   File Created.
*
* =============================================================================
*/

#include <cl_wrapper.hpp>

namespace iv {


void CLSetup::init()
{
    getPlatformID();
    getDeviceID();
    getContextnQueue();
}

void CLSetup::getPlatformID()
{
    _status = clGetPlatformIDs(1, NULL, &_numPlatforms);
    DEBUG_CL(_status);
    _status =clGetPlatformIDs(1, &_platformID, NULL);
    DEBUG_CL(_status);

    /// !TODO: Multiple Platforms
    //    cl_platform_id* _platformID;
    //    _status = clGetPlatformIDs(NUMBER_OF_PLATFORMS, NULL, &_numPlatforms);
    //    DEBUG_CL(_status);
    //    _platformID = (cl_platform_id *)malloc(sizeof(cl_platform_id) * _numPlatforms);
    //    _status =clGetPlatformIDs(_numPlatforms, _platformID, NULL);
    //    DEBUG_CL(_status);
    //    _platformIDsVector.assign(_platformID[0], _platformID[_numPlatforms]);
}

void CLSetup::getDeviceID()
{
    /// !TODO: For Multiple Devices
    _status = clGetDeviceIDs(_platformID,CL_DEVICE_TYPE_GPU, 1, NULL, &_numDevices);
    DEBUG_CL(_status);
    _status = clGetDeviceIDs(_platformID, CL_DEVICE_TYPE_GPU, 1, &_deviceID, NULL);
    DEBUG_CL(_status);

    // Getting some information about the device
    // Getting some information about the device
    clGetDeviceInfo(_deviceID, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &_maxComputeUnits, NULL);
    std::cout<<"\nCL_DEVICE_MAX_COMPUTE_UNITS: "<<_maxComputeUnits<<std::endl;
    clGetDeviceInfo(_deviceID, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &_maxWorkGroupSize, NULL);
    clGetDeviceInfo(_deviceID, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &_maxMemAllocSize, NULL);
    clGetDeviceInfo(_deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &_globalMemSize, NULL);
    clGetDeviceInfo(_deviceID, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &_constMemSize, NULL);
    clGetDeviceInfo(_deviceID, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &_localMemSize, NULL);
    ///!TODO:Add get KernelInfo APIs

}

void CLSetup::getContextnQueue()
{
    _context = clCreateContext(NULL, 1, &_deviceID, NULL, NULL, &_status);
    DEBUG_CL(_status);
    _queue = clCreateCommandQueue(_context, _deviceID, NULL, &_status);
    DEBUG_CL(_status);
}

///
/// \brief CLSetup::createProgram
/// \param kernelFilePath
/// \return
///
Program *CLSetup::createProgram(std::vector<std::string> kernelFilePath)
//Program *CLSetup::createProgram(std::string& kernelFilePath)
{
    ///!TODO: Add support for char** along with string
    Program* tmp = new Program(kernelFilePath, &_context, &_queue,
                               &_deviceID);
    return tmp;
}

Buffer* CLSetup::createBuffer(const size_t size, const cl_mem_flags flags,
                              void *hostMem)
{
    cl_mem buff = clCreateBuffer(_context,flags, size, hostMem ,&_status);
    if(_status == CL_SUCCESS)
    {
        Buffer* ret = new Buffer(buff, &_queue);
        return ret;
    }
    DEBUG_CL(_status);
    return NULL; //TODO: Return custom status value
}

/// @TIPS: Don't give step value for CL_MEM_WRITE_ONLY
Image2D *CLSetup::createImage2D(const size_t width, const size_t height, const cl_image_format *format,
                                const cl_mem_flags flags, const size_t rowPitch, void *hostMem)
{
    cl_int err = 0;
    im2d = clCreateImage2D(_context,
                           flags,format,
                           width, height,
                           rowPitch,
                           hostMem, &_status);
    DEBUG_STRING("createImage2D");
    DEBUG_VALUE("Image2D im2d create:",im2d);
    DEBUG_CL(_status);
    Image2D* ret = new Image2D(im2d, &_queue, rowPitch);
    return ret;
}

Sampler* CLSetup::createSampler(cl_bool normalizedCoords,
                                cl_addressing_mode addrMode,
                                cl_filter_mode filterMode)
{
    cl_int err =0;
    Sampler* ret = new  Sampler(&_context, normalizedCoords,
                                addrMode, filterMode);
    return ret;
}


} // end of iv namespace



















