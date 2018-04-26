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
*   FILENAME            : optical_flow.cpp
*
*   DESCRIPTION         : A example to find optical flow on GPU
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 15th May 2014
*
*   File Created.
*
* =============================================================================
*/

#define USE_CAMERA 1

#include <cl_wrapper.hpp>
#include <cv_wrapper.h>

int main(int argc, char* argv[])
{
    //============================= CL Setup
    iv::CLSetup cl;
    iv::Program* prog;
    iv::KernelLauncher* kl;
    std::string file = "optical_flow.cl";
    std::vector<std::string> kernelFiles;
    kernelFiles.push_back(file);

    cl.init();
    prog = cl.createProgram(kernelFiles);
    prog->buildProgram();
    kl = prog->createKernelLauncher("optical_flow");

#if USE_CAMERA
    //============================= CV Setup
    cv::VideoCapture cap(-1);
    //cv::VideoCapture cap("/home/satyam/Desktop/dump/TheDictator.avi");
    cv::Mat previousFrame;

    int width, height, step;
    char key;

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);

    cap >> previousFrame;
    cv::cvtColor(previousFrame, previousFrame, CV_BGR2BGRA);

    // Initialize the resultatnt matrix
    // with input frame parameters
    cv::Mat currentFrame(previousFrame.size(),previousFrame.type());;
    cv::Mat opticalFrame(previousFrame.size(),previousFrame.type());;
    currentFrame = previousFrame.clone();
    opticalFrame = previousFrame.clone();

    width   =   previousFrame.size().width;
    height  =   previousFrame.size().height;
    step    =   previousFrame.step;

    DEBUG_VALUE("Image Width    : ", width);
    DEBUG_VALUE("Image Height   : ", height);
    DEBUG_VALUE("Image Step     : ", step);
    DEBUG_VALUE("Image Type     : ", getImgType(previousFrame.type()));

    //============================= Kernal Variables
    cl_image_format frameFormat;
    frameFormat.image_channel_data_type = CL_UNORM_INT8;
    frameFormat.image_channel_order = CL_RGBA;

    // Offset within the image to copy from
    size_t origin[3] = {0, 0, 0};
    // Elements to per dimension
    size_t region[3] = {previousFrame.size().width, previousFrame.size().height, 1};

    iv::Image2D* devFrameBuffer1 = cl.createImage2D(width, height,
                                                    &frameFormat,
                                                    CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                                    step,
                                                    previousFrame.data);
    iv::Image2D* devFrameBuffer2 = cl.createImage2D(width, height,
                                                    &frameFormat,
                                                    CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                                    step,
                                                    previousFrame.data);
    iv::Image2D* devFrameResult = cl.createImage2D(width, height,
                                                   &frameFormat,
                                                   CL_MEM_WRITE_ONLY);

    float scale = 10;
    float offset = 1;
    float lambda = 0.0025;
    float threshold = 1.0;

    while(key != 27)
    {
        cap >> currentFrame;
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2BGRA);

        devFrameBuffer1->write(previousFrame.data, origin, region);
        devFrameBuffer2->write(currentFrame.data, origin, region);

        //============================= Run Kernal
        kl->pGlobal(width, height)->pLocal(4,4);
        kl->pArg(devFrameBuffer1->getMem())
                ->pArg(devFrameBuffer2->getMem())
                ->pArg(devFrameResult->getMem())
                ->pArg(scale)
                ->pArg(offset   )
                ->pArg(lambda)
                ->pArg(threshold);
        kl->run();

        //============================= Read Kernal output
        devFrameResult->read(opticalFrame.data, origin, region);

        cv::imshow("Camera Frames", currentFrame);
        cv::imshow("Otical Flow", opticalFrame);
        key = cv::waitKey(10);

        currentFrame.copyTo(previousFrame);
    }
    delete devFrameBuffer1;
    delete devFrameBuffer2;
    delete devFrameResult;

#else
    //============================= CV Setup
    cv::Mat pngFile1;
    cv::Mat pngFile2;
    int width, height, step;

    //    pngFile1 = cv::imread("basketball1.png");
    //    pngFile2 = cv::imread("basketball2.png");
    //    cv::imshow("basketball1.png", pngFile1);
    //    cv::imshow("basketball2.png", pngFile2);
    //    cv::cvtColor(pngFile1, pngFile1, CV_BGR2GRAY);
    //    cv::cvtColor(pngFile2, pngFile2, CV_BGR2GRAY);

    pngFile1 = cv::imread("rubberwhale1.png");
    pngFile2 = cv::imread("rubberwhale2.png");
    cv::imshow("rubberwhale1.png", pngFile1);
    cv::imshow("rubberwhale2.png", pngFile2);
    cv::cvtColor(pngFile1, pngFile1, CV_BGR2BGRA);
    cv::cvtColor(pngFile2, pngFile2, CV_BGR2BGRA);

    cv::waitKey();

    if(pngFile1.cols != pngFile2.cols && pngFile1.rows != pngFile2.rows)
        ERROR_PRINT_STRING("Please choose images of same size");


    width   =   pngFile1.size().width;
    height  =   pngFile1.size().height;
    step    =   pngFile1.step;

    DEBUG_VALUE("Image Width    : ", width);
    DEBUG_VALUE("Image Height   : ", height);
    DEBUG_VALUE("Image Step     : ", step);
    DEBUG_VALUE("Image Type     : ", getImgType(pngFile1.type()));

    //============================= Kernal Variables
    cl_image_format pngFormat;
    pngFormat.image_channel_data_type = CL_UNORM_INT8;
    pngFormat.image_channel_order = CL_RGBA;

    // Offset within the image to copy from
    size_t origin[3] = {0, 0, 0};
    // Elements to per dimension
    size_t region[3] = {pngFile1.size().width, pngFile1.size().height, 1};


    iv::Image2D* pngBuffer1 = cl.createImage2D(width, height,
                                               &pngFormat,
                                               CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                               step,
                                               pngFile1.data);

    iv::Image2D* pngBuffer2 = cl.createImage2D(width, height,
                                               &pngFormat,
                                               CL_MEM_READ_ONLY,
                                               step,
                                               pngFile2.data);

    iv::Image2D* pngResult = cl.createImage2D(width, height,
                                              &pngFormat,
                                              CL_MEM_WRITE_ONLY);

    pngBuffer1->write(pngFile1.data, origin, region);
    pngBuffer2->write(pngFile2.data, origin, region); // To test the write API

    float scale = 10;
    float offset = 5;
    float lambda = 0.005;
    float threshold = 1.0;
    //============================= Run Kernal
    kl->pGlobal(width, height)->pLocal(4,4);

    kl->pArg(pngBuffer1->getMem())
            ->pArg(pngBuffer2->getMem())
            ->pArg(pngResult->getMem())
            ->pArg(scale)
            ->pArg(offset)
            ->pArg(lambda)
            ->pArg(threshold);


    kl->run();

    //============================= Read Kernal output
    pngResult->read(pngFile2.data, origin, region);\

    cv::imshow("Optical Flow", pngFile2);
    DEBUG_VALUE("Resultant Optical Matrix", pngFile2);
    cv::waitKey();

#endif
}








