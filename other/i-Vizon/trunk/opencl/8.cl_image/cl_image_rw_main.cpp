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
*   FILENAME            : cl_image_rw_main.cpp
*
*   DESCRIPTION         : A test code to integrate the OpenCV Mat object and CL
*                         image data type, with only CL Khoronos C++  wrappers
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 10th April 2014
*
*   File Created.
*
* =============================================================================
*/

// Use cl::vector instead of STL version
#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

//To print entry and exit of functions
#define F_LOG LogBlock _l(__func__)
struct LogBlock {
    const char *mLine;
    LogBlock(const char *line) : mLine(line) {
        std::cout<<mLine <<"  -----> Enter \n";
    }
    ~LogBlock() {
        std::cout<<mLine <<" <----- Leave \n";
    }
};

// CL Platform Setup
cl::vector<cl::Platform> platforms;
cl::vector<cl::Device> devices;
cl_context_properties cps[3];
cl::Context context;
cl::CommandQueue queue;

//CL Kernel Setup
cl::Program::Sources source;
cl::Program program;
cl::vector<cl::Kernel> kernels;

//CL Error values enquiry
std::string parameterName;
cl_long parameterValue;

//CV objects
cv::Mat jpegOriginal;
cv::Mat jpegBGRA;
cv::Mat jpegGRAY;

/**
 * @brief CLRuntime::get_error_string Return coresponding error String
 * @param err: Error Number
 * @return
 */
const char * get_error_string(cl_int err)
{
    F_LOG;
    switch(err)
    {
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_ISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_Program_FAILURE";
    case -12: return "CL_MAP_FAILURE";

    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_Context";
    case -35: return "CL_INVALID_Queue_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_mQueue";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_Program";
    case -45: return "CL_INVALID_Program_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_Buffer_SIZE";
    case -62: return "CL_INVALID_IP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    default: return "Unknown OpenCL error";
    }
}

/**
 * @brief CLRuntime::getImgType // take number
 *        image type number (from cv::Mat.type()), get OpenCV's enum string.
 * @param imgTypeInt
 * @return
 */
std::string getImgType(int imgTypeInt)
{
    int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

    int enum_Ints[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
                             CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
                             CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
                             CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
                             CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
                             CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
                             CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

    std::string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
                                  "CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
                                  "CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
                                  "CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
                                  "CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
                                  "CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
                                  "CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

    for(int i=0; i<numImgTypes; i++)
    {
        if(imgTypeInt == enum_Ints[i]) return enum_strings[i];
    }
    return "unknown image type";
}


/**
 * @brief Reads the .cl file and creates the kernels out of it.
 * @param kernelFileName
 */
void createKernel(std::string kernelFileName)
{
    F_LOG;
    try
    {
        std::ifstream programFile(kernelFileName.c_str());
        std::string programString(std::istreambuf_iterator<char>(programFile),
                                  (std::istreambuf_iterator<char>()));
        source = cl::Program::Sources(1, std::make_pair(programString.c_str(),
                                                        programString.length()+1));

        program = cl::Program(context, source);
        program.build(devices);
        program.createKernels(&kernels);

        if(1)
        {
            for(int _I=0; _I<kernels.size(); _I++)
            {
                parameterName = kernels[_I].getInfo<CL_KERNEL_FUNCTION_NAME>();
                std::cout << "Kernel: " << parameterName << std::endl;
            }
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;

    }
}

int main()
{

    try
    {
        cl::Platform::get(&platforms);
        platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
        //cps[3] = (CL_CONTEXT_PLATFORM, (cl_context_properties) (platform[0])(), 0);
        context = cl::Context(devices);
        queue = cl::CommandQueue(context, devices[0]);
        createKernel("imgRW.cl");
        ///@TIPS: Always load image in size multiples of 4
        jpegOriginal = cv::imread("shiva.jpg");
        cv::imshow("Original File", jpegOriginal);
        std::cout<<"Total Pixels in File: "<<jpegOriginal.total()<<"\n";
        std::cout<<"Pixel Data Type :"<<jpegOriginal.type()<<"\n";
        std::cout<<"Original Step Value :"<<jpegOriginal.step<<"\n";
        std::cout<<"Element Size :"<<jpegOriginal.elemSize()<<"\n";
        std::cout<<"Num of Channels :"<<jpegOriginal.channels()<<"\n";
        std::cout<<"Num of cols (cols * channesl) :"<<jpegOriginal.cols * jpegOriginal.channels()<<"\n";
        cv::waitKey();

        int width = jpegOriginal.size().width;

        int height = jpegOriginal.size().height;

        int step = jpegOriginal.step;

        cl::NDRange globalws(width, height);
        cl::NDRange localws(4,4);
        cl::NDRange offsetws(cl::NullRange);


        cl::size_t<3> origin, region;
        origin[0] = 0; origin[1] = 0; origin[2] = 0;
        region[0] = width; region[1] = height; region[2] = 1;
        ///!TODO: width or step

        //############################################################################
        {
            cv::cvtColor(jpegOriginal, jpegBGRA, CV_BGR2BGRA);
            cv::imshow("BGRA", jpegBGRA);
            cv::waitKey();

            int step = jpegBGRA.step;
            std::cout<<"BGRA Step Value :"<<step<<"\n";

            cl::ImageFormat jpegFormat;
            jpegFormat.image_channel_order = CL_RGBA;
            jpegFormat.image_channel_data_type = CL_UNORM_INT8;

            //ArgNo: 0
            cl::Image2D src(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, jpegFormat, size_t(width), size_t(height), step, jpegBGRA.data);
            //ArgNo: 1
            //Step seems to be 0 for empty image buffer
            cl::Image2D dst(context, CL_MEM_WRITE_ONLY, jpegFormat, size_t(width), size_t(height), 0);

            cl::Sampler sampler(context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST);

            kernels[0].setArg(0, src);
            kernels[0].setArg(1, dst);

            queue.enqueueNDRangeKernel(kernels[0], offsetws, globalws, localws);

            queue.enqueueReadImage(dst, CL_TRUE, origin, region, sizeof(cl_uchar) * jpegBGRA.step, 0, jpegBGRA.data);

            cv::imshow("After CL BGRA RW", jpegBGRA);
            cv::waitKey();
        }
        //############################################################################
        {
            cv::cvtColor(jpegOriginal, jpegGRAY, CV_BGR2GRAY);
            cv::imshow("GRAY", jpegGRAY);
            cv::waitKey();

            int step = jpegGRAY.step;
            std::cout<<"GRAY Step Value :"<<step<<"\n";

            cl::ImageFormat jpegFormat;
            jpegFormat.image_channel_order = CL_R;
            jpegFormat.image_channel_data_type = CL_UNORM_INT8;

            //ArgNo: 0
            cl::Image2D src(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, jpegFormat, size_t(width), size_t(height), size_t(step), jpegGRAY.data);
            //ArgNo: 1
            cl::Image2D dst(context, CL_MEM_WRITE_ONLY, jpegFormat, size_t(width), size_t(height), size_t(0));

            cl::Sampler sampler(context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST);

            kernels[0].setArg(0, src);
            kernels[0].setArg(1, dst);



            queue.enqueueNDRangeKernel(kernels[0], offsetws, globalws, localws);


            queue.enqueueReadImage(dst, CL_TRUE, origin, region, sizeof(cl_uchar) * jpegGRAY.step, 0, jpegGRAY.data);

            cv::imshow("After CL GRAY RW", jpegGRAY);
            cv::waitKey();
        }
        //############################################################################
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;

    }
}



//kernel
//void zsobel_grayscale1(read_only image2d_t src, write_only image2d_t dst)
//{
//    int x = (int)get_global_id(0);
//    int y = (int)get_global_id(1);

//    if (x >= get_image_width(src) || y >= get_image_height(src))
//        return;

//    float4 p00 = read_imagef(src, sampler, (int2)(x - 1, y - 1));
//    float4 p10 = read_imagef(src, sampler, (int2)(x,y - 1));
//    float4 p20 = read_imagef(src, sampler, (int2)(x + 1, y - 1));
//    float4 p01 = read_imagef(src, sampler, (int2)(x - 1, y));
//    float4 p21 = read_imagef(src, sampler, (int2)(x + 1, y));
//    float4 p02 = read_imagef(src, sampler, (int2)(x - 1, y + 1));
//    float4 p12 = read_imagef(src, sampler, (int2)(x, y + 1));
//    float4 p22 = read_imagef(src, sampler, (int2)(x + 1, y + 1));

//    float3 gx = -p00.xyz + p20.xyz + 2.0f * (p21.xyz - p01.xyz)- p02.xyz + p22.xyz;
//    float3 gy = -p00.xyz - p20.xyz + 2.0f * (p12.xyz - p10.xyz) + p02.xyz + p22.xyz;

//    float gs_x = 0.3333f * (gx.x + gx.y + gx.z);
//    float gs_y = 0.3333f * (gy.x + gy.y + gy.z);

//    float g = native_sqrt(gs_x * gs_x + gs_y * gs_y);

//    write_imagef(dst, (int2)(x, y), (float4)(g, g, g, 1.0f));
//}

//__kernel
//void convolution(
//   __read_only  image2d_t  sourceImage,
//   __write_only image2d_t  outputImage,
//   int rows,
//   int cols,
//   __constant float* filter,
//   int filterWidth,
//   sampler_t sampler)
//{
//   // Store each work-item’s unique row and column
//   int column = get_global_id(0);
//   int row    = get_global_id(1);

//   // Half the width of the filter is needed for indexing
//   // memory later
//   int halfWidth = (int)(filterWidth/2);

//   // All accesses to images return data as four-element vector
//   // (i.e., float4), although only the 'x' component will contain
//   // meaningful data in this code
//   float4 sum = {0.0f, 0.0f, 0.0f, 0.0f};

//   // Iterator for the filter
//   int filterIdx = 0;

//   // Each work-item iterates around its local area based on the
//   // size of the filter
//   int2 coords;  // Coordinates for accessing the image
//   // Iterate the filter rows
//   for(int i = -halfWidth; i <= halfWidth; i++) {
//      coords.y = row + i;

//      // Iterate over the filter columns
//      for(int j = -halfWidth; j <= halfWidth; j++) {
//         coords.x = column + j;

//         float4 pixel;
//         // Read a pixel from the image.  A single channel image
//         // stores the pixel in the 'x' coordinate of the returned
//         // vector.
//         pixel = read_imagef(sourceImage, sampler, coords);
//         sum.x += pixel.x * filter[filterIdx++];
//      }
//   }

//   // Copy the data to the output image if the
//   // work-item is in bounds
//   if(row < rows && column < cols) {
//      coords.x = column;
//      coords.y = row;
//      write_imagef(outputImage, coords, sum);
//   }
//}
