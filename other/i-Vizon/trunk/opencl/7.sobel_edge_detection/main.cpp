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
*   FILENAME            : test_sobel_edge_detection.cpp
*
*   DESCRIPTION         : A test code using cl_wrapper, to test
*                         sobel_filter kernel
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 28th Mar 2014
*
*   File Created.
*
* =============================================================================
*/
#include <cl_wrapper.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

int main()
{
    cout << "Sobel Filter" << endl;
    //CL Variables
    iv::CLSetup cl;
    iv::Program* prog;
    iv::KernelLauncher* kl;
    std::string file = "sobel_filter.cl";
    std::vector<std::string> kernelFiles;
    kernelFiles.push_back(file);

    cl.init();
    prog = cl.createProgram(kernelFiles);
    prog->buildProgram();
    kl = prog->createKernelLauncher("sobel");


    //Sobel Filter Varaibles
    cv::Mat sourceImageData;
    sourceImageData = cv::imread("shiva.jpg");
    /**
     0****************************************1023
     ******************************************
     ******************************************
     ******************************************
     ******************************************
     ******************************************
     ******************************************
     ******************************************
     767***************************************
     */
    cv::cvtColor(sourceImageData, sourceImageData, cv::COLOR_BGRA2GRAY);
    cv::imshow("Image Rotate", sourceImageData);

    uint H = sourceImageData.size().height;
    uint W = sourceImageData.size().width;
    cout<<H<<W<<"\n\n\n";
    iv::Buffer* dSrcImageData = cl.createBuffer(sizeof(uchar) * W * H,
                                            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                            sourceImageData.data);
    iv::Buffer* dDstImageData = cl.createBuffer(sizeof(uchar) * W * H,
                                                CL_MEM_WRITE_ONLY,
                                                NULL);
    kl->pGlobal(H,W)->pLocal(16,16); // 768 * 1024
    kl->pArg(dSrcImageData->getMem())
            ->pArg(dDstImageData->getMem());
    kl->run();

    //Time to see the output
    dDstImageData->read(sourceImageData.data,
                        sizeof(uchar) * W * H);

    cv::waitKey();
    cv::imshow("CL Output", sourceImageData);
    cv::waitKey();

    return 0;
}

