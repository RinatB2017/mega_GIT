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
*   FILENAME            : test_image_Convolution.cpp
*
*   DESCRIPTION         : A test code naive image image convolution
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 15th Mar 2014
*
*   File Created.
*
* =============================================================================
*/
#include <iostream>
#include <cl_wrapper.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>

//#define NO_CONVOLUTION
int main()
{
    std::cout << "Image Convolution Test!" << std::endl;

    //CL Variables
    iv::CLSetup cl;
    iv::Program* prog;
    iv::KernelLauncher* kl;
    std::vector<std::string> kernelFiles;
    std::string kernelFile = "image_convolution.cl";
    kernelFiles.push_back(kernelFile);

    cl.init();

    //Test specific variables
    size_t numComputeUnits;
    size_t  maxGrpSize, localMemSize, preferredWrkSize;
    size_t globalWrkItems;
    size_t localWrkItems;
    numComputeUnits     = cl.getMaxComputeUnits();
    maxGrpSize          = cl.getMaxWorkGroupSize();
    localMemSize        = cl.getLocalMemSize();

    globalWrkItems = numComputeUnits * maxGrpSize;
    localWrkItems = maxGrpSize;

    std::cout<<"Number of Compute Units   : "<<numComputeUnits<<std::endl
             <<"Max Work Group Size       : "<<maxGrpSize<<std::endl
             <<"Local Memory Size         : "<<localMemSize/1024<<"k"<<std::endl;

    //Image Convolution Varaibles
    cv::Mat jpegFile;
    jpegFile = cv::imread("shiva.jpg");
    //jpegFile = cv::imread("input.bmp");
    //cv::waitKey();

    std::cout<<"Image Size :"<<jpegFile.size()<<"\n";
    cv::cvtColor(jpegFile, jpegFile, CV_BGR2GRAY);
    cv::imshow("GrayScale Image", jpegFile);
    std::cout<<"Total Pixels in File: "<<jpegFile.total()<<"\n";
    std::cout<<"Pixel Data Type :"<<jpegFile.type()<<"\n";
    std::cout<<"Step Value :"<<jpegFile.step<<"\n";
    std::cout<<"Element Size :"<<jpegFile.elemSize()<<"\n";
    std::cout<<"Num of Channels :"<<jpegFile.channels()<<"\n";
    std::cout<<"Num of cols (cols * channesl) :"<<jpegFile.cols * jpegFile.channels()<<"\n";

    // The image format describes how the data will be stored in memory
    cl_image_format format;
    format.image_channel_order     = CL_R;     // single channel
    format.image_channel_data_type = CL_UNORM_INT8; // UCHAR data type

    cl_mem_flags srcFlags = CL_MEM_READ_ONLY |
                            CL_MEM_COPY_HOST_PTR;
    std::cout<<"\nSource Image2D flags:"<<srcFlags<<"\n";
    // Copy the source image to the device
    iv::Image2D* dSourceImage = cl.createImage2D(jpegFile.size().width,
                                                 jpegFile.size().height,
                                                 &format,
                                                 srcFlags,
                                                 jpegFile.step,
                                                 jpegFile.data);
    // Offset within the image to copy from
    size_t origin[3] = {0, 0, 0};
    // Elements to per dimension
    size_t region[3] = {jpegFile.size().width, jpegFile.size().height, 1};

    dSourceImage->write(jpegFile.data, region, origin);

    cl_mem_flags dstFlags = CL_MEM_WRITE_ONLY;
    std::cout<<"\nSource Image2D flags:"<<dstFlags<<"\n";
    iv::Image2D* dOutputImage = cl.createImage2D(jpegFile.size().width,
                                                 jpegFile.size().height,
                                                 &format,
                                                 dstFlags);

    // 45 degree motion blur
    float filter[49] =
    {0,      0,      0,      0,      0,      0,      0,
     0,      0,      0,      0,      0,      0,      0,
     0,      0,     -1,      0,      1,      0,      0,
     0,      0,     -2,      0,      2,      0,      0,
     0,      0,     -1,      0,      1,      0,      0,
     0,      0,      0,      0,      0,      0,      0,
     0,      0,      0,      0,      0,      0,      0};
    // The convolution filter is 7x7
    int filterWidth = 7;
    int filterSize  = filterWidth*filterWidth;  // Assume a square kernel
    iv::Buffer* dFilter = cl.createBuffer(filterSize * sizeof(float),
                                          CL_MEM_READ_ONLY |
                                          CL_MEM_COPY_HOST_PTR, filter);

    iv::Sampler* sampler = cl.createSampler(CL_FALSE,
                                            CL_ADDRESS_CLAMP_TO_EDGE,
                                            CL_FILTER_NEAREST);
    prog = cl.createProgram(kernelFiles);
    prog->buildProgram();

#ifdef NO_CONVOLUTION
    int height = jpegFile.size().height;
    int width = jpegFile.size().width;
    kl = prog->createKernelLauncher("imgRW");
    kl->pGlobal(width, height)
      ->pLocal(4,4);
    kl->pArg(dSourceImage->getMem())
      ->pArg(dOutputImage->getMem())
      ->pArg(sampler->getSampler());
#else if
    int height = jpegFile.size().height;
    int width = jpegFile.size().width;
    kl = prog->createKernelLauncher("convolution");
    kl->pGlobal(width, height)->pLocal(4,4);

//    void convolution(
//       __read_only  image2d_t  sourceImage,
//       __write_only image2d_t  outputImage,
//       int rows,
//       int cols,
//       __constant float* filter,
//       int filterWidth)
    kl->pArg(dSourceImage->getMem())
            ->pArg(dOutputImage->getMem())
            ->pArg(height)
            ->pArg(width)
            ->pArg(dFilter->getMem())
            ->pArg(filterSize);

    //   ->pArg(sampler->getSampler())
#endif
    kl->run();

    cv::waitKey();
    dOutputImage->read(jpegFile.data, origin, region);
    cv::imshow("Final", jpegFile);
    cv::waitKey();

    delete dSourceImage;
    delete dOutputImage;
    return 0;
}








//// FLOAT
///
//float* pixelData = new float[jpegFile.total() * jpegFile.channels()];
////uchar* pixelData = (uchar*)malloc(jpegFile.total() * sizeof(uchar));
//int c = 0;
//for (int y = 0; y < jpegFile.rows; ++y)
//{
//  for (int x = 0; x < (jpegFile.cols * jpegFile.channels() ); ++x)
//  {
//          pixelData[y * (jpegFile.cols * jpegFile.channels())+ x ] = jpegFile.data[y * (jpegFile.cols * jpegFile.channels())+ x ];
//          printf("%d, ",  pixelData[y * (jpegFile.cols * jpegFile.channels())+ x ]);
//          ++c;
//  }
//  printf("\n");
//}
//std::cout<<"\n\n\n0. Done: Number of Pixels: "<<c<<": \n\n\n\n\n";
//// return 0;

//cv::Mat intDataPtr(jpegFile.size(), CV_32FC4, pixelData, jpegFile.step);
//std::cout<<"Type: "<<intDataPtr.type()<<"\n";
//// std::cout<<"intDataPtr Data: "<<intDataPtr<<"\n";
//cv::imshow("intDataPtr", intDataPtr);
//cv::waitKey();



//    uchar* pixelData = new uchar[jpegFile.total() * jpegFile.channels()];
//    //uchar* pixelData = (uchar*)malloc(jpegFile.total() * sizeof(uchar));

//    for (int y = 0; y < jpegFile.rows; ++y)
//      for (int x = 0; x < (jpegFile.cols * jpegFile.channels() ); ++x)
//      {
//              pixelData[y * (jpegFile.cols * jpegFile.channels())+ x ] = jpegFile.data[y * (jpegFile.cols * jpegFile.channels())+ x ];
//             // printf("%d ", jpegFile.data[y * jpegFile.cols + x ]);
//      }
//   // std::cout<<"\n\n\n0. Done\n\n\n\n\n";
//   // return 0;

//    cv::Mat intDataPtr(jpegFile.size(), CV_8UC4, pixelData, jpegFile.step);
//    std::cout<<"Type: "<<intDataPtr.type()<<"\n";
//   // std::cout<<"intDataPtr Data: "<<intDataPtr<<"\n";
//    cv::imshow("intDataPtr", intDataPtr);
//    cv::waitKey();
